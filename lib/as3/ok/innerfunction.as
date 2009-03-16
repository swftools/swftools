package {
    import flash.display.MovieClip
    import flash.events.Event

    public class Main extends flash.display.MovieClip {
        public function Main()
        {
            var y:uint = 0;
            var inc_y = function() {
                y = y + 1;
            }
            
            inc_y();
            inc_y();
            
            if(y!=2) trace("error")
            else    trace("ok");
        }
    }
}
