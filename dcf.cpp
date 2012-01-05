#include "dcf.h"

DCF::DCF(int pin)
{
    _pin = pin;
    _firstPulse = true;
    _inPulse = false;
    _pulseStart = 0;
    pinMode(_pin, INPUT);
    digitalWrite(_pin, HIGH);
}

void DCF::pollTime(void)
{
    _advanceTime();
    int value = digitalRead(_pin);
    if (value && _firstPulse)
        return;
    else if (_firstPulse)
    {
        _firstPulse = false;
        return;
    }

    unsigned long pulseDuration;
    unsigned long now = millis();
    if (!value)
    {
        if (_inPulse)
        {
            unsigned long pulseInterval = now - _lastPulseStart;
            pulseDuration = now - _pulseStart;
            if (pulseInterval < 1000)
                return;
            _inPulse = false;
            _pulseEnd = now;
            _lastPulseStart = _pulseStart;
            if (_pulseNumber > 59) _pulseNumber = 0; // Overflow protection
            _buffer[_pulseNumber] = pulseDuration > 200;
            _pulseNumber++;
        }
    }
    else if (!_inPulse)
    {
        unsigned long pulseInterval = now - _lastPulseStart;
        if (pulseInterval < 800)
            return;
        else if (pulseInterval > 1500)
        {
            if (_pulseNumber == 59)
                _decodeTime();;
            _pulseNumber = 0;
        }
        _inPulse = true;
        _pulseStart = now;
    }

}

void DCF::_advanceTime(void)
{
    if (!_time.synced) return;

    if (millis() - _time.last_minute > 60000)
    {
        _time.last_minute = millis();
        _time.minute++;
        if (_time.minute == 60)
        {
            _time.minute = 0;
            _time.hour++;
        }
        if (_time.hour == 24)
        {
            _time.hour = 0;
        }
    }
}

void DCF::_decodeTime(void)
{
    if (_buffer[0] || !_buffer[20]) return;
    if (!_parityCheck(21, 28)) return;
    if (!_parityCheck(29, 35)) return;
    if (!_parityCheck(36, 58)) return;
    _time.synced = true;
    _time.last_minute = millis();
    _time.minute = _buffer[21] +
                   2 * _buffer[22] +
                   4 * _buffer[23] +
                   8 * _buffer[24] +
                   10 * _buffer[25] +
                   20 * _buffer[26] +
                   40 * _buffer[27];
    _time.hour = _buffer[29] +
                 2 * _buffer[30] +
                 4 * _buffer[31] +
                 8 * _buffer[32] +
                 10 * _buffer[33] +
                 20 * _buffer[34];
    _time.day = _buffer[36] +
                2 * _buffer[37] +
                4 * _buffer[38] +
                8 * _buffer[39] +
                10 * _buffer[40] +
                20 * _buffer[41];
    _time.month = _buffer[45] +
                2 * _buffer[46] +
                4 * _buffer[47] +
                8 * _buffer[48] +
                10 * _buffer[49];
    _time.year = _buffer[50] +
                 2 * _buffer[51] +
                 4 * _buffer[52] +
                 8 * _buffer[53] +
                 10 * _buffer[54] +
                 20 * _buffer[55] +
                 40 * _buffer[56] +
                 80 * _buffer[57] + 2000;
}

bool DCF::_parityCheck(int startBit, int endBit)
{
    int parity = 0;
    for (int i = startBit; i < endBit; i++)
    {
        parity += _buffer[i];
    }

    return parity % 2 == _buffer[endBit];
}

DCFTime DCF::time(void)
{
    return _time;
}
