package {
    import flash.display.MovieClip
    public interface I1 {
        function f()
    }
    public interface I2 extends I1 {
        function g()
    }
    public class C implements I2 {
        public function f() {
            trace("ok 1/2");
        }
        public function g() {
            trace("ok 2/2");
        }
    }

    public class Main extends flash.display.MovieClip {
        function Main() {
            var x:I2 = new C;
            x.f();
            x.g();
            
            trace("[exit]");
        }
    }
}
