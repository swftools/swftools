package {
    import flash.display.MovieClip
    public class Main extends flash.display.MovieClip {
        internal var s0;
        internal var s1:String="ok 3/4";
        internal var s2;
        public function printok1() {
            trace("ok 1/4");
        }
        public function printok2(x:uint) {
            trace(this.s0);
        }
        public function printok3() {
            trace(this.s2);
        }
        function Main() {

            this.printok1();
            this.s0 = "ok 2/4";
            this.printok2(0);

            // member w/ default value:
            trace(this.s1);

            //omit "this":
            s2 = "ok 4/4";
            printok3();
        }
    }
}
