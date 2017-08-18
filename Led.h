#ifndef Led_h
#define Led_h

#define RLED 5       // GPIO5 / E1 / D3
#define GLED 4       // GPIO4 / E2 / D4
#define BLED 0       // GPIO0 / E3 / D8

class Led
{
  public:
    Led(int rPin, int gPin, int bPin);
    void setup();
    void lightIt(int led, int brightness);

  private:
    int _rPin;
    int _gPin;
    int _bPin;
};

#endif