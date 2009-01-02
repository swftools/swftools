package {
    interface A {
        function f()
    }
    interface B {
        function f(x)
    }

    public class C implements A,B
    {
        // ambigious definition (or: B.f not implemented)
        public function f() {}
    }
}
