#include "Led.h"
#include <Arduino.h>

Led::Led(int rPin, int gPin, int bPin)
{
    _rPin = rPin;
    _gPin = gPin;
    _bPin = bPin;
}

void Led::setup()
{
    pinMode(_rPin, OUTPUT);
    pinMode(_gPin, OUTPUT);
    pinMode(_bPin, OUTPUT);
    digitalWrite(_rPin, HIGH);
    digitalWrite(_gPin, HIGH);
    digitalWrite(_bPin, HIGH);
}

void Led::lightIt(int led, int brightness)
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
