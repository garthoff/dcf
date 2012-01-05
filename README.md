## DCF77 Driver

This library is intended to be used with the conrad [DCF77][2] [receiver][1].

I never got the receiver to work reliably, but if you do, this library should decode the time successfully.


## Usage

``` c
#include <dcf.h>

DCF dcf(PIN);


void loop()
{
    dcf.pollTime(); // Do this in a loop as the lib does not use interrupt
    DCFTime time = dcf.time();
    if (time.synced) // Bingo we are synced
    {
        // access time.year, time.month, time.day, time.hour, time.minute, time.last_minute (this one is the last sync, in millis())
    }
}

```


[1]: http://www.conrad.com/DCF-receiver-board.htm?websale7=conrad-int&pi=641138&ci=SHOP_AREA_17348_1101115&Ctx=%7Bver%2F7%2Fver%7D%7Bst%2F3ec%2Fst%7D%7Bcmd%2F0%2Fcmd%7D%7Bm%2Fwebsale%2Fm%7D%7Bs%2Fconrad-int%2Fs%7D%7Bl%2Fint%2Fl%7D%7Bsf%2F%3Cs1%3Edcf%3C%2Fs1%3E%2Fsf%7D%7Bp1%2F275570afadab5f5c62680495454943dc%2Fp1%7D%7Bmd5%2F88d23823ec75094c0b81dd8ba3b1c6a7%2Fmd5%7D
[2]: http://en.wikipedia.org/wiki/DCF77
