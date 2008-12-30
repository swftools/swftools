package {
    import flash.display.MovieClip
    import flash.geom.Point

    public class Main extends flash.display.MovieClip {
        
        function run(f:Function, text:String) {
            f(text);
        }

        public function Main() {
            var log = function(text) {
                trace(text);
            }
            log("ok 1/3");
            var l = log;
            l("ok 2/3");

            run(function(text) {trace(text)}, "ok 3/3")
        }
    }
}
