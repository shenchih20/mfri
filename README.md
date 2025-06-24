# mfri
mini fastdds ros2 interface



## Description

**mfri** provides a minimal interface for using Fast DDS as a ROS 2-like middleware. It enables easy creation of publishers, subscribers, service servers, and service clients using ROS 2 IDL types, but without requiring a full ROS 2 installation. This is useful for lightweight or embedded applications that need ROS 2 interoperability.

## Features

- ROS 2-style publisher/subscriber API
- ROS 2-style service server/client API
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

mfri::MfriPublisher<HelloWorldPubSubType, HelloWorld> publisher("Hello", "HelloWorld");
publisher.initialize(participant);

HelloWorld data;
data.index(1);
data.message("Hello World");
publisher.publish(data);
```

### Subscriber

```cpp
#include "mfri/subscriber.hpp"
#include "idl/HelloWorldPubSubTypes.hpp"

class MySubscriber : public mfri::MfriSubscriber<HelloWorldPubSubType, HelloWorld> {
public:
    MySubscriber() : MfriSubscriber("Hello", "HelloWorld") {}
    void process_data() override {
        std::cout << "Received: " << mData.index() << ", " << mData.message() << std::endl;
    }
};

MySubscriber subscriber;
subscriber.initialize(participant);
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

AddTwoIntsServer server;
server.initialize(participant);
```

### Service Client

```cpp
#include "mfri/srv_client.hpp"
#include "idl/AddTwoInts_RequestPubSubTypes.hpp"
#include "idl/AddTwoInts_ResponsePubSubTypes.hpp"

mfri::MfriSrvClient<
    example_interfaces::srv::AddTwoInts_RequestPubSubType,
    example_interfaces::srv::AddTwoInts_Request,
    example_interfaces::srv::AddTwoInts_ResponsePubSubType,
    example_interfaces::srv::AddTwoInts_Response> client("add_two_ints", "example_interfaces::srv::dds_::AddTwoInts_");

client.initialize(participant);

client.request_data().a(1);
client.request_data().b(2);
client.send_request();
int64_t sum = client.response_data().sum();
std::cout << "Sum: " << sum << std::endl;
```

## License

See [LICENSE](LICENSE).
