package {

    public class E {
    }

    public class A extends E {
        static var x = D.d();
        trace("ok 4/4");
    }
    public class B {
        static var x = C.c();
        trace("ok 2/4");
        static function b() {
        }
    }
    public class C extends E {
        trace("ok 1/4");
        static function c() {
        }
    }
    public class D {
        static var x = C.c();
        trace("ok 3/4");
        if(B.b) {}
        static function d() {
        }
    }

    import flash.display.MovieClip;
    public class Main extends MovieClip {
        function Main() {
            trace("[exit]");
        }
    }
}
