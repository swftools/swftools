package p {
    internal class C {
        private function e() {
            trace("err");
        }
        protected function f() {
            trace("ok 1/5");
        }
    }
    public class D extends C {
        protected function e() {
        }
        override protected function f() {
            super.f()
        }
    }
}
package {
    public class X extends p.D {
        function X() {
            super.e();
            f()
        }
    }
    import flash.display.MovieClip
    public class Main extends flash.display.MovieClip {
        function Main() {
            var x = new X
            
            import p.D
            /* not sure what the difference between "is" and "instanceof" actually is */
            if(x is X) trace("ok 2/5");
            if(x is D) trace("ok 3/5");
            if(x instanceof X) trace("ok 4/5");
            if(x instanceof D) trace("ok 5/5");
        }
    }
}
