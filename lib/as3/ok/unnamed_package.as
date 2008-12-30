/* test whether variables and functions in the global package are accessible everywhere */

package {
    
    public var ok1="ok 1/3";
    internal var ok2="ok 2/3";
    public var ok3="ok 3/3";

    public function f() {
        trace(ok2);
    }
    public class C {
        function C() {
            trace(ok3);
        }
    }

}
package package1 {
    public function g() {
        trace(ok1)
        f()
        new C
    }
}

package {
    import package1.g
    import flash.display.MovieClip

    public class Main extends flash.display.MovieClip {
        public function Main() {
            g()
        }
    }
}
