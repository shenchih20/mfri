// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*!
 * @file AddTwoInts_Request.hpp
 * This header file contains the declaration of the described types in the IDL file.
 *
 * This file was generated by the tool fastddsgen.
 */

#ifndef FAST_DDS_GENERATED__EXAMPLE_INTERFACES_SRV_ADDTWOINTS_REQUEST_HPP
#define FAST_DDS_GENERATED__EXAMPLE_INTERFACES_SRV_ADDTWOINTS_REQUEST_HPP

#include <cstdint>
#include <utility>

#if defined(_WIN32)
#if defined(EPROSIMA_USER_DLL_EXPORT)
#define eProsima_user_DllExport __declspec( dllexport )
#else
#define eProsima_user_DllExport
#endif  // EPROSIMA_USER_DLL_EXPORT
#else
#define eProsima_user_DllExport
#endif  // _WIN32

#if defined(_WIN32)
#if defined(EPROSIMA_USER_DLL_EXPORT)
#if defined(ADDTWOINTS_REQUEST_SOURCE)
#define ADDTWOINTS_REQUEST_DllAPI __declspec( dllexport )
#else
#define ADDTWOINTS_REQUEST_DllAPI __declspec( dllimport )
#endif // ADDTWOINTS_REQUEST_SOURCE
#else
#define ADDTWOINTS_REQUEST_DllAPI
#endif  // EPROSIMA_USER_DLL_EXPORT
#else
#define ADDTWOINTS_REQUEST_DllAPI
#endif // _WIN32

namespace example_interfaces {

namespace srv {

/*!
 * @brief This class represents the structure AddTwoInts_Request defined by the user in the IDL file.
 * @ingroup AddTwoInts_Request
 */
class AddTwoInts_Request
{
public:

    /*!
     * @brief Default constructor.
     */
    eProsima_user_DllExport AddTwoInts_Request()
    {
    }

    /*!
     * @brief Default destructor.
     */
    eProsima_user_DllExport ~AddTwoInts_Request()
    {
    }

    /*!
     * @brief Copy constructor.
     * @param x Reference to the object AddTwoInts_Request that will be copied.
     */
    eProsima_user_DllExport AddTwoInts_Request(
            const AddTwoInts_Request& x)
    {
                    m_a = x.m_a;

                    m_b = x.m_b;

    }

    /*!
     * @brief Move constructor.
     * @param x Reference to the object AddTwoInts_Request that will be copied.
     */
    eProsima_user_DllExport AddTwoInts_Request(
            AddTwoInts_Request&& x) noexcept
    {
        m_a = x.m_a;
        m_b = x.m_b;
    }

    /*!
     * @brief Copy assignment.
     * @param x Reference to the object AddTwoInts_Request that will be copied.
     */
    eProsima_user_DllExport AddTwoInts_Request& operator =(
            const AddTwoInts_Request& x)
    {

                    m_a = x.m_a;

                    m_b = x.m_b;

        return *this;
    }

    /*!
     * @brief Move assignment.
     * @param x Reference to the object AddTwoInts_Request that will be copied.
     */
    eProsima_user_DllExport AddTwoInts_Request& operator =(
            AddTwoInts_Request&& x) noexcept
    {

        m_a = x.m_a;
        m_b = x.m_b;
        return *this;
    }

    /*!
     * @brief Comparison operator.
     * @param x AddTwoInts_Request object to compare.
     */
    eProsima_user_DllExport bool operator ==(
            const AddTwoInts_Request& x) const
    {
        return (m_a == x.m_a &&
           m_b == x.m_b);
    }

    /*!
     * @brief Comparison operator.
     * @param x AddTwoInts_Request object to compare.
     */
    eProsima_user_DllExport bool operator !=(
            const AddTwoInts_Request& x) const
    {
        return !(*this == x);
    }

    /*!
     * @brief This function sets a value in member a
     * @param _a New value for member a
     */
    eProsima_user_DllExport void a(
            int64_t _a)
    {
        m_a = _a;
    }

    /*!
     * @brief This function returns the value of member a
     * @return Value of member a
     */
    eProsima_user_DllExport int64_t a() const
    {
        return m_a;
    }

    /*!
     * @brief This function returns a reference to member a
     * @return Reference to member a
     */
    eProsima_user_DllExport int64_t& a()
    {
        return m_a;
    }


    /*!
     * @brief This function sets a value in member b
     * @param _b New value for member b
     */
    eProsima_user_DllExport void b(
            int64_t _b)
    {
        m_b = _b;
    }

    /*!
     * @brief This function returns the value of member b
     * @return Value of member b
     */
    eProsima_user_DllExport int64_t b() const
    {
        return m_b;
    }

    /*!
     * @brief This function returns a reference to member b
     * @return Reference to member b
     */
    eProsima_user_DllExport int64_t& b()
    {
        return m_b;
    }



private:

    int64_t m_a{0};
    int64_t m_b{0};

};

} // namespace srv

} // namespace example_interfaces

#endif // _FAST_DDS_GENERATED_EXAMPLE_INTERFACES_SRV_ADDTWOINTS_REQUEST_HPP_


