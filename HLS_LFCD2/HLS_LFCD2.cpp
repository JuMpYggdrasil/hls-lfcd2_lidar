#include "HLS_LFCD2.h"

//#define debugPrint

HLS_LFCD2::HLS_LFCD2(void)
{
    _preStart = 0;
    _postStart = 0;
    _preStop = 0;
    _postStop = 0;
}
//void HLS_LFCD2::begin(Stream &serial)
//{
//    _serial = &serial;
//    _serialForDebug = 0;
//}
void HLS_LFCD2::begin(Stream &serial, Stream &serialForDebug)
{
    _serial = &serial;
    _serialForDebug = &serialForDebug;
}
void HLS_LFCD2::start(void) {
    // flush receive buffer before transmitting request
    while (_serial->read() != -1);

    if (_preStart)
    {
        _preStart();
    }

    _serial->write('b');
    _serial->flush();    // flush transmit buffer

    if (_postStart)
    {
        _postStart();
    }

    _runFlag = true;
}
void HLS_LFCD2::stop(void) {
    // flush receive buffer before transmitting request
    while (_serial->read() != -1);

    if (_preStop)
    {
        _preStop();
    }

    _serial->write('e');
    _serial->flush();    // flush transmit buffer

    if (_postStop)
    {
        _postStop();
    }
    _runFlag = false;
}

void HLS_LFCD2::handle(void)
{
    uint8_t u8RxTempo;
    uint8_t u8RawRxBuff[256];
    uint8_t i;
    boolean errorDetect = false;

    if (_runFlag) {
        while (_serial->available()) {//not error and not time out
            u8RxTempo = _serial->read();

            if (u8RxTempo == 0xFA) {//0xFA
                _syncFlag = true;
            }

            if (_syncFlag) {
                u8RawRxBuff[u8BufferSize] = u8RxTempo;
#ifdef debugPrint
                _serialForDebug->print(u8RxTempo, HEX);
                _serialForDebug->print(' ');
#endif
                if (u8BufferSize == 1) {
                    uint16_t angleIndex = (u8RawRxBuff[1] - 0xA0) * 6;

                    if (u8RawRxBuff[u8BufferSize] >= 0xA0  && u8RawRxBuff[u8BufferSize] <= 0xDB) {
#ifdef debugPrint
                        _serialForDebug->print("(");
                        _serialForDebug->print(angleIndex);
                        _serialForDebug->print(")");
#endif
                    } else {
#ifdef debugPrint
                        _serialForDebug->print("error");
#endif
                        //errorDetect = true;
                        //_syncFlag = false;
                    }
                }

                if (u8BufferSize == 7) {
                    uint16_t range = (u8RawRxBuff[7] << 8) + u8RawRxBuff[6];
#ifdef debugPrint
                    _serialForDebug->print("[");
                    _serialForDebug->print(range);
                    _serialForDebug->print("]");
#endif
                }
                if (u8BufferSize == 13) {
                    uint16_t range = (u8RawRxBuff[13] << 8) + u8RawRxBuff[12];
#ifdef debugPrint
                    _serialForDebug->print("[");
                    _serialForDebug->print(range);
                    _serialForDebug->print("]");
#endif
                }

                if (++u8BufferSize >= _u8MaxBufferSize) {
                    _syncFlag = false;
                    avail = true;
                    break;
                }
            }
            //check time out
        }
        //_serialForDebug->println("hi");
        //parser package
        //if not error or timeout
        if (!errorDetect && avail) {
            avail = false;
            //avail2 = true;
#ifdef debugPrint
            _serialForDebug->println();
#endif
            if (u8BufferSize <= _u8MaxBufferSize) {
                uint16_t degree_count_num = 0;
                uint16_t angleIndex = (u8RawRxBuff[1] - 0xA0) * 6;

                for (uint8_t j = 4; j < 40; j = j + 6)
                {
                    uint8_t byte0 = u8RawRxBuff[j];
                    uint8_t byte1 = u8RawRxBuff[j + 1];
                    uint8_t byte2 = u8RawRxBuff[j + 2];
                    uint8_t byte3 = u8RawRxBuff[j + 3];
                    //byte4,5 is reserved*

                    //uint16_t intensity = (byte1 << 8) + byte0;
                    uint16_t range     = (byte3 << 8) + byte2;

                    //intensities[359 - angleIndex - degree_count_num] = intensity;
                    ranges[359 - angleIndex - degree_count_num] = range;

                    //    if (range < _minRange) {
                    //        ranges[359 - angleIndex - degree_count_num] = _minRange;
                    //        //ranges[angleIndex + degree_count_num] = _minRange;
                    //    } else if (range > _maxRange) {
                    //        ranges[359 - angleIndex - degree_count_num] = _maxRange;
                    //        //ranges[angleIndex + degree_count_num] = _maxRange;
                    //    } else {
                    //        ranges[359 - angleIndex - degree_count_num] = range;
                    //        //ranges[angleIndex + degree_count_num] = range;
                    //    }

                    degree_count_num++;
                }
            }
            //reset index
            if ((u8BufferSize >= _u8MaxBufferSize) || errorDetect) {
                u8BufferSize = 0;
            }
        }
    }

    return;//pass status
}


void HLS_LFCD2::preStart(void (*preStart)())
{
    _preStart = preStart;
}

void HLS_LFCD2::postStart(void (*postStart)())
{
    _postStart = postStart;
}
void HLS_LFCD2::preStop(void (*preStop)())
{
    _preStop = preStop;
}

void HLS_LFCD2::postStop(void (*postStop)())
{
    _postStop = postStop;
}
