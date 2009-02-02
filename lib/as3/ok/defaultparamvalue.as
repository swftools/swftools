package {
    import flash.display.MovieClip
    public class Main extends flash.display.MovieClip {

        public function testuint(x:uint=33) {
            if(x == 33) trace("ok 5/12");
        }
        public function testint(x:int=-33) {
            if(x == -33) trace("ok 6/12");
        }
        public function teststring(x:String="33") {
            if(x == "33") trace("ok 7/12");
        }
        public function testfloat(x:Number=33.33) {
            if(x == 33.33) trace("ok 8/12");
        }
        public function testtrue(x:Boolean=true) {
            if(x == true) trace("ok 9/12");
        }
        public function testfalse(x:Boolean=false) {
            if(x == false) trace("ok 10/12");
        }
        public function testnull(x:Object=null) {
            if(x == null) trace("ok 11/12");
        }
        public function testfloat2(x:Number=33) {
            if(x == 33.0) trace("ok 12/12");
        }

        public function myfunction(x:String="ok 2/12") 
        {
            trace(x);
        }
        public function myfunction2(i:int,x:String="ok 4/12") 
        {
            trace(x);
        }

        public function Main() {
            myfunction("ok 1/12");
            myfunction();
            myfunction2(0,"ok 3/12");
            myfunction2(0);

            testuint();
            testint();
            teststring();
            testfloat();
            testtrue();
            testfalse();
            testnull();
            testfloat2();
            
            trace("[exit]");
        }
    }
}
