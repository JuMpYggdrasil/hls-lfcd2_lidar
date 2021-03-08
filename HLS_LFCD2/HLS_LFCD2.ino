#include "HLS_LFCD2.h"

HLS_LFCD2 lidarSensor;
uint16_t angleCnt = 0;
bool dispFlag = false;

void setup()
{
    Serial.begin(230400);
    Serial1.begin(230400);
    Serial2.begin(230400);
    Serial3.begin(230400);

    delay(5000);

    lidarSensor.begin(Serial1, Serial);//Serial1 pin A9,A10  connect to LIDAR , Serial (micro-USB) connect to computer(MATLAB)  
    //lidarSensor.start();
}


void loop()
{
    lidarSensor.handle();
#if 1
    if (dispFlag) {//lidarSensor.avail2
        if (angleCnt == 0) {
            Serial.print("start\n");
        }

        Serial.print(lidarSensor.ranges[angleCnt], DEC);
        //Serial.print(angleCnt, DEC);
        Serial.print("\n");//default matlab Terminator

        angleCnt++;
        if (angleCnt >= 360) {
            angleCnt = 0;
            dispFlag = false;
        }
    }
#endif

    if (Serial.available()) {      // If anything comes in Serial (USB),
        uint8_t asdf = Serial.read();
        if (asdf == 'b') {
            lidarSensor.start();
        }
        if (asdf == 'e') {
            lidarSensor.stop();
        }
        if (asdf == 'f') {
            dispFlag = true;
        }
        //Serial.println("0");
    }
    if (Serial2.available()) {      // If anything comes in Serial (USB),
        uint8_t asdf = Serial2.read();
        Serial.println("2");
    }
    if (Serial3.available()) {      // If anything comes in Serial (USB),
        uint8_t asdf = Serial3.read();
        Serial.println("3");
    }
}
