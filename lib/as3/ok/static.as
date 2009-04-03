package {
    import flash.display.MovieClip;
    import flash.events.*

    public class Main extends flash.display.MovieClip {
        static var ok1 = "ok 1/12";
        static var ok2:String = "ok 2/12";

        static function ok5() {
            trace("ok 5/12");
        }

        static var ok3:String = "ok 6/12";
        static var ok4:String = "ok 7/12";
        static function f(s) {
            trace(s);
            trace(ok4);
        }
        static var ok8:String = "ok 8/12";

        static var _ok9:String = "ok 9/12";
        var ok9 = Main._ok9;

        static var ok11:String = "ok 11/12";
        var ok11:int = 12;

        function Main() {
            trace(ok1);
            trace(ok2);
            ok1 = "ok 3/12";
            ok2 = "ok 4/12";
            trace(ok1);
            trace(ok2);
            ok5();
            f(ok3);
            trace(Main.ok8);
            trace(this.ok9);
            
            XML.prettyPrinting = true;
            
            if(TextEvent.TEXT_INPUT == "textInput") 
                trace("ok 10/12");

            trace(this.ok11);
            trace("ok "+Main.ok11+"/12");

            trace("[exit]");
        }
    }
}
