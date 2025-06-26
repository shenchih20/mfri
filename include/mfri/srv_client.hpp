#pragma once

#include <mfri/publisher.hpp>
#include <mfri/subscriber.hpp>


namespace mfri
{

template <class RequestType, class RequestData, class ResponseType, class ResponseData>    
class MfriSrvClient
{
public:
    MfriSrvClient(const std::string&topic_name, const std::string& data_type_name) :
        mRequest(std::string("rq/") + topic_name +"Request", data_type_name + "Request_"),
        mResponse(std::string("rr/") + topic_name +"Reply", data_type_name + "Response_")
    {

        mResponse.set_data_available_callback(
            [this]()
            {                
                std::unique_lock<std::mutex> lock(mMutex);
                mResponseReceived = true;
                on_data_available();
                mCondVar.notify_all();
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

    virtual void on_data_available()
    {

    }

protected:
    friend class MfriParamClient;
    ReturnCode_t send_request()
    {
        {
            std::lock_guard<std::mutex> lock(mMutex);
            mResponseReceived = false; // Reset response flag before sending request
        }
        
        auto ret = mRequest.publish();
        if (ret != RETCODE_OK)
        {
            return ret;
        }

        {
            std::unique_lock<std::mutex> lock(mMutex);
            if (!mCondVar.wait_for(lock, std::chrono::seconds(mTimeOut), [this]{ return mResponseReceived; }))
            {
                return RETCODE_TIMEOUT; // Wait for response with timeout
            }
        }
       
        return RETCODE_OK;
    }

    RequestData & request_data()
    {
        return mRequest.Data();
    }

    ResponseData & response_data()
    {
        return mResponse.Data();
    }



private:
    int32_t mTimeOut = 2; // seconds
    std::mutex mMutex;
    std::condition_variable mCondVar;
    bool mResponseReceived = false;

    MfriPublisherBase<RequestType, RequestData> mRequest;
    MfriSubscriberBase<ResponseType, ResponseData> mResponse;

};

}
