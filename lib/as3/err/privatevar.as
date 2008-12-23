package package1 {
    public class A {
        private var x;
    }
    public class Test extends flash.display.MovieClip {
        public function Test() {
            var cls = new package1.A();
            cls.x=3; //invalid: x is private
        }
    }
}
