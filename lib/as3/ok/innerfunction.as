package {
    import flash.display.MovieClip
    import flash.events.Event

    public class Main extends flash.display.MovieClip {

        public function test1(y:int, ...rest)
        {
            var a:Array = [null];
            a[0] = function() {
                y = y + 1;
            }
            a[0]();
            a[0]();
            if(y!=2) trace("error")
            else     trace("ok 1/2");
        }
        public function test2(y:uint)
        {
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
            this.test1(0,5)
            this.test2(0)
            trace("[exit]");
        }
    }
}
