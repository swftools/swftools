package {
    import flash.display.MovieClip;

    public class Main extends flash.display.MovieClip {
        static var ok1 = "ok 1/8";
        static var ok2:String = "ok 2/8";

        static function ok5() {
            trace("ok 5/8");
        }

        static var ok3:String = "ok 6/8";
        static var ok4:String = "ok 7/8";
        static function f(s) {
            trace(s);
            trace(ok4);
        }
        static var ok8:String = "ok 8/8";

        function Main() {
            trace(ok1);
            trace(ok2);
            ok1 = "ok 3/8";
            ok2 = "ok 4/8";
            trace(ok1);
            trace(ok2);
            ok5();
            f(ok3);
            trace(Main.ok8);
            
            XML.prettyPrinting = true;
        }
    }
}
