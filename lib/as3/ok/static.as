package {
    import flash.display.MovieClip;

    public class Main extends flash.display.MovieClip {
        static var ok1 = "ok 1/7";
        static var ok2:String = "ok 2/7";

        static function ok5() {
            trace("ok 5/7");
        }

        static var ok3:String = "ok 6/7";
        static var ok4:String = "ok 7/7";
        static function f(s) {
            trace(s);
            trace(ok4);
        }

        function Main() {
            trace(ok1);
            trace(ok2);
            ok1 = "ok 3/7";
            ok2 = "ok 4/7";
            trace(ok1);
            trace(ok2);
            ok5();
            f(ok3);
        }
    }
}
