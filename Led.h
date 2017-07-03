#ifndef Led_h
#define Led_h

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