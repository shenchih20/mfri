#pragma once


#include <mfri/srv_client.hpp>
#include <mfri/idl/ParameterType.hpp>
#include <mfri/idl/SetParametersPubSubTypes.hpp>

namespace mfri
{

class MfriParamClient
{

public:
    MfriParamClient(const std::string& node_name) :
        mNodeName(node_name),
        mSetParameter( node_name + "/set_parameters", "rcl_interfaces::srv::dds_::SetParameters_")
    {
        
    }

    bool initialize(DomainParticipant* participant)
    {        
        return mSetParameter.initialize(participant);
    }

    ReturnCode_t set_param(const std::string& name, const std::string& value, bool &result, std::string &ret_msg)
    {
        rcl_interfaces::msg::Parameter param;
        param.name(name);
        param.value().type(rcl_interfaces::msg::ParameterType_Constants::PARAMETER_STRING);
        param.value().string_value(value);

        auto& request = mSetParameter.request_data();
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

        result = response.results().at(0).successful();
        ret_msg = response.results().at(0).reason();

        return RETCODE_OK;                    
    }

private:
    std::string mNodeName;

    
    MfriSrvClient<rcl_interfaces::srv::SetParameters_RequestPubSubType, 
                  rcl_interfaces::srv::SetParameters_Request, 
                  rcl_interfaces::srv::SetParameters_ResponsePubSubType, 
                  rcl_interfaces::srv::SetParameters_Response> mSetParameter;

};

    
} // namespace mfri
