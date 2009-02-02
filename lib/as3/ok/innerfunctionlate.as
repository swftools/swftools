package {
    import flash.display.MovieClip

    public class Main extends flash.display.MovieClip {
        public function Main() {
            var log = f3;
            f1("ok 1/3");
            var f = f2;
            f("2/3");

            f0();

            function f0() {
                f3("ok","3","3");
            }
            function f1(text) {
                trace(text);
            }
            function f2(text) {
                trace("ok "+text);
            }
            function f3(t1,t2,t3) {
                trace(t1+" "+t2+"/"+t3);
            }
        }
    }
}
