package {

    public class Test {
        function Test() {
        }
        function getok1() {
            return "ok 1/2";
        }
        function getok2():String {
            return "ok 2/2";
        }
    };

    import flash.display.MovieClip
    public class Main extends flash.display.MovieClip {
        function Main() {
            trace((new Test).getok1());
            var t = new Test;
            trace(t.getok2());
            
            trace("[exit]");
        }
    }
}
