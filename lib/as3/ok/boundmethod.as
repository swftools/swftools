package {
    import flash.display.MovieClip;
    public class Main extends flash.display.MovieClip {
        internal var r;
        function print(s) {
            trace(s);
            trace(this.r);
        }
        function Main() {
            var x = this.print;
            this.r = "ok 2/2";
            x("ok 1/2");
        }
    }
}
