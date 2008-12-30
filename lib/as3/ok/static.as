package {
    import flash.display.MovieClip;
    import flash.events.*

    public class Main extends flash.display.MovieClip {
        static var ok1 = "ok 1/10";
        static var ok2:String = "ok 2/10";

        static function ok5() {
            trace("ok 5/10");
        }

        static var ok3:String = "ok 6/10";
        static var ok4:String = "ok 7/10";
        static function f(s) {
            trace(s);
            trace(ok4);
        }
        static var ok8:String = "ok 8/10";

        static var _ok9:String = "ok 9/10";
        var ok9 = Main._ok9;

        function Main() {
            trace(ok1);
            trace(ok2);
            ok1 = "ok 3/10";
            ok2 = "ok 4/10";
            trace(ok1);
            trace(ok2);
            ok5();
            f(ok3);
            trace(Main.ok8);
            trace(this.ok9);
            
            XML.prettyPrinting = true;
            
            if(TextEvent.TEXT_INPUT == "textInput") 
                trace("ok 10/10");
        }
    }
}
