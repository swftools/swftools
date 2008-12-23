package {
    import flash.display.MovieClip
    public class Main extends flash.display.MovieClip {
        function Main() {
            message1 = "ok 1/2";
            this.message2 = "ok 2/2";
            printok1();
        }
        public function printok1() {
            printok2(17);
        }
        public function printok2(x:uint) {
            if(x==17)
                this.printok3();
        }
        public function printok3() {
            trace(message1);
            trace(this.message2);
        }
        internal var message1:String = "error";
        internal var message2:String = "error";
    }
}
