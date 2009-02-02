package {

    public var mystring:String;
    public function run():void
    {
        mystring = "test";
        trace('ok');
    }

    import flash.display.MovieClip
    public class Main extends flash.display.MovieClip {
        public function Main () {
            run();
            trace("[exit]");
        }
    }
}
