/**
 * @file       TinyGsmClient.h
 * @author     Volodymyr Shymanskyy
 * @license    LGPL-3.0
 * @copyright  Copyright (c) 2016 Volodymyr Shymanskyy
 * @date       Nov 2016
 */

#ifndef TinyGsmClient_h
#define TinyGsmClient_h

#if defined(TINY_GSM_MODEM_A6) || defined(TINY_GSM_MODEM_A7)
#include "TinyGsmClientA6.h"
typedef TinyGsm::GsmClient TinyGsmClient;

#elif defined(TINY_GSM_MODEM_ESP8266)
#include "TinyGsmClientESP8266.h"
typedef TinyGsm::GsmClient TinyGsmClient;

#else
#error "Please define GSM modem model"
#endif

#endif
