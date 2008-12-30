package {
    import flash.display.MovieClip;
    import flash.events.*

    public class Main extends flash.display.MovieClip {
        static var ok1 = "ok 1/9";
        static var ok2:String = "ok 2/9";

        static function ok5() {
            trace("ok 5/9");
        }

        static var ok3:String = "ok 6/9";
        static var ok4:String = "ok 7/9";
        static function f(s) {
            trace(s);
            trace(ok4);
        }
        static var ok8:String = "ok 8/9";

        function Main() {
            trace(ok1);
            trace(ok2);
            ok1 = "ok 3/9";
            ok2 = "ok 4/9";
            trace(ok1);
            trace(ok2);
            ok5();
            f(ok3);
            trace(Main.ok8);
            
            XML.prettyPrinting = true;
            
            if(TextEvent.TEXT_INPUT == "textInput") 
                trace("ok 9/9");
        }
    }
}
