#ifndef Led_h
#define Led_h

class Led
{
  public:
    Led(int rPin, int gPin, int bPin);
    void setup();
    void lightIt(int led, int brightness);
    void lightItR(int brightness);
    void lightItG(int brightness);
    void lightItB(int brightness);
    
  private:
    int _rPin;
    int _gPin;
    int _bPin;
};

#endif