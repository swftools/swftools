package {
    import flash.display.MovieClip

    public class Main extends flash.display.MovieClip {
        public function Main() {
            var i = 0;
            f(0);
            function f(j:int) {
                if(i!=j) 
                    trace("error");
                i++;
                if(j==3) {
                    trace("ok");
                    return;
                }
                f(j+1);
            }
            trace("[exit]");
        }
    }
}
