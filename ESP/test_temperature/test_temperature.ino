// LIBS INCLUDE
#include <ESP8266WiFi.h>
#include <DHT.h>

// PIN DEFINITIONS
#define DHTPIN D2
#define LEDPIN D1

// OTHER DEFINITIONS
#define DHTTYPE DHT11

//VARIABLES DECLARATION
DHT dht(DHTPIN, DHTTYPE);

const char *ssid = "Cormac";
const char *password = "+Ah(nstP7.U7+qz";
const char *host = "192.168.100.17";

// function to connect Wifi
void wifiConnect()
{
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(200);
        Serial.print(".");
    }
    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("");
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
    }
    else
    {
        Serial.println("\r\nWiFi not disconnected");
    }
}

// read temperature data from DHT11 sensor
float getTemp()
{
    float temp = dht.readTemperature();
    if (isnan(temp))
    {
        return -1;
    }
    return temp;
}

void setup()
{
    Serial.begin(9600);
    pinMode(LEDPIN,OUTPUT);
    delay(10);
    // connect to wifi
    wifiConnect();
}

void loop()
{
    delay(2000);

    float temperature = getTemp();

    // check if there is any error while reading data from DHT11 or not
    if (temperature == -1)
    {
        Serial.println("Failed to read from DHT sensor!");   
    }
    else
    {
        Serial.println(String(temperature));

        if(temperature >= 27){
            digitalWrite(LEDPIN, HIGH);
            delay(3000);
        }
        else{
            digitalWrite(LEDPIN, LOW);
        }
    }
}
