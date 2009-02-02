package {
    import flash.display.MovieClip;
    public class Main extends flash.display.MovieClip {
        function f() {}
        function Main() {
            if(typeof(3)=="number") trace("ok 1/7");
            if(typeof("")=="string") trace("ok 2/7");
            if(typeof(this)=="object") trace("ok 3/7");
            if(typeof(undefined)=="undefined") trace("ok 4/7");
            if(typeof(null)=="object") trace("ok 5/7");
            if(typeof(f)=="function") trace("ok 6/7");
            if(typeof(Main)=="object" ||
               typeof(Main)=="movieclip") trace("ok 7/7");
            trace("[exit]");
        }
    }
}
