#ifndef ALARM_INITIATE_RADIO_HEADER
#define ALARM_INITIATE_RADIO_HEADER

#include "radio_RF24.h"

static const char* ALARM_SERVER_KEY = "PZQR9";
const byte readingPipe[6] = "00001";
const byte writingPipe[6] = "00002";

const byte handshakeWritingingPipe[6] = "Xgdt2";
const byte handshakeReadingPipe[6] = "Ygdt2";
static RadioRF24 radio(2, 4, readingPipe, writingPipe, handshakeReadingPipe, handshakeWritingingPipe) ;

#endif