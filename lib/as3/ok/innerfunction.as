package {
    import flash.display.MovieClip
    import flash.events.Event

    public class Main extends flash.display.MovieClip {

        public function test1()
        {
            var y:int = 0;
            var a:Array = [null];
            a[0] = function() {
                y = y + 1;
            }
            a[0]();
            a[0]();
            if(y!=2) trace("error")
            else     trace("ok 1/2");
        }
        public function test2()
        {
            var y:uint = 0;
            var inc_y = function() {
                y = y + 1;
            }
            
            inc_y();
            inc_y();
            
            if(y!=2) trace("error")
            else     trace("ok 2/2");
        }
        public function Main()
        {
            this.test1()
            this.test2()
            trace("[exit]");
        }
    }
}
