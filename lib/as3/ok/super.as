package {
    class C {
        function C(s:String) {
            trace(s);
        }
    }
    class D extends C {
        function D() {
            //explicit super call w/ args
            super("ok");
        }
    }
    class E extends D {
        function E() {
            //explicit super call
            super();
        }
    }
    class F extends E {
        function F() {
            //implicit super call
        }
    }
    import flash.display.MovieClip
    public class Main extends flash.display.MovieClip {
        function Main() {
            new F
            trace("[exit]");
        }
    }
}
