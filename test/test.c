/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2019-12-11 21:53:07
 * @LastEditTime: 2020-02-26 22:43:54
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "mqttclient.h"

mqtt_client_t client;
client_init_params_t init_params;

static void topic_test1_handler(void* client, message_data_t* msg)
{
    (void) client;
    LOG_I("-----------------------------------------------------------------------------------");
    LOG_I("%s:%d %s()...\ntopic: %s\nmessage:%s", __FILE__, __LINE__, __FUNCTION__, msg->topic_name, (char*)msg->message->payload);
    LOG_I("-----------------------------------------------------------------------------------");
}

int main(void)
{
    int err;
    char buf[100] = { 0 };
    mqtt_message_t msg;
    memset(&msg, 0, sizeof(msg));

    msg.qos = QOS0;
    msg.payload = (void *) buf;

    log_init();

    rt_thread_delay(6000);
    
    init_params.read_buf_size = 1024;
    init_params.write_buf_size = 1024;
#ifdef KAWAII_MQTT_NETWORK_TYPE_TLS
    extern const char *test_ca_get();
    init_params.connect_params.network_params.network_ssl_params.ca_crt = test_ca_get();
    init_params.connect_params.network_params.port = "8883";
#else
    init_params.connect_params.network_params.port = "1883";
#endif
    init_params.connect_params.network_params.addr = "www.jiejie01.top"; //"47.95.164.112";//"jiejie01.top"; //"129.204.201.235"; //"192.168.1.101";

    init_params.connect_params.user_name = random_string(10); // random_string(10); //"jiejietop-acer1";
    init_params.connect_params.password = random_string(10);; //random_string(10); // "123456";
    init_params.connect_params.client_id = random_string(10);; //random_string(10); // "clientid-acer1";
    init_params.connect_params.clean_session = 1;

    mqtt_init(&client, &init_params);

    err = mqtt_connect(&client);
    
    LOG_I("mqtt connect return %d", err);
    
    err = mqtt_subscribe(&client, "rtt-topic1", QOS0, topic_test1_handler);
    err += mqtt_subscribe(&client, "rtt-topic2", QOS0, NULL);
    
    LOG_I("mqtt subscribe return %d", err);
    
    while (1) {

        sprintf(buf, "welcome to mqttclient, this is a publish test, a rand number: %d ...", random_number());
        mqtt_publish(&client, "rtt-topic1", &msg);
        rt_thread_mdelay(1000);
        
        sprintf(buf, "welcome to mqttclient, this is a publish test, a rand number: %d ...", random_number());
        mqtt_publish(&client, "rtt-topic2", &msg);
        rt_thread_mdelay(1000);
    }
}

