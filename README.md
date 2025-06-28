# mfri
mini fastdds ros2 interface



## Description

**mfri** provides a minimal interface for using Fast DDS as a ROS 2-like middleware. It enables easy creation of publishers, subscribers, service servers, and service clients using ROS 2 IDL types, but without requiring a full ROS 2 installation. This is useful for lightweight or embedded applications that need ROS 2 interoperability.

## Features

- ROS 2-style publisher/subscriber
- ROS 2-style service server/client
- ROS 2-style parameter server/client
- Uses Fast DDS for transport
- Simple C++ interface

## Install Fast DDS on Windows

1. **Install vcpkg**  
   Follow the official guide:  
   https://learn.microsoft.com/en-us/vcpkg/get_started/get-started?pivots=shell-powershell

2. **Install Fast DDS using vcpkg**  
   ```
   vcpkg install fastdds
   ```
   See https://vcpkg.link/ports/fastdds for more details.

## Example Usage

### Publisher

```cpp
#include "mfri/publisher.hpp"
#include "idl/HelloWorldPubSubTypes.hpp"

class HelloWorldPublisher : public mfri::MfriPublisher<HelloWorldPubSubType, HelloWorld>
{
public:
    
    HelloWorldPublisher(const std::string& topic_name) : MfriPublisher(topic_name, "HelloWorld")
    {
        

    }

    void write_data(uint32_t index, const std::string& message)
    {
        
        mData.index(index);
        mData.message(message);

        publish();
    }

};
```

### Subscriber

```cpp
#include "mfri/subscriber.hpp"
#include "idl/HelloWorldPubSubTypes.hpp"

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
    }
};
```

### Service Server

```cpp
#include "mfri/srv_server.hpp"
#include "idl/AddTwoInts_RequestPubSubTypes.hpp"
#include "idl/AddTwoInts_ResponsePubSubTypes.hpp"

class AddTwoIntsServer : public mfri::MfriSrvServer<
    example_interfaces::srv::AddTwoInts_RequestPubSubType,
    example_interfaces::srv::AddTwoInts_Request,
    example_interfaces::srv::AddTwoInts_ResponsePubSubType,
    example_interfaces::srv::AddTwoInts_Response>
{
public:
    AddTwoIntsServer() : MfriSrvServer("add_two_ints", "example_interfaces::srv::dds_::AddTwoInts_") {}
    void on_request_received() override {
        int64_t sum = request_data().a() + request_data().b();
        response_data().sum(sum);
    }
};

```

### Service Client

```cpp
#include "mfri/srv_client.hpp"
#include "idl/AddTwoInts_RequestPubSubTypes.hpp"
#include "idl/AddTwoInts_ResponsePubSubTypes.hpp"

class AddTwoIntsSrvClient : public mfri::MfriSrvClient<example_interfaces::srv::AddTwoInts_RequestPubSubType,        
                                                       example_interfaces::srv::AddTwoInts_Request, 
                                                       example_interfaces::srv::AddTwoInts_ResponsePubSubType, 
                                                       example_interfaces::srv::AddTwoInts_Response>
{
public:
     AddTwoIntsSrvClient() : MfriSrvClient("add_two_ints", 
                                           "example_interfaces::srv::dds_::AddTwoInts_")
     {

     }
    

     ReturnCode_t get_sum(int64_t a, int64_t b, int64_t& sum)
     {
        request_data().a(a);
        request_data().b(b);

        auto ret = send_request();
        if (ret != RETCODE_OK)
        {            
            return ret;
        }
        sum = response_data().sum();
               
        return RETCODE_OK;                         
     }
     
};
```

### Parameter Server

```cpp

#include "mfri/param_server.hpp"

mfri::MfriParamServer param_server("minimal_param_node");

param_server.declare_parameter("my_parameter", "test");

```

### Parameter Client

```cpp

#include "mfri/param_client.hpp"

mfri::MfriParamClient param_client("minimal_param_node");

rcl_interfaces::msg::SetParametersResult result;
param_client.set_param("my_parameter", "test", result);

rcl_interfaces::msg::ParameterValue value;
param_client.get_param("my_parameter", value);

```

