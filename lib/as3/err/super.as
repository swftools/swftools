package {
    public class A {
        function A() {
        }
    }
    public class Main {
        public var x;
        public function f() {
        }
        function Main() {
            this.x = 3; // error: access to instance variable before super()
            this.f()
            super();
            super(); // error: super called twice
        }
    }
}
