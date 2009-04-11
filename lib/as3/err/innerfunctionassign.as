package {
    import flash.display.MovieClip
    import flash.events.Event

    public class Main extends flash.display.MovieClip {

        /* this test is just to showcase a problem with slotted functions
           in conjunction with inner function assignment.
           What (I think) the compiler should do is just to prevent inner
           functions from being assigned. */
        public function Main() {
            function f0() {
                trace("error");
            }
            f0 = function(s:String) {
                trace(s);
            }
            f0("ok 1/2");
            function() {f0("ok 2/2");}()
        }
    }
}
