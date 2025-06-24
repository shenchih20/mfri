#pragma once

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>


using namespace eprosima::fastdds::dds;

namespace mfri
{

template <class Type_t, class Data_t>    
class MfriPublisherBase : public DataWriterListener
{
public:
    MfriPublisherBase(const std::string&topic_name, const std::string& data_type_name) : 
        mTopicName(topic_name),
        mDataTypeName(data_type_name),
        mDataType(new Type_t())
    {
        
    }

    ~MfriPublisherBase()
    {
        close();
    }        

    bool initialize(DomainParticipant* participant)
    {
        if (true == mReady)
        {
            return true;
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
       
        
        PublisherQos pub_qos;
        participant->get_default_publisher_qos(pub_qos);
        mPublisher = participant->create_publisher(pub_qos, nullptr, StatusMask::none());
        if (nullptr == mPublisher)
        {
            participant->delete_topic(mTopic);
            return false;
        }

        DataWriterQos writer_qos;
        mPublisher->get_default_datawriter_qos(writer_qos);
        mWriter = mPublisher->create_datawriter(mTopic, writer_qos, this);
        if (nullptr == mWriter)
        {
            participant->delete_publisher(mPublisher);
            participant->delete_topic(mTopic);
            return false;
        }

        mReady = true;
        return true;

    }

    bool is_ready() const
    {
        return mReady;
    }

    ReturnCode_t publish()
    {
        if (false == mReady)
        {
            return RETCODE_NOT_ENABLED;
        }

        return mWriter->write(&mData);        
    }

    void close()
    {
        if (false == mReady)
        {
            return;
        }

        mReady = false;

        if (nullptr != mWriter)
        {
            mPublisher->delete_datawriter(mWriter);
        }

        if (nullptr != mPublisher)
        {
            mParticipant->delete_publisher(mPublisher);
        }

        if (nullptr != mTopic)
        {
            mParticipant->delete_topic(mTopic);
        }
    }


    Data_t& Data()
    {
        return mData;
    }
     
protected:
    Data_t mData;

private:
    bool mReady = false;

    std::string mDataTypeName = "";
    TypeSupport mDataType;

    Topic* mTopic = nullptr;
    std::string mTopicName = "rt/";

    DomainParticipant* mParticipant = nullptr;
    Publisher* mPublisher = nullptr;
    
    DataWriter* mWriter = nullptr;
    
};



template <class Type_t, class Data_t>    
class MfriPublisher : public MfriPublisherBase<Type_t, Data_t>
{
public:
    MfriPublisher(const std::string& topic_name, const std::string& data_type_name) : 
        MfriPublisherBase<Type_t, Data_t>(std::string("rt/")+topic_name, data_type_name)
    {
        
    }
      
      
};

} // namespace mfri