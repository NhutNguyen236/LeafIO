// Welcome
// Electronics University

#define relayPin D4

int sensor_pin = A0; // Soil Sensor input at Analog PIN A0
int output_value;
void setup() // put your setup code here, to run once:
{
    Serial.begin(9600);
    pinMode(sensor_pin, INPUT);
    pinMode(relayPin, OUTPUT);
    Serial.println("Reading From the Sensor ...");
    delay(2000);
}

void loop()
{
    output_value = analogRead(sensor_pin);
    output_value = map(output_value, 550, 10, 0, 100);
    if (output_value <= 30)
    {
        digitalWrite(relayPin, HIGH);
    }
    else
    {
        digitalWrite(relayPin, LOW);
    }
    delay(1000);
}
