package p {
    public class C {
        public function e() {
        }
    }
    public class D extends C {
        override function f() {
            // illegal override: no f() in superclass
        }
    }
}
