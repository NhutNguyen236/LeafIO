// LIBS INCLUDE
#include <ESP8266WiFi.h>
#include <DHT.h>

// PIN DEFINITIONS
#define DHTPIN D2
#define MOISTUREPIN A0
#define FANPIN D8
#define LIGHTPIN D6
#define WATERPUMP D7
#define LEDFAN D8    // red led
#define LEDWATER D7  // yellow led
#define GARDENLED D1 // green led

// OTHER DEFINITIONS
#define DHTTYPE DHT11

// VARIABLES DECLARATION
DHT dht(DHTPIN, DHTTYPE);

const char *ssid = "Cormac";
const char *password = "+Ah(nstP7.U7+qz";
const char *host = "192.168.100.17";

// soil moisture value define
int moisture_value = 0, moisture_state = 0xFF;

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
// this will return the value in degree Celcius
float getTemp()
{
    float temp = dht.readTemperature();
    if (isnan(temp))
    {
        return -1;
    }
    return temp;
}

// get soil moisture value from sensor
// this will return the value in percentage
float getSoil()
{
    moisture_value = analogRead(MOISTUREPIN);
    // convert analog value to percentage
    moisture_value = map(moisture_value, 550, 0, 0, 100);

    if (isnan(moisture_value))
    {
        return -1;
    }
    return moisture_value;
}

// get light value from light sensor
int getLight()
{
    int light_value = analogRead(LIGHTPIN);
    // convert analog value to percentage
    light_value = map(light_value, 550, 10, 0, 100);

    if(isnan(light_value))
    {
        return -1;
    }
    return light_value;
}

void setup()
{
    Serial.begin(9600);

    // configure input pin for sensors
    pinMode(DHTPIN, INPUT);
    pinMode(MOISTUREPIN, INPUT);
    pinMode(LIGHTPIN, INPUT);

    // configure output pin for actuators
    pinMode(FANPIN, OUTPUT);
    pinMode(WATERPUMP, OUTPUT);
    pinMode(LEDFAN, OUTPUT);
    pinMode(LEDWATER, OUTPUT);
    pinMode(GARDENLED, OUTPUT);

    delay(10);

    // connect to wifi
    wifiConnect();
}

void loop()
{
    delay(2000);

    // read temperature data from DHT11 sensor
    float temperature = getTemp();
    // read soil moisture data from sensor
    float soil_moisture = getSoil();
    // read light data from sensor
    int light_value = getLight();

    // check if there is any error while reading data from DHT11 or not
    // if there is any error, then print error message
    if (temperature == -1 || soil_moisture == -1  || light_value == -1)
    {
        Serial.println("Failed to read from DHT sensor or soil moisture sensor or there is a broken light sensor!");
    }
    // else then serialize the temperature and moisture then activate actuators
    else
    {
        Serial.printf("Nhiệt độ hiện tại là: %f \n", temperature);
        Serial.printf("Độ ẩm đất hiện tại là : %f \n", soil_moisture);
        Serial.printf("Độ sáng trong vườn ươm là: %d \n", light_value);

        // if current temperature is higher or euqal to 27 degree then turn on fan
        if (temperature >= 27)
        {
            digitalWrite(FANPIN, HIGH);
            // turn on led to indicate fan is on
            digitalWrite(LEDFAN, HIGH);
        }
        else
        {
            digitalWrite(FANPIN, LOW);
            // turn off led to indicate fan is off
            digitalWrite(LEDFAN, LOW);
        }

        // if current soil moisture is less or equal to 30% then turn on water pump
        if (soil_moisture <= 30)
        {
            digitalWrite(WATERPUMP, HIGH);
            // turn on led to indicate water pump is on
            digitalWrite(LEDWATER, HIGH);
        }
        else
        {
            digitalWrite(WATERPUMP, LOW);
            // turn off led to indicate water pump is off
            digitalWrite(LEDWATER, LOW);
        }

        // if the garden is lack of light then turn on the led to indicate the garden is lack of light
        if (light_value <= 30)
        {
            digitalWrite(GARDENLED, HIGH);
        }
        else
        {
            digitalWrite(GARDENLED, LOW);
        }

        // Use WiFiClient class to create TCP connections
        WiFiClient client;
        const int httpPort = 8000;
        if (!client.connect(host, httpPort))
        {
            Serial.println("connection failed");
            return;
        }

        // We now create a URI for the request
        String url = "/";

        Serial.print("Requesting URL: ");
        Serial.println(url);

        // 1st param is temp, 2nd is moisture
        String data = String(temperature) + "," + String(soil_moisture);

        Serial.print("Requesting POST: ");
        // Send request to the server:
        client.println("POST / HTTP/1.1");
        client.println("Host: server_name");
        client.println("Accept: */*");
        client.println("Content-Type: application/x-www-form-urlencoded");
        client.print("Content-Length: ");
        client.println(data.length());
        client.println();
        client.print(data);
        // This will send the request to the server
        /*this is a get method working
         * client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                  "Connection: close\r\n\r\n");*/
        unsigned long timeout = millis();
        while (client.available() == 0)
        {
            if (millis() - timeout > 5000)
            {
                Serial.println(">>> Client Timeout !");
                client.stop();
                return;
            }
        }

        // Read all the lines of the reply from server and print them to Serial
        while (client.available())
        {
            String line = client.readStringUntil('\r');
            Serial.print(line);
        }

        Serial.println();
        Serial.println("closing connection");
    }
}
