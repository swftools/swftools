package {
    import flash.display.MovieClip;

    public class MyError {
    }
    public class MyOtherError {
        var ok5="ok 5/5";
    }
    public class Main extends flash.display.MovieClip {
        function Main() {
            try {
                throw new MyError
                trace("error");
            } catch(error:Error) {
                // MyError is not of the Error class
                trace("error");
            } catch(error:MyError) {
                trace("ok 1/5");
            } catch(x) {
                trace("error");
            }
            
            try {
                throw new MyOtherError
                trace("error");
            } catch(error:MyError) {
                trace("error");
            } catch(x:*) { // ":*" is the same as ""
                trace("ok 2/5");
            }
            
            try {
                trace("ok 3/5");
                // don't throw any error
            } catch(error:MyError) {
                trace("error");
            } catch(error:MyOtherError) {
                trace("error");
            } catch(x:*) { // ":*" is the same as ""
                trace("error");
            }

            trace("ok 4/5");

            try {throw new MyOtherError} 
            catch(x:*) {
                trace((x as MyOtherError).ok5);
            }
            trace("[exit]");

        }
    }
}
