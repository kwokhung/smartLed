#ifndef App_h
#define App_h

#define LED_ONBOARD_PIN 2 // GPIO2 / E4 / D9

#define LED_R_PIN 5 // GPIO5 / E1 / D3
#define LED_G_PIN 4 // GPIO4 / E2 / D4
#define LED_B_Pin 0 // GPIO0 / E3 / D8

#define SERIAL_RX0_PIN 3 // GPIO3 / RX / D0
#define SERIAL_TX0_PIN 1 // GPIO1 / TX / D1

#define OLed_ADDRESS 0x3c
#define OLed_SDA_PIN 12 // GPIO12 / E6 / D6
#define OLed_SCL_PIN 14 // GPIO14 / E5 / D5

class App
{
  public:
    App();
    
    void setup();
};

#endif