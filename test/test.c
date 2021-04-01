
/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @LastEditTime: 2020-06-17 14:35:29
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */
#include <stdio.h>
#include <stdint.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "mqttclient.h"


static void sub_topic_handle1(void* client, message_data_t* msg)
{
    (void) client;
    KAWAII_MQTT_LOG_I("-----------------------------------------------------------------------------------");
    KAWAII_MQTT_LOG_I("%s:%d %s()...\ntopic: %s\nmessage:%s", __FILE__, __LINE__, __FUNCTION__, msg->topic_name, (char*)msg->message->payload);
    KAWAII_MQTT_LOG_I("-----------------------------------------------------------------------------------");
}


static int mqtt_publish_handle1(mqtt_client_t *client)
{
    mqtt_message_t msg;
    memset(&msg, 0, sizeof(msg));

    msg.qos = QOS0;
    msg.payload = (void *)"this is a kawaii mqtt test ...";

    return mqtt_publish(client, "rtt-pub", &msg);
}

static char cid[64] = { 0 };
static void kawaii_mqtt_demo(void *parameter)
{
    mqtt_client_t *client = NULL;
    
    rt_thread_delay(6000);
    
    mqtt_log_init();

    client = mqtt_lease();

    rt_snprintf(cid, sizeof(cid), "rtthread%d", rt_tick_get());

    mqtt_set_host(client, "jiejie01.top");
    mqtt_set_port(client, "1883");
    mqtt_set_user_name(client, "rt-thread");
    mqtt_set_password(client, "rt-thread");
    mqtt_set_client_id(client, cid);
    mqtt_set_clean_session(client, 1);

    KAWAII_MQTT_LOG_I("The ID of the Kawaii client is: %s ",cid);

    mqtt_connect(client);
    
    mqtt_subscribe(client, "rtt-sub", QOS0, sub_topic_handle1);
    
    while (1) {
        mqtt_publish_handle1(client);
                               
        mqtt_sleep_ms(4 * 1000);
    }
}

int ka_mqtt(void)
{
    rt_thread_t tid_mqtt;

    tid_mqtt = rt_thread_create("kawaii_demo", kawaii_mqtt_demo, RT_NULL, 2048, 17, 10);
    if (tid_mqtt == RT_NULL) {
        return -RT_ERROR;
    }

    rt_thread_startup(tid_mqtt);

    return RT_EOK;
}
MSH_CMD_EXPORT(ka_mqtt, Kawaii MQTT client test program);
