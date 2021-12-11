// check board
#if !defined(ESP8266)
#error This code is intended to run only on the ESP8266 boards ! Please check your Tools->Board setting.
#endif

// LIBS INCLUDE
#include <ESP8266WiFi.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// PIN DEFINITIONS
// #define SCL_LCD D1
// #define SDA_LCD D2
#define DHTPIN D3
#define MOISTUREPIN A0
#define LIGHTPIN D6
#define FANPIN D8
#define WATERPUMP D7
#define LEDFAN D8    // red led
#define LEDWATER D7  // yellow led
#define GARDENLED D4 // green led

// OTHER DEFINITIONS
#define DHTTYPE DHT11

// VARIABLES DECLARATION
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

const char *ssid = "Cormac";
const char *password = "+Ah(nstP7.U7+qz";
const char *host = "192.168.100.17";
const int httpPort = 8000;

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

    if (isnan(light_value))
    {
        return -1;
    }
    return light_value;
}

// client communication function
void clientComm(float temperature, float soil_moisture, int light)
{
    // Initialize WiFiClient class to create TCP connections
    WiFiClient client;

    if (!client.connect(host, httpPort))
    {
        Serial.println("Connection failed");
        return;
    }

    // We now create a URI for the request
    String url = "/";

    Serial.print("Requesting URL: ");
    Serial.println(url);

    // 1st param is temp, 2nd is moisture
    String temp_data = "temperature=" + String(temperature) + "&" + "soil=" + String(soil_moisture) + "&" + "light=" + String(light);
    
    Serial.print("Requesting POST: ");
    // Send request to the server:
    client.println("POST / HTTP/1.1");
    client.println("Host: server_name");
    client.println("Accept: */*");
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.println((temp_data).length());
    client.println();
    client.println(temp_data);

    // This will send the request to the server
    /*this is a get method working*/
    // this GET req is for printing on dashboard
    // client.print(String("GET ") + url + " HTTP/1.1\r\n" + temp_data + 
    //           "Connection: close\r\n\r\n");
    
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

void printLCD(float temperature, float soil_moisture, int light)
{
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temperature);
    lcd.print("C");
    lcd.setCursor(0, 1);
    lcd.print("Soil: ");
    lcd.print(soil_moisture);
    lcd.print("%");
    lcd.setCursor(0, 2);
    lcd.print("Light: ");
    lcd.print(light);
    lcd.print("%");
}

// function to set pinMode to all components
void setPin()
{
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
}

void setup()
{
    Serial.begin(9600);

    // set pin mode
    setPin();
    
    // setup for LCD
    lcd.init();      // initialize the lcd
    lcd.backlight(); // Turn on backlight

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

    // display on LCD
    // clear LCD when update
    lcd.clear();
    printLCD(temperature, soil_moisture, light_value);

    // check if there is any error while reading data from DHT11 or not
    // if there is any error, then print error message
    if (temperature == -1 || soil_moisture == -1 || light_value == -1)
    {
        Serial.println("Failed to read from DHT sensor or soil moisture sensor or there is a broken light sensor!");
        return; 
    }
    // else then serialize the temperature and moisture then activate actuators
    else
    {
        Serial.printf("Nhiệt độ hiện tại là: %f \n", temperature);
        Serial.printf("Độ ẩm đất hiện tại là : %f \n", soil_moisture);
        Serial.printf("Độ sáng trong vườn ươm là: %d \n", light_value);

        // if current temperature is higher or euqal to 27 degree then turn on fan
        if (temperature >= 28)
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
        if (soil_moisture <= 0)
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

        // send all the data to node server
        clientComm(temperature, soil_moisture, light_value);
    }
}
