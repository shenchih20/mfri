#pragma once

#include <functional>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>


using namespace eprosima::fastdds::dds;

namespace mfri
{   

template <class Type_t, class Data_t>
class MfriSubscriberBase : public DataReaderListener
{
public:
    MfriSubscriberBase(const std::string&topic_name, const std::string& data_type_name) :
        mTopicName(topic_name),
        mDataTypeName(data_type_name),
        mDataType(new Type_t())
    {
    }

    ~MfriSubscriberBase()
    {
        close();
    }


    bool initialize(DomainParticipant* participant)
    {
        if (true == mReady)
        {
            return false;
        }

        mReady = false;
        if (nullptr == participant)
        {
            return false;
        }

        mParticipant = participant;
        

        mDataType.register_type(participant, mDataTypeName);

        TopicQos topic_qos;
        participant->get_default_topic_qos(topic_qos);

        mTopic = participant->find_topic(mTopicName, Duration_t());
        if (nullptr == mTopic)
        {
            mTopic = participant->create_topic(mTopicName, mDataTypeName, topic_qos);
            if (nullptr == mTopic)
            {
                return false;
            }

        }
       
        SubscriberQos sub_qos;	
        participant->get_default_subscriber_qos(sub_qos);
        mSubscriber = participant->create_subscriber(sub_qos, nullptr, StatusMask::none());
        if (nullptr == mSubscriber)
        {
            participant->delete_topic(mTopic);
            return false;
        }

        DataReaderQos reader_qos;	
        mSubscriber->get_default_datareader_qos(reader_qos);
        mReader = mSubscriber->create_datareader(mTopic, reader_qos, this);
        if (nullptr == mReader)
        {
            participant->delete_subscriber(mSubscriber);
            participant->delete_topic(mTopic);
            return false;
        }

        mReady = true;
        return true;
    }

    void close()
    {
        if (false == mReady)
        {
            return;
        }

        mReady = false;

        if (nullptr != mReader)
        {
            mSubscriber->delete_datareader(mReader);
        }

        if (nullptr != mSubscriber)
        {
            mParticipant->delete_subscriber(mSubscriber);
        }

        if (nullptr != mTopic)
        {
            mParticipant->delete_topic(mTopic);
        }
    }

    void on_data_available(DataReader* reader) override
    {
        static_cast<void>(reader);
        if (nullptr == reader)
        {
            return;
        }

        SampleInfo info;
        if (reader->take_next_sample(&mData, &info) == RETCODE_OK)
        {
            if (info.valid_data)
            {
                // Process the data
                process_data();

                // Notify that data is available
                if (mDataAvailableCallback)
                {
                    mDataAvailableCallback();
                }
            }
        }
    }


    virtual void process_data() 
    {

    }

   
    void set_data_available_callback(std::function<void()> callback)
    {
        mDataAvailableCallback = callback;
    }



    Data_t& Data()
    {
        return mData;
    }

protected:
    Data_t mData;

private:
    std::function<void()> mDataAvailableCallback = nullptr;
    

    bool mReady = false;

    std::string mDataTypeName = "";
    TypeSupport mDataType;

    Topic* mTopic = nullptr;
    std::string mTopicName = "rt/";

    DomainParticipant* mParticipant = nullptr;
    Subscriber* mSubscriber = nullptr;

    DataReader* mReader = nullptr;
     
};

template <class Type_t, class Data_t>    
class MfriSubscriber : public MfriSubscriberBase<Type_t, Data_t>
{
public:
    MfriSubscriber(const std::string& topic_name, const std::string& data_type_name) : 
    MfriSubscriberBase<Type_t, Data_t>(std::string("rt/")+topic_name, data_type_name)
    {
        
    }
      
      
};

} // namespace mfri
