#include <ESP8266WiFi.h>
#include <DHT.h>
#define DHTPIN D2
#define DHTTYPE DHT11
#define  MOISTURE_THRESHOLD 55

DHT dht(DHTPIN, DHTTYPE);

const char *ssid = "Cormac";
const char *password = "+Ah(nstP7.U7+qz";

const char *host = "192.168.100.17";

// soil moisture pin define
int moisture_Pin= 0; // Soil Moisture Sensor input at Analog PIN A0
int moisture_value= 0, moisture_state = 0xFF;

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

// get soil moisture value from sensor
float getSoil(){
    moisture_value = analogRead(moisture_Pin);
    if(isnan(moisture_value)){
        return -1;
    }
    return moisture_value;
}

void setup()
{
    Serial.begin(9600);
    delay(10);
    // connect to wifi
    wifiConnect();
}

void loop()
{
    delay(2000);

    float temperature = getTemp();
    float soil_moisture = getSoil();

    // check if there is any error while reading data from DHT11 or not
    if (temperature != -1 || soil_moisture != -1)
    {
        Serial.printf("Nhiệt độ hiện tại là: %f \n", temperature);
        Serial.printf("Độ ẩm đất hiện tại là : %f \n", soil_moisture);

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
        String data = "temperature=" + String(temperature) + "soil_moisture=" + String(soil_moisture);

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
    else
    {
        Serial.println("Failed to read from DHT sensor!");
    }
}
