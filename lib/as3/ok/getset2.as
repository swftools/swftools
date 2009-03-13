
package {
    import flash.display.MovieClip;

    public class Main extends flash.display.MovieClip {
        public static function set foobar(val:String ):void {
        }
        public static function get foobar():String
        {
                return "ok";
        }
        function Main() {
            trace(Main.foobar);
            trace("[exit]");
        }
    }
}
