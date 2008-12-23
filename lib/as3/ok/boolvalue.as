package {
    import flash.display.MovieClip;
    public class Main extends flash.display.MovieClip {
        function Main() {
            trace(false || "ok 1/6");
            trace("ok 2/6" || "error");

            if(false && true) trace("error") else trace("ok 3/6");
            trace("error" && "ok 4/6");

            if(!false) trace("ok 5/6");

            if(!true) trace("error");
            else      trace("ok 6/6");
        }
    }
}
