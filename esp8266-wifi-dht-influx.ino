#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <DHT.h>

const char* ssid = "iPhone";
const char* password = "crown123"; // should be empty for guest network

String device_id = "esp-demo";
float h,t;

#define DHTPIN D4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  pinMode(DHTPIN, INPUT_PULLUP);

  Serial.println();
  Serial.printf("Connecting to %s\n", ssid);
  Serial.flush();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }
  Serial.println("Connected");
  dht.begin();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    h = dht.readHumidity();
    t = dht.readTemperature();
    Serial.println(t);
    sendMeasurement();
  }
  delay(20000);
}

void sendMeasurement() {
  HTTPClient client;
  client.begin("http://sb.ixa.pl:8086/write?db=sensors");
  String params;
  params = "temperature,node="+device_id+",location=x,unit=Celsius value="+t+"\n";
  params += "humidity,node="+device_id+",location=x,unit=Percent value="+h;
  int httpCode = client.POST(params);
  if (httpCode != 204) {
    Serial.printf("Request failed, error: %d\n", httpCode);
  }
  Serial.flush();
  client.end();
}

