#include <WiFi.h>
#include <PubSubClient.h>
#include <string.h>

#define ON 1
#define OFF 0
#define LEFT_LED GPIO_NUM_26
#define RIGHT_LED GPIO_NUM_27
#define WIFI_LED GPIO_NUM_32
#define MQTT_LED GPIO_NUM_33
#define ERROR_LED GPIO_NUM_25

const char* ssid = "LIB-7298699";
const char* password = "Nq6jypqgqGmj";
const char* mqtt_server = "turnthatlighton-inel4206.space";
const char* mqtt_topic = "pose";

char message[3];

WiFiClient espClient;
PubSubClient client(espClient);

/**********************************************************
* Does the initial setup for the whole program.
* Connects to WIFI and MQTT broker.
***********************************************************/

void setup() {

  gpio_set_direction(LEFT_LED, GPIO_MODE_OUTPUT);
  gpio_set_direction(RIGHT_LED, GPIO_MODE_OUTPUT);
  gpio_set_direction(WIFI_LED, GPIO_MODE_OUTPUT);
  gpio_set_direction(MQTT_LED, GPIO_MODE_OUTPUT);
  gpio_set_direction(ERROR_LED, GPIO_MODE_OUTPUT);

  gpio_set_level(LEFT_LED, OFF); //LEFT GREEN
  gpio_set_level(RIGHT_LED, OFF); // RIGHT GREEN
  gpio_set_level(MQTT_LED, OFF); // BLUE
  gpio_set_level(WIFI_LED, OFF); // GREEN
  gpio_set_level(ERROR_LED, OFF); //RED

  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi.");Serial.print(".");
    gpio_set_level(WIFI_LED, ON);
    delay(300);
    gpio_set_level(WIFI_LED, OFF);
  }
  Serial.println("Connected to WiFi");
  gpio_set_level(WIFI_LED, ON);
  delay(5000);

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  while (!client.connected()) {
    Serial.println("Connecting to MQTT broker...");
    if (client.connect("ESP32Client")) {
      gpio_set_level(WIFI_LED, OFF);
      gpio_set_level(ERROR_LED, OFF);
      gpio_set_level(MQTT_LED, ON);
      Serial.println("Connected to MQTT broker");
      client.subscribe(mqtt_topic);
    } else {
    
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println("retrying in 5 seconds");
      delay(5000);
    }
  }
}

/**********************************************************
* Constantly checks if client is connected to MQTT broker,
* if not it calls the reconnect fucntion. Also, it continously
* updates the payload being received from the client.
***********************************************************/

void loop() {
  if (!client.connected()) {
    gpio_set_level(WIFI_LED, OFF);
    gpio_set_level(MQTT_LED, OFF);
    gpio_set_level(ERROR_LED, ON);
    reconnect_MQTT();
  }
  gpio_set_level(ERROR_LED, OFF);
  gpio_set_level(MQTT_LED, ON);
  client.loop();
}

/**********************************************************
* Function that receives MQTT topic, payload from MQTT topic,
* and length of payload. It the converts this payload byte by
* byte into a string and also saves this string in a message
* variable. 
* 
* @param MQTT Topic, MQTT Payload, length of payload
***********************************************************/

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println();
  Serial.print("Message received on topic: ");
  Serial.println(topic);
  Serial.print("Payload: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    message[i] = (char)payload[i];
  }
  Serial.println();

  determine_light(message);
}

/**********************************************************
* Function that receives pose message from payload and based 
* on that pose turns on target light.
*
* @param MQTT message 
***********************************************************/

void determine_light(char *message){
  
  if(strcmp(message, "100") == 0){ //left on
    gpio_set_level(LEFT_LED, ON);
  }
  else if(strcmp(message, "101") == 0){ //left off
    gpio_set_level(LEFT_LED, OFF);
  }
  else if(strcmp(message, "200") == 0){ //right on
    gpio_set_level(RIGHT_LED, ON);
  }
  else if(strcmp(message, "201") == 0){ //right off
    gpio_set_level(RIGHT_LED, OFF);
  }
  else if(strcmp(message, "300") == 0){ //no pose on
    Serial.println("NO POSE.");
  }
  else if(strcmp(message, "301") == 0){ //no pose off
    Serial.println("NO POSE.");
  }

}

/**********************************************************
* Function that reconnects to MQTT broker if connection is
* lost at any time. 
***********************************************************/
void reconnect_MQTT() {
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println("Connected to MQTT.");
      client.subscribe(mqtt_topic);
    } else {
      Serial.print("Failed, error code = ");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds");
      delay(5000);
    }
  }
}