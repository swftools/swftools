/* functions which are valid for this source file only */

function f1() {
    trace("ok 1/3")
};

package {
    import flash.display.MovieClip

    public class Main extends flash.display.MovieClip {
        public function Main() {
            f1()
            trace("ok 2/3")
            f3()
            trace("[exit]");
        }
    }
}

function f3() {
    trace("ok 3/3")
};

