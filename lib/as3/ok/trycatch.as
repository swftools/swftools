package {
    import flash.display.MovieClip;
    import flash.net.navigateToURL;

    public class MyError {
    }
    public class MyOtherError {
        var ok5="ok 5/7";
    }
    public class ClassWithStaticFunctions {
        static function ok7() {
            trace("ok 7/7");
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
            trace("ok 6/7");

            with(getClass()) {
                try {
                  throw new Error;
                } catch(e:Error) {
                    ok7();
                }
            }
        }
		

        function Main() {
            try {
                throw new MyError
                trace("error");
            } catch(error:Error) {
                // MyError is not of the Error class
                trace("error");
            } catch(error:MyError) {
                trace("ok 1/7");
            } catch(x) {
                trace("error");
            }
            
            try {
                throw new MyOtherError
                trace("error");
            } catch(error:MyError) {
                trace("error");
            } catch(x:*) { // ":*" is the same as ""
                trace("ok 2/7");
            }
            
            try {
                trace("ok 3/7");
                // don't throw any error
            } catch(error:MyError) {
                trace("error");
            } catch(error:MyOtherError) {
                trace("error");
            } catch(x:*) { // ":*" is the same as ""
                trace("error");
            }

            trace("ok 4/7");

            try {throw new MyOtherError} 
            catch(x:*) {
                trace((x as MyOtherError).ok5);
            }

            checkScope();
            
            trace("[exit]");
        }
    }
}
