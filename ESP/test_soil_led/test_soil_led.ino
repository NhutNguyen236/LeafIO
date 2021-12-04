// Here, we are testing soil moisture sensor.
// The sensor is connected to pin D3
// The relay and its water pump are connected to pin D4 and D5 respectively.
// The relay is on when the soil moisture is less than the threshold.
#define MOISTUREPIN A0
#define LED D4

int output_value = 0;

void setup()
{
    Serial.begin(9600);
    pinMode(MOISTUREPIN, INPUT);
    pinMode(LED, OUTPUT);
    delay(2000);
}

void loop()
{
    output_value = analogRead(MOISTUREPIN);
    output_value = map(output_value, 550, 10, 0, 100);
    Serial.print("Soil moisture: ");
    Serial.println(output_value);
    if (output_value < 20)
    {
        digitalWrite(LED, HIGH);
    }
    else
    {
        digitalWrite(LED, LOW);
    }
    delay(1000);
}
