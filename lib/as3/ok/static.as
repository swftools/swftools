package {
    import flash.display.MovieClip;

    public class Main extends flash.display.MovieClip {
        static var ok1 = "ok 1/5";
        static var ok2:String = "ok 2/5";

        static function ok5() {
            trace("ok 5/5");
        }

        function Main() {
            trace(ok1);
            trace(ok2);
            ok1 = "ok 3/5";
            ok2 = "ok 4/5";
            trace(ok1);
            trace(ok2);
            ok5();
        }
    }
}
