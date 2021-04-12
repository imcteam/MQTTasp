/*******************************************************************************
 * 调用paho c mqtt 库
 * 用于与TTCN-3 MQTTasp demo 交互测试
 * 接受来自TTCN-3的数据，循环发送输入信息
 *******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "MQTTClient.h"

#define ADDRESS     "tcp://0.0.0.0:1883"
//#define ADDRESS     "tcp://broker.hivemq.com:1883"
#define CLIENTID    "ExampleClientPub2"
#define TOPIC       "TTCN1"
#define TOPIC2       "TTCN2"
#define QOS         1
#define TIMEOUT     10000L

volatile MQTTClient_deliveryToken deliveredtoken;

void delivered(void *context, MQTTClient_deliveryToken dt)
{
    //printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    //printf("Message arrived\n");
    //printf("     topic: %s\n", topicName);
    printf("TTCN-3: %.*s\n", message->payloadlen, (char*)message->payload);
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

void connlost(void *context, char *cause)
{
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}


int main(int argc, char* argv[])
{
    char PAYLOAD[256];
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    int rc;

    if ((rc = MQTTClient_create(&client, ADDRESS, CLIENTID,
        MQTTCLIENT_PERSISTENCE_NONE, NULL)) != MQTTCLIENT_SUCCESS)
    {
         printf("Failed to create client, return code %d\n", rc);
         exit(EXIT_FAILURE);
    }

    if ((rc = MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to set callbacks, return code %d\n", rc);
        rc = EXIT_FAILURE;
    }

    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    if ((rc = MQTTClient_subscribe(client, TOPIC2, QOS)) != MQTTCLIENT_SUCCESS)
    {
    	printf("Failed to subscribe, return code %d\n", rc);
    	rc = EXIT_FAILURE;
    }


    // for(int i=0;i<20;i++){
    // sprintf(PAYLOAD, "{\"hello world:\":%d}", i);
    // pubmsg.payload = PAYLOAD;
    // pubmsg.payloadlen = (int)strlen(PAYLOAD);
    // pubmsg.qos = QOS;
    // pubmsg.retained = 0;
    // if ((rc = MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token)) != MQTTCLIENT_SUCCESS)
    // {
    //      printf("Failed to publish message, return code %d\n", rc);
    //      exit(EXIT_FAILURE);
    // }
    // sleep(1);
    // memset(PAYLOAD,0,sizeof(PAYLOAD));
    // }

    memset(PAYLOAD,0,sizeof(PAYLOAD));
    while(scanf("%s",PAYLOAD)){
        pubmsg.payload = PAYLOAD;
        pubmsg.payloadlen = (int)strlen(PAYLOAD);
        pubmsg.qos = QOS;
        pubmsg.retained = 0;
        if ((rc = MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token)) != MQTTCLIENT_SUCCESS)
        {
            printf("Failed to publish message, return code %d\n", rc);
            exit(EXIT_FAILURE);
        }
	    memset(PAYLOAD,0,sizeof(PAYLOAD));
    }

    if ((rc = MQTTClient_disconnect(client, 10000)) != MQTTCLIENT_SUCCESS)
    	printf("Failed to disconnect, return code %d\n", rc);
    MQTTClient_destroy(&client);
    return rc;
}