#include "Led.h"

void setupLed()
{
    pinMode(RLED, OUTPUT);
    pinMode(GLED, OUTPUT);
    pinMode(BLED, OUTPUT);
    digitalWrite(RLED, HIGH);
    digitalWrite(GLED, HIGH);
    digitalWrite(BLED, HIGH);
}

void lightIt(int led, int brightness)
{
    Serial.print('*');
    Serial.print(brightness);

    if (brightness > 0)
    {
        analogWrite(led, 1023 - brightness);
    }
    else if (brightness == 0)
    {
        analogWrite(led, 1023);
        digitalWrite(led, HIGH);
    }
}