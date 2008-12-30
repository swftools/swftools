package package_A {

    import flash.utils.setInterval;

    internal class class_A {

        function class_A() {
            trace("ok 2/4")
        }
        internal function f() {
            trace("ok 3/4");
        }
        static internal function g() {
            trace("ok 4/4");
        }
    }

    internal function i_A() {
        trace("ok 1/4");
        (new class_A).f()
    }

    public function p_A() {
        i_A();
        class_A.g()
    }
}

package {
    import flash.display.MovieClip
    import package_A.p_A

    public class Main extends flash.display.MovieClip {
        public function Main() {
            p_A()
        }
    }
}
