#pragma once


#include <mfri/publisher.hpp>
#include <mfri/subscriber.hpp>


namespace mfri
{

template <class RequestType, class RequestData, class ResponseType, class ResponseData>
class MfriSrvServer
{ 
public:
    MfriSrvServer(const std::string& topic_name, const std::string& data_type_name) :
        mRequest(std::string("rq/") + topic_name + "Request", data_type_name + "Request_"),
        mResponse(std::string("rr/") + topic_name + "Reply", data_type_name + "Response_")
    {
            mRequest.set_data_available_callback(
            [this]()
            {                                
                on_request_received();
                mResponse.publish();
            });
    }

    bool initialize(DomainParticipant* participant)
    {
        if (false == mRequest.initialize(participant))
        {
            return false;
        }
        
        if (false == mResponse.initialize(participant))
        {
            mRequest.close();
            return false;
        }

        return true;
    }

    void close()
    {
        mRequest.close();
        mResponse.close();
    }  

    virtual void on_request_received()
    {
        
    }

protected:
    RequestData & request_data()
    {
        return mRequest.Data();
    }

    ResponseData & response_data()
    {
        return mResponse.Data();
    }

private:
    MfriSubscriberBase<RequestType, RequestData> mRequest;
    MfriPublisherBase<ResponseType, ResponseData> mResponse;

       
};


}