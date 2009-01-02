package {
    import flash.display.MovieClip;

    public class Main extends flash.display.MovieClip {
        function test_return() {
            try {
                return;
                trace("error");
            } finally {
                trace("ok 1/8");
            }
            trace("error");
        }
        
        function test_break() {
            do {
                try {
                    break;
                    trace("error");
                } finally {
                    trace("ok 2/8");
                }
                trace("error");
            } while(true);
        }
        
        function test_fallthrough() {
            try {
                var x = 1+1;
            } finally {
                trace("ok 3/8");
            }
        }
        
        function test_exception() {
            try {
                try {
                    throw new Error();
                    trace("error");
                } finally {
                    trace("ok 4/8");
                }
                trace("error");
            } catch(e:Error) {
                trace("ok 5/8");
            }
        }
        
        function test_exception2() {
            var x:int = 0;
            try {
                throw new Error();
                trace("error");
            } catch(e:Error) {
                x=1;
            } finally {
                x*=2;
            }
            if(x==2)
                trace("ok 6/8");
        }

        function fail() {
            try {
                throw new Error();
            } finally {
                trace("ok 7/8");
            }
        }
        
        function test_exception3() {
            try {
                fail();
            } catch(e:Error) {
                trace("ok 8/8");
            }
        }

        function Main() {
            test_return();
            test_break();
            test_fallthrough();
            test_exception();
            test_exception2();
            test_exception3();
        }
    }
}
