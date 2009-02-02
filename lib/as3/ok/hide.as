
package {
    var xx = "error";

    function print_xx()
    {
        trace(xx);
    }

    import flash.display.MovieClip;
    public class Main extends flash.display.MovieClip {

        public var xx = "error";

        public static function f() 
        {
            // this sets the *package* variable x, 
            // *not* the class variable (which is unreachable from here)
            xx = "ok 1/1";
        }

        public function Main() {
            f(); // run static function
            print_xx();
            trace("[exit]");
        }
    }
}
