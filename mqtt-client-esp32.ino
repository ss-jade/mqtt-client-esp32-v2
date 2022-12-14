#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Haus2.4G";
const char* password = "7476Haus#CB2040$";
const char* mqttServer = "192.168.1.14";
const int mqttPort = 1883;
const char* mqttUser = "";
const char* mqttPassword = "";

WiFiClient espClient;
PubSubClient client(espClient);

void OnMqttReceived(char *topic, byte *payload, unsigned int length){
  Serial.print("Received on ");
  Serial.print(topic);
  Serial.print(": ");
  String content = "";
  for (size_t i = 0; i < length; i++){
    content.concat((char)payload[i]);
    }
  Serial.print(content);
  Serial.println();
  }

void setup(){ 
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("...................................");

  Serial.print("Connecting to WiFi.");
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
    }
  Serial.println("");  
  Serial.println("Connected to the WiFi network");
  //mqtt connection
  client.setServer(mqttServer, mqttPort);
while (!client.connected()){
  Serial.println("Connecting to MQTT...");
  if (client.connect("ESP32", mqttUser, mqttPassword )){
    Serial.println("connected");
    client.subscribe("data/esp32");
    client.setCallback(OnMqttReceived);
    }
  else{
    Serial.print("failed with state ");
    Serial.print(client.state());
    delay(2000);
       }
  }
}

void loop(){  
  client.loop();
  char str[16];
  sprintf(str, "%u", random(100));
  client.publish("data/esp32", str);
  Serial.println(str);
  delay(2000);
}
