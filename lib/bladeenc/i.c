#include "codec.h"

int main()
{
    CodecInitOut * init;
    CodecInitIn params;
    char buf[1024];
    short data[1024];
    int len = 0;

    memset(&params, 0, sizeof(params));
    params.frequency = 32000;  //48000, 44100 or 32000
    params.mode = 0;	  //0 = Stereo, 2 = Dual Channel, 3 = Mono
    params.emphasis = 0;  //0 = None, 1 = 50/15 microsec, 3 = CCITT J.17
    params.bitrate = 128;	  //default is 128 (64 for mono)

    init = codecInit(&params);

    len = codecEncodeChunk(20, data, buf);
    len += codecFlush (&buf[len]);
    len += codecExit(&buf[len]);

    printf("data is %d bytes\n", len);
}
