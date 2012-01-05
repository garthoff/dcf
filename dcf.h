
#ifndef DCF_H_
#define DCF_H_

#include "Arduino.h"

class DCFTime
{
public:
    int year;
    char month;
    char day;
    char hour;
    char minute;
    unsigned long last_minute;
    bool synced;
};

class DCF
{
public:
  DCF(int pin);
  void pollTime(void);
  DCFTime time(void);

private:
  void _decodeTime(void);
  void _advanceTime(void);
  int _pin;
  bool _firstPulse;
  bool _inPulse;
  int _pulseNumber;
  bool _buffer[60];
  unsigned long _lastPulseStart;
  unsigned long _pulseStart;
  unsigned long _pulseEnd;
  DCFTime _time;
  bool _parityCheck(int startBit, int endBit);
};

#endif
