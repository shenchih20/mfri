#include <gtest/gtest.h>
#include <thread>
#include <chrono>
#include "idl/HelloWorldPubSubTypes.hpp"
#include "idl/AddTwoInts_RequestPubSubTypes.hpp"
#include "idl/AddTwoInts_ResponsePubSubTypes.hpp"
#include "mfri/publisher.hpp"
#include "mfri/subscriber.hpp"
#include "mfri/srv_client.hpp"
#include "mfri/srv_server.hpp"
#include "mfri/param_client.hpp"
#include "mfri/param_server.hpp"



class mfri_test : public ::testing::Test
{
protected:
     virtual void SetUp() override
    {
       
    }

    virtual void TearDown() override
    {
        
    }
 
};


class HelloWorldPublisher : public mfri::MfriPublisher<HelloWorldPubSubType, HelloWorld>
{
public:
    uint32_t Index = 0;

    HelloWorldPublisher(const std::string& topic_name) : MfriPublisher(topic_name, "HelloWorld")
    {
        

    }

    void write_data(uint32_t index, const std::string& message)
    {
        Index = index;
        mData.index(index);
        mData.message(message);

        publish();
    }

};

class HelloWorldSubscriber : public mfri::MfriSubscriber<HelloWorldPubSubType, HelloWorld>
{   
public:
    uint32_t Index = 0;
    HelloWorldSubscriber(const std::string& topic_name) : MfriSubscriber(topic_name, "HelloWorld")
    {
        
    }

    void process_data() override
    {
        Index = mData.index();
        std::cout << "Received data: " << mData.index() << ", " << mData.message() << std::endl;
    }
};


TEST_F(mfri_test, test_pub_sub)
{

    int mDomainId = 0;
	std::string mDomainName = "mfri";
	std::string mParticipantName = "mfri_participant";
	DomainParticipant* mParticipant = nullptr;

	DomainParticipantQos participantQos;
	participantQos.name(mParticipantName);
	mParticipant = DomainParticipantFactory::get_instance()->create_participant(mDomainId, participantQos);

    
    HelloWorldPublisher publisher("Hello");
    HelloWorldSubscriber subscriber("Hello");
    
    ASSERT_EQ(publisher.initialize(mParticipant), true) << "Publisher initialization failed";
    ASSERT_EQ(subscriber.initialize(mParticipant), true) << "Subscriber initialization failed";
    


    int32_t index = 0;
    while(index < 5)
    {
        std::cout << "Publishing data: " << index << std::endl;
        publisher.write_data(index++, "Hello World");          
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        ASSERT_EQ(subscriber.Index, publisher.Index) << "Subscriber index does not match publisher index";
    }

    if (mParticipant)
    {
        DomainParticipantFactory::get_instance()->delete_participant(mParticipant);
        mParticipant = nullptr;
    }
}


class AddTwoIntsSrvServer : public mfri::MfriSrvServer<example_interfaces::srv::AddTwoInts_RequestPubSubType, 
                                                       example_interfaces::srv::AddTwoInts_Request, 
                                                       example_interfaces::srv::AddTwoInts_ResponsePubSubType, 
                                                       example_interfaces::srv::AddTwoInts_Response>
{
public:
     AddTwoIntsSrvServer() : MfriSrvServer("add_two_ints", "example_interfaces::srv::dds_::AddTwoInts_")
     {

     }

     int64_t A;
     int64_t B;
     int64_t Sum;

    void on_request_received() override
    {
        A = request_data().a();
        B = request_data().b();
        Sum = A + B;

        std::cout << "server request: a = " << A << ", b = " << B << ", sum = " << Sum << std::endl;

        response_data().sum(Sum);
        
    }
     
};


class AddTwoIntsSrvClient : public mfri::MfriSrvClient<example_interfaces::srv::AddTwoInts_RequestPubSubType, 
                                                       example_interfaces::srv::AddTwoInts_Request, 
                                                       example_interfaces::srv::AddTwoInts_ResponsePubSubType, 
                                                       example_interfaces::srv::AddTwoInts_Response>
{
public:
     AddTwoIntsSrvClient() : MfriSrvClient("add_two_ints", "example_interfaces::srv::dds_::AddTwoInts_")
     {

     }

     int64_t A;
     int64_t B;
     int64_t Sum;

     ReturnCode_t get_sum(int64_t a, int64_t b, int64_t& sum)
     {
        request_data().a(a);
        request_data().b(b);

        auto ret = send_request();
        if (ret != RETCODE_OK)
        {
            std::cerr << "Failed to send request: " << ret << std::endl;
            return ret;
        }
        sum = response_data().sum();

        A = a;
        B = b;
        Sum = sum;

        std::cout << "client a = " << a << ", b = " << b << ", sum = " << sum << std::endl;
        return RETCODE_OK;                         
     }
     
};

TEST_F(mfri_test, test_service)
{
    int mDomainId = 0;
    std::string mDomainName = "mfri";
    std::string mParticipantName = "mfri_participant";
    DomainParticipant* mParticipant = nullptr;

    DomainParticipantQos participantQos;
    participantQos.name(mParticipantName);
    mParticipant = DomainParticipantFactory::get_instance()->create_participant(mDomainId, participantQos);

    AddTwoIntsSrvServer server;
    ASSERT_EQ(server.initialize(mParticipant), true) << "Server initialization failed";

    AddTwoIntsSrvClient client;    
    ASSERT_EQ(client.initialize(mParticipant), true) << "Client initialization failed";

    int64_t index = 0;
    int64_t sum = 0;
    while(index < 10)
    {        
        client.get_sum(index++, index++, sum);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        ASSERT_EQ(client.A, server.A) << "Client A does not match server A";
        ASSERT_EQ(client.B, server.B) << "Client B does not match server B";
        ASSERT_EQ(client.Sum, server.Sum) << "Client Sum does not match server Sum";
    }

    if (mParticipant)
    {
        DomainParticipantFactory::get_instance()->delete_participant(mParticipant);
        mParticipant = nullptr;
    }
}


TEST_F(mfri_test, test_param)
{
     int mDomainId = 0;
    std::string mDomainName = "mfri";
    std::string mParticipantName = "mfri_participant";
    DomainParticipant* mParticipant = nullptr;

    DomainParticipantQos participantQos;
    participantQos.name(mParticipantName);
    mParticipant = DomainParticipantFactory::get_instance()->create_participant(mDomainId, participantQos);


    mfri::MfriParamClient param_client("minimal_param_node");
    mfri::MfriParamServer param_server("minimal_param_node");

    param_client.initialize(mParticipant);
    param_server.initialize(mParticipant);


    const std::string param_name = "my_parameter";

    param_server.declare_parameter(param_name, "test");


    int32_t index = 0;
    rcl_interfaces::msg::ParameterValue p_value;
    rcl_interfaces::msg::SetParametersResult result;
    while( 5 > index++ )
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        auto target = std::to_string(index);
        std::cout << "Setting parameter: " << param_name << " to " << target << std::endl;
        param_client.set_param(param_name, target, result);
        param_client.get_param(param_name, p_value);

        ASSERT_EQ(p_value.string_value(), target);
    }
    

}



