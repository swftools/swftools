package {
    import flash.display.MovieClip
    import flash.utils.ByteArray
    public interface I1 {
        function f()
    }
    public class TestI1 implements I1{
        public function f() {
            trace("ok");
        }
    }
    public class Main extends flash.display.MovieClip {
        function Main() {
            var x:I1 = new TestI1();
            x.f();
            trace("[exit]");
        }
    }
    public interface I2 {
	function init(key:ByteArray):void;
    }
}
