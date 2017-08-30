#ifndef App_h
#define App_h

#define RLED 5 // GPIO5 / E1 / D3
#define GLED 4 // GPIO4 / E2 / D4
#define BLED 0 // GPIO0 / E3 / D8

#define RX0 3 // GPIO3 / RX / D0
#define TX0 1 // GPIO1 / TX / D1

#define SCL 14 // GPIO14 / E5 / D5
#define SDA 12 // GPIO12 / E6 / D6

#define OnboardLED 2 // GPIO2 / E4 / D9

class App
{
  public:
    App();
    void setup();
};

#endif