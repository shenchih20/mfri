#pragma once

#include <map>
#include <mfri/srv_server.hpp>
#include <mfri/param_value.hpp>
#include <mfri/idl/SetParametersPubSubTypes.hpp>
#include <mfri/idl/GetParametersPubSubTypes.hpp>


namespace mfri
{
class MfriParamServer
{
public:
    MfriParamServer(const std::string& node_name) :
        mNodeName(node_name),
        mSetParameter( node_name + "/set_parameters", "rcl_interfaces::srv::dds_::SetParameters_"),
        mGetParameter( node_name + "/get_parameters", "rcl_interfaces::srv::dds_::GetParameters_")
    {
    
        mSetParameter.mRequest.set_data_available_callback(
            [this]()
            {
                this->set_parameter_callback( mSetParameter.request_data(), mSetParameter.response_data());
                mSetParameter.mResponse.publish();

            });

        mGetParameter.mRequest.set_data_available_callback(
            [this]()
            {
                this->get_parameter_callback( mGetParameter.request_data(), mGetParameter.response_data());
                mGetParameter.mResponse.publish();
            });
    }

    bool initialize(DomainParticipant* participant)
    {
        auto ret_1 = mSetParameter.initialize(participant);

        auto ret_2 = mGetParameter.initialize(participant);

        return (ret_1 == RETCODE_OK && ret_2 == RETCODE_OK);
    }


    template<typename ParameterT>    
    MfriParamValueSPtr declare_parameter( const std::string& name, const ParameterT& value)
    {
        auto it = mParameters.find(name);
        if (it != mParameters.end())
        {
            return it->second;
        }

        auto param_value = std::make_shared<MfriParamValue>(value);
        mParameters[name] = param_value;
        return param_value;
    }
    

private:
    void set_parameter_callback( const rcl_interfaces::srv::SetParameters_Request& request, rcl_interfaces::srv::SetParameters_Response& response)
    {
        response.results().clear();
        for( const auto& param : request.parameters())
        {
            auto result = rcl_interfaces::msg::SetParametersResult();
            auto it = mParameters.find(param.name());
            if (it == mParameters.end())
            {
                result.successful(false);
                result.reason("Parameter not found");
                response.results().push_back(result);
                continue;
            }

            auto& src = it->second->value();

            if(src.type() != param.value().type())
            {
                result.successful(false);
                result.reason("Type mismatch");
                response.results().push_back(result);
                continue;
            }

            src = param.value();
            result.successful(true);
            result.reason("Parameter set successfully");
            response.results().push_back(result);
        }

    }

    void get_parameter_callback( const rcl_interfaces::srv::GetParameters_Request& request, rcl_interfaces::srv::GetParameters_Response& response)
    {
        response.values().clear();
        for (const auto& name : request.names())
        {
            auto it = mParameters.find(name);
            if (it != mParameters.end())
            {
                response.values().push_back(it->second->value());
            }           
        }
    }    

private:
    std::map<std::string, mfri::MfriParamValueSPtr> mParameters;

    std::string mNodeName;
    
    MfriSrvServer<rcl_interfaces::srv::SetParameters_RequestPubSubType, 
                  rcl_interfaces::srv::SetParameters_Request, 
                  rcl_interfaces::srv::SetParameters_ResponsePubSubType, 
                  rcl_interfaces::srv::SetParameters_Response> mSetParameter;

    MfriSrvServer<rcl_interfaces::srv::GetParameters_RequestPubSubType, 
                  rcl_interfaces::srv::GetParameters_Request, 
                  rcl_interfaces::srv::GetParameters_ResponsePubSubType, 
                  rcl_interfaces::srv::GetParameters_Response> mGetParameter;
};
}