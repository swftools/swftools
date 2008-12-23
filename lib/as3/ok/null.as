package {

    public class Main extends flash.display.MovieClip {
        public function Main() {
            var x = null;
            var y = null;
            if(x == y) {
                trace("ok 1/2");
            }
            if(x === y) {
                trace("ok 2/2");
            }

        }
    }

}
