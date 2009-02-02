package {
    import flash.display.MovieClip;
    public class Main extends flash.display.MovieClip {
        var s1 = "ok 4/7";
        function ok1() {trace("ok 1/7");}
        var s2 = "ok 5/7";
        function ok2() {trace("ok 2/7");}
        const s3 = "ok 6/7"
        function ok3() {trace("ok 3/7");}
        var s4 = "ok 7/7";

        function Main() {
            ok1();
            ok2();
            ok3();
            trace(this.s1);
            trace(this.s2);
            trace(this.s3);
            trace(this.s4);
            trace("[exit]");
        }
    }
}
