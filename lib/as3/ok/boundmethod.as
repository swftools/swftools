package {
    import flash.display.MovieClip;

    public class Other {
        var a="ok 3/3";

        function print2() {
            trace(this.a);
        }
    }

    public class Main extends flash.display.MovieClip {
        internal var r;
        function print(s) {
            trace(s);
            trace(this.r);
        }
        function Main() {
            var x = this.print;
            this.r = "ok 2/3";
            x("ok 1/3");

            x = (new Other).print2;
            x();
            
            trace("[exit]");
        }
    }
}
