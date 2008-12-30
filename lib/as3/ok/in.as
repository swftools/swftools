package {
    import flash.display.MovieClip;

    public class C {
        public var x;
        public function y() {};
        var e1;
        function e2() {};

        public static var s1;
        public static function s2() {}
    }
    public class Main extends flash.display.MovieClip {
        function Main() {
            /* test "in" for arrays */
            var a:Array = new Array(1,2,3,4);
            if(3 in a) trace("ok 1/7");
            if(5 in a) trace("error");
            else       trace("ok 2/7");

            /* test "in" for normal classes */
            var c:C = new C;
            if("x" in c) trace("ok 3/7");
            if("y" in c) trace("ok 4/7");
            if("e1" in c) trace("error"); // not public
            if("e2" in c) trace("error"); // not public

            /* test "in" for static members */
            if("s1" in C) trace("ok 5/7");
            if("s2" in C) trace("ok 6/7");

            /* test "in" for dynamic objects */
            var o:Object = new Object();
            o["r"] = 1;
            if("r" in o) trace("ok 7/7");
            delete o["r"];
            if("r" in o) trace("error");
        }
    }
}
