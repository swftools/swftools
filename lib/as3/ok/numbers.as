package test.package {
    public class Main extends flash.display.MovieClip {
        function Main() {
            /* test the encoding of various constants.
               Depending on the radix size, int constants may be
               encoded as byte, u30 short, u31 short, signed, etc. */
            var a1 = 127;
            if(String(a1) == "127") trace("ok 1/15");
            var a2 = 256;
            if(String(a2) == "256") trace("ok 2/15");
            var a3 = 1073741823;
            if(String(a3) == "1073741823") trace("ok 3/15");
            var a4 = 1073741824;
            if(String(a4) == "1073741824") trace("ok 4/15");
            var a5 = 2147483647;
            if(String(a5) == "2147483647") trace("ok 5/15");
            var a6 = -1;
            if(String(a6) == "-1") trace("ok 6/15");
            var a7 = -127;
            if(String(a7) == "-127") trace("ok 7/15");
            var a8 = -32767;
            if(String(a8) == "-32767") trace("ok 8/15");
            var a9 = -32768;
            if(String(a9) == "-32768") trace("ok 9/15");
            var a10 = -32769;
            if(String(a10) == "-32769") trace("ok 10/15");
            var a11 = -65536;
            if(String(a11) == "-65536") trace("ok 11/15");
            var a12 = -1073741823;
            if(String(a12) == "-1073741823") trace("ok 12/15");
            var a13 = -1073741824;
            if(String(a13) == "-1073741824") trace("ok 13/15");
            var a14 = 0xa3b5c7f;
            if(String(a14) == "171662463") trace("ok 14/15");
            var a15 = -0xa3b5c7f;
            if(String(a15) == "-171662463") trace("ok 15/15");
        }
    }
}

