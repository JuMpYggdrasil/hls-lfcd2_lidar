#ifndef HLS_LFCD2_h
#define HLS_LFCD2_h

// created by N'jump - meow meow

#include "Arduino.h"

class HLS_LFCD2
{
    public:
        HLS_LFCD2();// Constructor

        boolean avail = false;
        
        uint16_t ranges[360];
        //uint16_t intensities[360];

        //void begin(Stream &serial);
        void begin(Stream &serial, Stream &serialForDebug);
        void start(void);
        void stop(void);
        void handle(void);

        //call back Fn
        void preStart(void (*)());
        void postStart(void (*)());
        void preStop(void (*)());
        void postStop(void (*)());


    private:
        Stream* _serial;///< reference to serial port object
        Stream* _serialForDebug;

        static const uint16_t _minRange = 120;//120 default for hls-lfcd2
        static const uint16_t _maxRange = 3500;//3500 default for hls-lfcd2

        uint8_t u8BufferSize = 0;

        boolean _runFlag = false;
        boolean _syncFlag = false;
        
        static const uint8_t _u8MaxBufferSize = 42;//42 default for hls-lfcd2
        uint8_t _u8ResponseBuffer[_u8MaxBufferSize];

        void (*_preStart)();
        void (*_postStart)();
        void (*_preStop)();
        void (*_postStop)();
};

#endif
