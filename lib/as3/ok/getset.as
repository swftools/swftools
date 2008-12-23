package package2 {
    public class Test {
        public function set x (v) {
            if(v==2)
                trace("ok 2/2");
        }
        public function get x ():uint {
            trace("ok 1/2");
            return 1;
        }
    }
}

package {
    import flash.display.MovieClip;
    import package2.Test

    public class Main extends flash.display.MovieClip {
        function Main() {
            var t:Test = new Test();
            t.x++;
        }
    }
}
