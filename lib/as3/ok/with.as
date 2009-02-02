package {
    import flash.display.MovieClip;
    public class A {
        var ok1 = "ok 1/4";
        function ok2() {
            trace("ok 2/4");
        }
        static var ok3="ok 3/4";
        static function ok4() {
            trace("ok 4/4");
        }
    }
    public class Main extends flash.display.MovieClip {
        function Main() {
            var a:A = new A;
            with(a) {
                trace(ok1);
                ok2();
            }
            with(A) {
                trace(ok3);
                ok4();
            }
            trace("[exit]");
        }
    }
}
