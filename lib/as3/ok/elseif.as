package {
    import flash.display.MovieClip;
    public class Main extends flash.display.MovieClip {
        function Main() {
            if(false) {
                trace("error");
            } else if(true) {
                trace("ok 1/1");
            } else {
                trace("error");
            }
            
            trace("[exit]");
        }
    }
}
