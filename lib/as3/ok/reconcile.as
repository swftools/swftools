package {
        import flash.display.MovieClip;
	import flash.utils.ByteArray;

	public class Main extends flash.display.MovieClip {

            // test whether local variables with default value "null"
            // will be coerced to their object
            public function test1() {
                var dataBuffer:Array;
                while(true) {
                    dataBuffer = new Array();
                    if(dataBuffer)
                        break;
                }
                trace("ok 1/8");
            }

            public function test2() {
                var output:String = "";
                while(output.length<1) {
                    output += "abc".charAt(0);
                }
                trace("ok 2/8");
            }

            public function test3() {
                var k:uint;

                while(true) {
                    for (var k:uint = 0; k < 4; k++) {
                    }
                    break;
                }
                trace("ok 3/8");
            }

            public var b:Boolean = false;

            public function test4() {
                var x:int;
                var y:uint;
                if(b) {
                    x = x+y;
                    y = y+x;
                }
                trace("ok 4/8");
            }
            public function test5() {
                var x:int;
                var y:uint;
                var z:int;
                if(b) {
                    x = x*y;
                    z = x*z;
                }
                trace("ok 5/8");
            }
            public function test6() {
                var i:int = b?3.0:3;
                var j:uint = b?3:"abc";
                trace("ok 6/8");
            }
            public function test7() {
                var j:Number = 0;
                if(b) {
                    j += 8;
                }
                trace("ok 7/8");
            }
            public function s():String {
                return "x";
            }
            public function test8() {
                var x:String = "abcd";

                var i:String = "test";
                // test return types of system libraries
                if(!b) {
                    i = x.charAt(0);
                }
                var a = "test";
                // test return types of system libraries
                if(!b) {
                    a = x.charAt(0);
                }

                // ...and local functions
                if(!b) {
                    i = s();
                }
                trace("ok 8/8");
            }

            public function Main() {
                test1();
                test2();
                test3();
                test4();
                test5();
                test6();
                test7();
                test8();
            
                trace("[exit]");
            }
        }
}
