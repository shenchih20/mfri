#pragma once


#include <mfri/srv_client.hpp>
#include <mfri/param_value.hpp>
#include <mfri/idl/SetParametersPubSubTypes.hpp>
#include <mfri/idl/GetParametersPubSubTypes.hpp>

namespace mfri
{

class MfriParamClient
{

public:
    MfriParamClient(const std::string& node_name) :
        mNodeName(node_name),
        mSetParameter( node_name + "/set_parameters", "rcl_interfaces::srv::dds_::SetParameters_"),
        mGetParameter( node_name + "/get_parameters", "rcl_interfaces::srv::dds_::GetParameters_")
    {
        
    }

    bool initialize(DomainParticipant* participant)
    {
        auto ret_1 = mSetParameter.initialize(participant);

        auto ret_2 = mGetParameter.initialize(participant);

        return (ret_1 == RETCODE_OK && ret_2 == RETCODE_OK);
    }

    template<typename ParameterT>    
    ReturnCode_t set_param(const std::string& name, const ParameterT& value, rcl_interfaces::msg::SetParametersResult& result)
    {
        rcl_interfaces::msg::Parameter param;        
        param.name(name);
        param.value(MfriParamValue(value).value());

        auto& request = mSetParameter.request_data();
        request.parameters().clear();
        request.parameters().push_back(param);

        auto ret = mSetParameter.send_request();
        if (ret != RETCODE_OK)
        {            
            return ret;
        }

        auto& response = mSetParameter.response_data();
        if(response.results().empty())
        {            
            return RETCODE_ERROR;
        }
                    
        result = response.results().at(0);

        return RETCODE_OK;                    
    }


    ReturnCode_t get_param(const std::string& name, rcl_interfaces::msg::ParameterValue& value)
    {
        auto& request = mGetParameter.request_data();

        request.names().clear();
        request.names().push_back(name);

        auto ret = mGetParameter.send_request();
        if (ret != RETCODE_OK)
        {
            return ret;
        }

        auto& response = mGetParameter.response_data();
        if (response.values().empty())
        {
            return RETCODE_ERROR;
        }

        value = response.values().at(0);
        return RETCODE_OK;
    }

private:
    std::string mNodeName;

    
    MfriSrvClient<rcl_interfaces::srv::SetParameters_RequestPubSubType, 
                  rcl_interfaces::srv::SetParameters_Request, 
                  rcl_interfaces::srv::SetParameters_ResponsePubSubType, 
                  rcl_interfaces::srv::SetParameters_Response> mSetParameter;

    MfriSrvClient<rcl_interfaces::srv::GetParameters_RequestPubSubType, 
                  rcl_interfaces::srv::GetParameters_Request, 
                  rcl_interfaces::srv::GetParameters_ResponsePubSubType, 
                  rcl_interfaces::srv::GetParameters_Response> mGetParameter;

};

    
} // namespace mfri
