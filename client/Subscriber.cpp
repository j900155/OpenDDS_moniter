#include <iostream>
#include <dds/DdsDcpsInfrastructureC.h>
#include <dds/DdsDcpsPublicationC.h>

#include <dds/DCPS/Marked_Default_Qos.h>
#include <dds/DCPS/Service_Participant.h>
#include <dds/DCPS/WaitSet.h>

#include "dds/DCPS/StaticIncludes.h"

#include "MessengerTypeSupportImpl.h"

#include <fstream>

#ifdef linux
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/un.h>
#endif


int ACE_TMAIN(int argc, char *argv[])
{

	DDS::DomainParticipantFactory_var dpf = TheParticipantFactoryWithArgs(argc, argv);
	DDS::DomainParticipant_var participant = dpf-> create_participant(43,
																	PARTICIPANT_QOS_DEFAULT,
																	0,
																	OpenDDS::DCPS::DEFAULT_STATUS_MASK);
	std::cout << "participant" << participant->get_domain_id() << std::endl;

	Messenger::MessageTypeSupport_var ts = new Messenger::MessageTypeSupportImpl;
	if (ts->register_type(participant, "") != DDS::RETCODE_OK)
	{
		std::cerr <<"create ts fail"<< std::endl;
	}
	CORBA::String_var type_name = ts->get_type_name();
	std::cout << "get_type_name" << type_name << std::endl;
	//topic
	
	std::string topic_name;
	std::cout << "topic name?" << std::endl;
	std::cin >> topic_name;
	std::cout << "topic name " << topic_name << std::endl;

	DDS::TopicQos topic_Qos;
	participant->get_default_topic_qos(topic_Qos);
	DDS::Topic_var topic = participant->create_topic(topic_name.c_str(),
										type_name,
										topic_Qos,
										0,
										OpenDDS::DCPS::DEFAULT_STATUS_MASK);
	if(!topic)
	{
		std::cerr <<"create topic fail"<< std::endl;
	}
	//topic end
	//subscriber
	DDS::SubscriberQos sub_qos;
	participant->get_default_subscriber_qos(sub_qos);

	DDS::Subscriber_var subscriber = 
			participant->create_subscriber(sub_qos,
				0,
				OpenDDS::DCPS::DEFAULT_STATUS_MASK);
	DDS::DataReaderQos readerQos;
	if(!subscriber)
	{
		std::cerr <<"create subcriber fail"<< std::endl;
	}
	else
	{
		subscriber->get_default_datareader_qos(readerQos);
	}
	//subscriber end
	//reader
	DDS::DataReader_var reader1 = subscriber->create_datareader(
			topic,
	readerQos,
			0,
			OpenDDS::DCPS::DEFAULT_STATUS_MASK);
	if(!reader1)
	{
		std::cerr <<"create reader fail"<< std::endl;
	}
	//reader end
	//datareader
	Messenger::MessageDataReader_var dataReader = Messenger::MessageDataReader::_narrow(reader1);
	if(!dataReader)
	{
		std::cerr <<"create dataReader fail"<< std::endl;
	}

	std::cout<< "topic name " << topic->get_name() << std::endl;

	DDS::ReturnCode_t error;
	DDS::SampleInfo info;
	Messenger::Message message;
	struct timeval tv;
	int delay_us = 100;
	fstream fp;
	std::string fileName;
	std::cout << "file name\n";
	std::cin >> fileName;
	fileName +=".txt";
	fp.open(fileName, std::ios::app);
	int timeNow;
	int timeDiff=0;
	long lenData = 0;
	std::string sData;
	long ID[100];
	int IDlen=0;
	std::string allID;
	int i,j;
	int point, newPoint;
	char *arrayKey[] = {"k1","k2","k3","k4","k5","k6"};
	int arrayKeyLen = 6;
	std::string jsonString = "{";
	gettimeofday(&tv,NULL);
	while(true)
	{
		error = dataReader->take_next_sample(message, info);

		if(error == DDS::RETCODE_OK)
		{
			if(info.valid_data)	
			{		
				std::cout << "message ID " << message.ID;
				std::cout << "message sec " << message.sendSec;
				std::cout << "message usec " << message.sendUsec;
				std::cout << ";message data " << message.sendData << std::endl;
				sData = message.sendData._retn();
				lenData = sData.length();
				point = 0;
				jsonString = "{";
				for(i=0;i<arrayKeyLen;i++)
				{
					jsonString += "\"";
					jsonString += arrayKey[i];
					jsonString += "\"";
					newPoint = sData.find(",", point);
					for(j=point;j<newPoint;j++)
					{
						jsonString +=sData[j];
					}
					if(i != (arrayKeyLen-1))
					{
						jsonString +=",";
					}
				}
				jsonString +="}";
				std::cout << jsonString << std::endl;
				gettimeofday(&tv,NULL);
				timeNow = tv.tv_sec;
				timeDiff = tv.tv_sec - message.sendSec;
				timeDiff = (timeDiff*1000000) + tv.tv_usec - message.sendUsec;

				fp << "message_ID," << message.ID << ",";	
				fp << "message_len," << lenData << ",message_sec," << message.sendSec;	
				fp << "message_usec," << message.sendUsec << ",message_diff," << timeDiff  << std::endl;	
			}
		}
		usleep(delay_us);

	}
    // Clean-up!
    participant->delete_contained_entities();
    dpf->delete_participant(participant);

    TheServiceParticipant->shutdown();

	return 0;
}

