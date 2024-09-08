#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>


#define EchoPin 2
#define TrigPin 3

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = "Wifi name";    // your network SSID (name)
char pass[] = "wifi password";    // your network password (use for WPA, or use as key for WEP)

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "broker.emqx.io";
int        port     = 1883;
const char topic[]  = "Same name as in Receiver code";

long Duration;
int Distance;

int count = 0;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  
 pinMode(TrigPin, OUTPUT); 
 pinMode(EchoPin, INPUT); 
  
  while (!Serial); // waits till the Serial Monitor opens up

  // attempt to connect to WiFi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // retries till connected
    Serial.print(".");
    delay(2000);
  }

  Serial.println("You're connected to the network");
  Serial.println();

  // You can provide a unique client ID, if not set the library uses Arduino-millis()
  // Each client must have a unique client ID
  // mqttClient.setId("clientId");

  // You can provide a username and password for authentication
  // mqttClient.setUsernamePassword("username", "password");

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();
}

void loop() {
  // calling poll to send the keep alive message to keep the thing updated and running smoothly
  mqttClient.poll();

  // sends a pulse by turning the TrigPin on and off and then calculates the distance  
  digitalWrite(TrigPin, LOW);
  delay(2);
  digitalWrite(TrigPin, HIGH);
  delay(10);
  digitalWrite(TrigPin, LOW);
  Duration = pulseIn(EchoPin, HIGH);
  Distance = Duration * 0.034 / 2;
  Serial.print("Distance: ");  
  Serial.println(Distance);
  
  if (Distance <= 20) {
    // Prints the following on the Serial Monitor to keep us updated 
    Serial.print("Sending message to topic: ");
    Serial.println(topic);
    Serial.print("hello ");
    Serial.println(count);

    // sends the message by using the print method 
    mqttClient.beginMessage(topic);
    mqttClient.print("hello ");
    mqttClient.print(count);
    mqttClient.endMessage();

    Serial.println();

    count++;
  }
  delay(1000);
}