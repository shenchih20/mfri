#pragma once

#include <mfri/idl/ParameterType.hpp>
#include <mfri/idl/ParameterValue.hpp>

namespace mfri
{

class MfriParamValue
{
public:
    MfriParamValue()
    {
        mValue.type(rcl_interfaces::msg::ParameterType_Constants::PARAMETER_NOT_SET);
    }

    explicit MfriParamValue(const bool bool_value)
    {
        mValue.type(rcl_interfaces::msg::ParameterType_Constants::PARAMETER_BOOL);
        mValue.bool_value(bool_value);
    }

    explicit MfriParamValue(const int int_value)
    {
        mValue.type(rcl_interfaces::msg::ParameterType_Constants::PARAMETER_INTEGER);
        mValue.integer_value(int_value);
    }

    explicit MfriParamValue(const int64_t int_value)
    {
        mValue.type(rcl_interfaces::msg::ParameterType_Constants::PARAMETER_INTEGER);
        mValue.integer_value(int_value);
    }
    explicit MfriParamValue(const float double_value)
    {
        mValue.type(rcl_interfaces::msg::ParameterType_Constants::PARAMETER_DOUBLE);
        mValue.double_value(double_value);
    }
    explicit MfriParamValue(const double double_value)
    {
        mValue.type(rcl_interfaces::msg::ParameterType_Constants::PARAMETER_DOUBLE);
        mValue.double_value(double_value);
    }
    explicit MfriParamValue(const std::string &string_value)
    {
        mValue.type(rcl_interfaces::msg::ParameterType_Constants::PARAMETER_STRING);
        mValue.string_value(string_value);
    }
    explicit MfriParamValue(const char *string_value)
    {
        mValue.type(rcl_interfaces::msg::ParameterType_Constants::PARAMETER_STRING);
        mValue.string_value(std::string(string_value));
    }
    explicit MfriParamValue(const std::vector<uint8_t> &byte_array_value)
    {
        mValue.type(rcl_interfaces::msg::ParameterType_Constants::PARAMETER_BYTE_ARRAY);
        mValue.byte_array_value(byte_array_value);
    }
    explicit MfriParamValue(const std::vector<bool> &bool_array_value)
    {
        mValue.type(rcl_interfaces::msg::ParameterType_Constants::PARAMETER_BOOL_ARRAY);
        mValue.bool_array_value(bool_array_value);
    }
    explicit MfriParamValue(const std::vector<int> &int_array_value)
    {
        mValue.type(rcl_interfaces::msg::ParameterType_Constants::PARAMETER_INTEGER_ARRAY);
        mValue.integer_array_value().resize(int_array_value.size());
        for (size_t i = 0; i < int_array_value.size(); ++i)
        {
            mValue.integer_array_value()[i] = int_array_value[i];
        }
    }

    explicit MfriParamValue(const std::vector<int64_t> &int_array_value)
    {
        mValue.type(rcl_interfaces::msg::ParameterType_Constants::PARAMETER_INTEGER_ARRAY);
        mValue.integer_array_value(int_array_value);
    }

    explicit MfriParamValue(const std::vector<float> &double_array_value)
    {
        mValue.type(rcl_interfaces::msg::ParameterType_Constants::PARAMETER_DOUBLE_ARRAY);
        mValue.double_array_value().resize(double_array_value.size());
        for (size_t i = 0; i < double_array_value.size(); ++i)
        {
            mValue.double_array_value()[i] = double_array_value[i];
        }
        
    }

    explicit MfriParamValue(const std::vector<double> &double_array_value)
    {
        mValue.type(rcl_interfaces::msg::ParameterType_Constants::PARAMETER_DOUBLE_ARRAY);
        mValue.double_array_value(double_array_value);
    }

    explicit MfriParamValue(const std::vector<std::string> &string_array_value)
    {
        mValue.type(rcl_interfaces::msg::ParameterType_Constants::PARAMETER_STRING_ARRAY);
        mValue.string_array_value(string_array_value);
    }

    rcl_interfaces::msg::ParameterValue & value()
    {
        return mValue;
    }

private:
    rcl_interfaces::msg::ParameterValue mValue;
};

} // namespace mfri
