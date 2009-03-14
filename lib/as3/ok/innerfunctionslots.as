package {
    import flash.display.MovieClip
    import flash.events.Event

    public class Main extends flash.display.MovieClip {
        public function test(e:Event) {
            if(e.type instanceof String) {
                trace("ok");
            }

            function inner(x:String) {
                trace(e.type);
            }
        }
        public function Main() {
            var e = new Event("");
            test(e);
        }
    }
}
