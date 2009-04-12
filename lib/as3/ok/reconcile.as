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
                trace("ok 1/9");
            }

            public function test2() {
                var output:String = "";
                while(output.length<1) {
                    output += "abc".charAt(0);
                }
                trace("ok 2/9");
            }

            public function test3() {
                var k:uint;

                while(true) {
                    for (var k:uint = 0; k < 4; k++) {
                    }
                    break;
                }
                trace("ok 3/9");
            }

            public var b:Boolean = false;

            public function test4() {
                var x:int;
                var y:uint;
                if(b) {
                    x = x+y;
                    y = y+x;
                }
                trace("ok 4/9");
            }
            public function test5() {
                var x:int;
                var y:uint;
                var z:int;
                if(b) {
                    x = x*y;
                    z = x*z;
                }
                trace("ok 5/9");
            }
            public function test6() {
                var i:int = b?3.0:3;
                var j:uint = b?3:"abc";
                trace("ok 6/9");
            }
            public function test7() {
                var j:Number = 0;
                if(b) {
                    j += 8;
                }
                trace("ok 7/9");
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
                trace("ok 8/9");
            }
            
            public function test_add() {
                var i:int = 3;
                var s:String = "4";
                var any1 = "any1";
                var any2 = "any2";
                var xml:XML = <test/>
                var xmllist:XMLList = XMLList(<test/>)
                var d1:Date = new Date();
                var d2:Date = new Date();
                var array1:Array = [1];
                var array2:Array = [1,2];

                /* int+String */
                var z1a:Object;
                var z1b:String;
                if(b) {
                    z1a = i+s;
                    z1b = i+s;
                }

                /* String+int */
                var z2a:Object;
                var z2b:String;
                if(b) {
                    z2a = s+i;
                    z2b = s+i;
                }

                /* any+any */
                var z3a:*;
                var z3b:Object;
                if(b) {
                    z3a = any1+any2;
                    z3b = any1+any2;
                }
                
                /* any+String */
                var z4a:Object;
                var z4b:*;
                if(b) {
                    z4a = any1+s;
                    z4b = any1+s;
                }

                /* xml+xmllist */
                var z5a:Object;
                var z5b:XMLList;
                if(b) {
                    z5a = xml+xmllist;
                    z5b = xml+xmllist;
                }

                /* xmllist+xml */
                var z6:Object;
                if(b) {
                    z6 = xmllist+xml;
                }
                
                /* xml+any */
                var z7a:*;
                var z7b:XMLList;
                if(b) {
                    z7a = xml+any1;
                    z7b = xml+any1;
                }
                
                /* date+date */
                var z8a:Object;
                var z8b:String;
                if(b) {
                    z8a = d1+d2;
                    z8b = d1+d2;
                }
                
                /* date+any */
                var z9a:Object;
                var z9b:String;
                if(b) {
                    z9a = d1+any1;
                    z9b = d1+any1;
                }
                
                /* any+date */
                var z10a:Object;
                var z10b:*;
                if(b) {
                    z10a = any1+d1;
                    z10b = any1+d1;
                }
                
                /* array+array */
                var z11a:Object;
                var z11b:Array;
                if(b) {
                    z11a = array1+array2;
                    z11b = array1+array2;
                }
                trace("ok 9/9");
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
                test_add();
            
                trace("[exit]");
            }
        }
}
