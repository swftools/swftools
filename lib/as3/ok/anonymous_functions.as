package {
    import flash.display.MovieClip
    import flash.geom.Point

    public var func:Function = function(nr:uint) {trace("ok "+nr+"/4");}

    public class Main extends flash.display.MovieClip {
        
        function run(f:Function, text:String) {
            f(text);
        }

        public function Main() {
            var log = function(text) {
                trace(text);
            }
            log("ok 1/4");
            var l = log;
            l("ok 2/4");

            run(function(text) {trace(text)}, "ok 3/4")

            func(4);

            trace("[exit]");
        }
    }
}
