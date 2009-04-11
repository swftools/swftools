package {
    import flash.display.MovieClip
    import flash.events.Event

    public class Main extends flash.display.MovieClip {
        public function Main() {
            var x:int=0;
            var f1ok:Boolean = false;
            var f2ok:Boolean = false;
            var f3ok:Boolean = false;
            f1(1);
            function f1(level:int) {
                var y:int = 0;
                f2(level+1);
                x = y;
                f1ok = true;
                function f2(level:int) {
                    var z:int = 0;
                    f3(level+1);
                    y = z;
                    f2ok = true;
                    function f3(level:int) {
                        if(level==3)
                            trace("ok");
                        z = 4;
                        f3ok = true;
                    }
                }
            }
            if(x!=4 || !f1ok || !f2ok || !f3ok) 
                trace("error");
            trace("[exit]");
        }
    }
}
