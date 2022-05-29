#ifndef RF24_NODEMCU_RADIO_HEADER
#define RF24_NODEMCU_RADIO_HEADER

// FOR RADIO FUNCTIONALITIES
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "../HTTP_Utilities.h"


class NodeMCURadioRF24
{
    private:
        RF24 _radio;
        const byte _writingPipe[6];
        const byte _readingPipe[6];

    public:
        NodeMCURadioRF24(uint16_t cepin, uint16_t cspin)
            : _radio{cepin, cspin}, _writingPipe{"00001"}, _readingPipe{"00002"}
        {

        }

        void begin()
        {
            _radio.begin();
        }

        void writeMode()
        {
            //  TO WRITE
            _radio.openWritingPipe(_writingPipe);
            _radio.setPALevel(RF24_PA_MAX);
            _radio.stopListening();
        }

        void readMode()
        {
            //  TO READ
            _radio.openReadingPipe(0, _readingPipe);
            _radio.setPALevel(RF24_PA_MAX);
            _radio.startListening();
        }

        bool available() 
        {
            return _radio.available();
        }

        void writeMessage(const String& msg)
        {
            writeMode();
            
            char text[msg.length()];
            msg.toCharArray(text, msg.length());

            debug(_radio.write(&text, sizeof(text))?"radio write success":"radio write not success");
            // TODO: see _radio.getMaxPayloadSize()
        }


        // void callback(bool available, char msg[])
        template<typename T>
        void readMessage(T callback)
        {
            readMode();
            
            if(available()){
                // 32 is maximum for dynamicPayload size
                // TODO: See the _radio.setPayloadSize()
                // FIXME:Determine the max payload size
                char text[32] = "";
                _radio.read(text, sizeof(text));

                callback(true, text);
            }
            callback(false, "");
        }

        template<typename T>
        void writeMessageListen(const String& msg, T callback, int retries = 3)
        {
            writeMessage(msg);
        }

        String readStringUntil(char terminator)
        {
            String message;
            char in[2];
            while(_radio.available()){
                _radio.read(in, 1);
                if(in[0] == terminator){
                    break;
                }
                message + in[0];
            }
        }

};


#endif