package {
    import flash.display.MovieClip
    public class Main extends flash.display.MovieClip {
        function Main() {
            /* test normal floats */
            var x = .25;
            if(x is Number) trace("ok 1/8");
            if(x*4==1.0) trace("ok 2/8");
            var y = -.25;
            if(y is Number) trace("ok 3/8");
            if(y*4==-1.0) trace("ok 4/8");

            /* test hex floats */
            var a = 0x.
            var b = 0xa.
            var c = 0x.4
            var d = 0xa.a
            if(a==0) trace("ok 5/8");
            if(b==10.0) trace("ok 6/8");
            if(c==0.25) trace("ok 7/8");
            if(d==10.625) trace("ok 8/8");
            trace("[exit]");
        }
    }
}
