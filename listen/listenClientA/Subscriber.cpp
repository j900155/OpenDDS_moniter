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

#include "../recvData.h"

#define socketFile "/var/lib/dds-mointer/server.sock"

int ACE_TMAIN(int argc, char *argv[])
{
	struct sockaddr_un addr;
	char buffer[100];
	int ret;
	int client;
	struct recvData data;

	memset(&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, socketFile);

	client = socket(AF_UNIX, SOCK_STREAM, 0);
	ret = connect(client, (struct sockaddr *) &addr, sizeof(struct sockaddr_un));
	if(0 > ret)
	{
		std::cerr <<"create socket clinet  fail"<< std::endl;
		return -1;
	}

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
	int timeTmp, timeNow;
	long lenData = 0;
	std::string sData;
	long ID[100];
	int IDlen=0;
	std::string allID;
	int i;
	gettimeofday(&tv,NULL);
	timeTmp = 0;
	strcpy(data.topicName,topic_name.c_str());
	while(true)
	{
		error = dataReader->take_next_sample(message, info);

		if(error == DDS::RETCODE_OK)
		{
			if(info.valid_data)	
			{		
				gettimeofday(&tv,NULL);
				std::cout << "message ID " << message.ID;
				std::cout << "message time " << message.sendSec;
				std::cout << ";message data " << message.sendData << std::endl;
				sData = message.sendData._retn();
				lenData += (long)sData.length();
				//lenData += message.sendData.length();
				fp << "message_data," << message.sendData << ",message_time," << message.sendSec  << std::endl;	
				for(i=0;i<=IDlen;i++)
				{
					if(message.ID == ID[i])
					{
						break;
					}

				}
				
				if(i > IDlen)
				{
					std::cout << "IDlen " << IDlen << " i " << i << std::endl;
					ID[IDlen] = message.ID;
					IDlen+=1;
				}
				timeNow = tv.tv_sec;
				if((timeNow-timeTmp)>10)
				{
					for(i=0;i<IDlen;i++)
					{
						allID += std::to_string(ID[i])+";";
					}
					std::cout << "allID " <<allID << std::endl;
					strcpy(data.pubID, allID.c_str());
					data.sendSec = timeNow;
					data.sendUsec = tv.tv_usec;
					if(timeTmp ==0)
					{
						lenData = 10;
					}
					data.dataSize = lenData;
					std::cout << "data.pubID " << data.pubID << "data.sendSec " << data.sendSec << std::endl;
					send(client,&data,sizeof(data),0);
					allID = "";
					lenData = 0;
					timeTmp = timeNow;
				}

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

