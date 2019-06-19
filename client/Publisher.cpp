#include <ace/Log_Msg.h>

#include <dds/DdsDcpsInfrastructureC.h>
#include <dds/DdsDcpsPublicationC.h>

#include <dds/DCPS/Marked_Default_Qos.h>
#include <dds/DCPS/Service_Participant.h>
#include <dds/DCPS/WaitSet.h>

#include "dds/DCPS/StaticIncludes.h"

#include "MessengerTypeSupportImpl.h"
#include <iostream>
#include <string.h>

//time stamp
#ifdef linux
#include <sys/time.h>
#include <unistd.h>
#endif

char configFile[] = "./config.conf";
int readConf(char *topic, long *ID, int *delay,char *text)
{
	FILE *readFile;
	readFile = fopen(configFile, "r");
	if(readFile == NULL)
	{
		return -1;
	}
	fscanf(readFile, "%20s", topic);
	fscanf(readFile, "%ld", ID);
	fscanf(readFile, "%d", delay);
	fscanf(readFile, "%500s", text);
	//printf("topic %s ID %ld", topic, *ID);
	return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
	char topic[20];
	//char topic[] = "A";
	char *tmp;
	long ID,i;
	char text[500];
	ID = 123;
	int delay_us = 1000000;
	i = readConf(topic,&ID, &delay_us, text);
	//i=0;
	if (-1 ==i) 
	{
		printf("read config error\n");
		return -1;
    }
	
	std::cout <<"topic " << topic << " ID " << ID << std::endl;
  try {
    // Initialize DomainParticipantFactory
    DDS::DomainParticipantFactory_var dpf =
      TheParticipantFactoryWithArgs(argc, argv);

    // Create DomainParticipant
    DDS::DomainParticipant_var participant =
      dpf->create_participant(43,
                              PARTICIPANT_QOS_DEFAULT,
                              0,
                              OpenDDS::DCPS::DEFAULT_STATUS_MASK);

    if (!participant) {
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT("ERROR: %N:%l: main() -")
                        ACE_TEXT(" create_participant failed!\n")),
                       -1);
    }

    // Register TypeSupport (Messenger::Message)
    Messenger::MessageTypeSupport_var ts =
      new Messenger::MessageTypeSupportImpl;

    if (ts->register_type(participant, "") != DDS::RETCODE_OK) {
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT("ERROR: %N:%l: main() -")
                        ACE_TEXT(" register_type failed!\n")),
                       -1);
    }


    // Create Topic (Movie Discussion List)
	i = strlen(topic);
	//i +=1;
	tmp =(char*) malloc(i*sizeof(char));
	strncpy(tmp,topic,i);
	//tmp[i] = '\0';
    CORBA::String_var type_name = ts->get_type_name();
    DDS::Topic_var topic =
      participant->create_topic(tmp,
                                type_name,
                                TOPIC_QOS_DEFAULT,
                                0,
                                OpenDDS::DCPS::DEFAULT_STATUS_MASK);

    if (!topic) {
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT("ERROR: %N:%l: main() -")
                        ACE_TEXT(" create_topic failed!\n")),
                       -1);
    }
	free(tmp);

    // Create Publisher
    DDS::Publisher_var publisher =
      participant->create_publisher(PUBLISHER_QOS_DEFAULT,
                                    0,
                                    OpenDDS::DCPS::DEFAULT_STATUS_MASK);

    if (!publisher) {
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT("ERROR: %N:%l: main() -")
                        ACE_TEXT(" create_publisher failed!\n")),
                       -1);
    }
	
    DDS::DataWriter_var writer =
      publisher->create_datawriter(topic,
                                   DATAWRITER_QOS_DEFAULT,
                                   0,
                                   OpenDDS::DCPS::DEFAULT_STATUS_MASK);

    if (!writer) {
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT("ERROR: %N:%l: main() -")
                        ACE_TEXT(" create_datawriter failed!\n")),
                       -1);
    }

    Messenger::MessageDataWriter_var message_writer =
      Messenger::MessageDataWriter::_narrow(writer);

    if (!message_writer) {
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT("ERROR: %N:%l: main() -")
                        ACE_TEXT(" _narrow failed!\n")),
                       -1);
    }


	//time
	struct timeval tv;
	gettimeofday(&tv,NULL);
    // Write samples
    Messenger::Message message;
	DDS::ReturnCode_t error;
	//wait for subscriber
	while(true)
	{
		DDS::PublicationMatchedStatus matches;
		if(writer->get_publication_matched_status(matches) != DDS::RETCODE_OK)
		{
			 ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT("ERROR: %N:%l: main() -")
                        ACE_TEXT(" get_publication_matched_status faild!\n")),
                       -1);
	
		}
		if(matches.current_count >=1)
		{
			break;
		}
	}
	long t;
	gettimeofday(&tv,NULL);
	t = tv.tv_sec;
	while(true)
	{
		if((tv.tv_sec - t) >= 10)
		{
			break;
		}
		const char *cc;
		cc = text;	
		message.sendData =cc;
	    //message.sendTime  = 0;
		gettimeofday(&tv,NULL);
		message.sendSec = tv.tv_sec;
		message.sendUsec = tv.tv_usec;
		message.ID = ID;
	    error = message_writer->write(message, DDS::HANDLE_NIL);
	      if (error != DDS::RETCODE_OK)
		  {
		    ACE_ERROR((LM_ERROR,
            ACE_TEXT("ERROR: %N:%l: main() -"),
            ACE_TEXT(" write returned %d!\n"), error));
		}
		std::cout << "ok "  << std::endl;
		usleep(delay_us);
	}
	message.sendData = "end";
	error = message_writer->write(message, DDS::HANDLE_NIL);
    // Wait for samples to be acknowledged
	std::cout << "end" << std::endl;	
    DDS::Duration_t timeout = { 30, 0 };
    if (message_writer->wait_for_acknowledgments(timeout) != DDS::RETCODE_OK) {
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT("ERROR: %N:%l: main() -")
                        ACE_TEXT(" wait_for_acknowledgments failed!\n")),
                       -1);
    }
	std::cout << "clean" << std::endl;
    // Clean-up!
    participant->delete_contained_entities();
    dpf->delete_participant(participant);

    TheServiceParticipant->shutdown();

  } catch (const CORBA::Exception& e) {
    e._tao_print_exception("Exception caught in main():");
    return -1;
  }

  return 0;
}
