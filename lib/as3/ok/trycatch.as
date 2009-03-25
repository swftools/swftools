package {
    import flash.display.MovieClip;
    import flash.net.navigateToURL;

    public class MyError {
    }
    public class MyOtherError {
        var ok5="ok 5/8";
    }
    public class ClassWithStaticFunctions {
        static function ok7() {
            trace("ok 7/8");
        }
    }
    public class Main extends flash.display.MovieClip {

        public function getClass():Class {
            return ClassWithStaticFunctions;
        }
		
        public function checkScope():void
        {
            try {
                flash.net.navigateToURL;
            } catch (e:Error) {
            }
            trace("ok 6/8");

            with(getClass()) {
                try {
                  throw new Error;
                } catch(e:Error) {
                    ok7();
                }
            }
        }
        public function checkActivation():void
        {
            var yy:uint = 0;
            var inc_y = function() {
                yy = yy + 1;
            }
            
            try {
              inc_y();
              throw new Error;
            } catch(e:Error) {
                inc_y();
            }
            if(yy!=2) trace("error ("+yy+")")
            else      trace("ok 8/8");
        }
		

        function Main() {
            try {
                throw new MyError
                trace("error");
            } catch(error:Error) {
                // MyError is not of the Error class
                trace("error");
            } catch(error:MyError) {
                trace("ok 1/8");
            } catch(x) {
                trace("error");
            }
            
            try {
                throw new MyOtherError
                trace("error");
            } catch(error:MyError) {
                trace("error");
            } catch(x:*) { // ":*" is the same as ""
                trace("ok 2/8");
            }
            
            try {
                trace("ok 3/8");
                // don't throw any error
            } catch(error:MyError) {
                trace("error");
            } catch(error:MyOtherError) {
                trace("error");
            } catch(x:*) { // ":*" is the same as ""
                trace("error");
            }

            trace("ok 4/8");

            try {throw new MyOtherError} 
            catch(x:*) {
                trace((x as MyOtherError).ok5);
            }

            checkScope();
            checkActivation();
            
            trace("[exit]");
        }
    }
}
