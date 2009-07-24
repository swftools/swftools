package {
    import flash.display.MovieClip
    public class Main extends flash.display.MovieClip {
        internal var s0;
        internal var s1:String="ok 3/5";
        internal var s2;
        public function printok1() {
            trace("ok 1/5");
        }
        public function printok2(x:uint) {
            trace(this.s0);
        }
        public function printok3() {
            trace(this.s2);
        }

        public function f() {
	    trace("ok 5/5");
	}
        public function get_f() {
	    return this["f"];
	}                                                                                                         

        function Main() {

            this.printok1();
            this.s0 = "ok 2/5";
            this.printok2(0);

            // member w/ default value:
            trace(this.s1);

            //omit "this":
            s2 = "ok 4/5";
            printok3();

	    get_f()();
            
            trace("[exit]");
        }
    }
}
