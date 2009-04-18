package {
    import flash.display.MovieClip

    public class A {
        if(B == null)
            trace("error");
        B.b();
        static function a() {trace("ok 5/5")}
    }
    public class B {
        static function b() {trace("ok 4/5")}
    }
    public class C {
        if(A == null || B == null)
            trace("error");
        A.a();
    }
    
    public class Main extends flash.display.MovieClip {
	public static const types:Array = ["error", "ok 1/5", "error"];

        public static var fptr:Function = Main.f;

        public var c1="ok 3/5";
        public var c2=c1;

        public static function f() {
            trace(types[1]);
            trace("ok 2/5");
        }

        function Main() {
            fptr();
            trace(c2);
        }
    }
}
