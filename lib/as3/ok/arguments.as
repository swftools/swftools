package {
    import flash.display.MovieClip
    
    public class Main extends flash.display.MovieClip {
        
        public function f(x,y) {
            var z = arguments;
            trace(z[0]);
            trace(z[1]);
        }

        function test_function()
        {
            f("ok 1/8","ok 2/8");
        }
        function test_innerfunction()
        {
            var z = function(x,y) {
                var z = arguments;
                trace(z[0]);
                trace(z[1]);
            }
            z("ok 3/8", "ok 4/8");
        }
        function test_innerslotfunction()
        {
            var i = 0;
            var z = function(x,y) {
                var v = arguments;
                trace(v[0]);
                trace(v[1]);
                i++;
            }
            z("ok 5/8", "ok 6/8");
            if(i!=1) 
                trace("error");
        }
        function test_slottedfunction(r1:String, r2:String)
        {
            var z = arguments;
            if(!(z instanceof Array))
                trace("error");
            if(!(r1 instanceof String))
                trace("error");
            if(!(r2 instanceof String))
                trace("error");
            function inner() {
                trace(z[0]);
                trace(z[1]);
            }
            inner();
        }

        function Main() {
            test_function();
            test_innerfunction();
            test_innerslotfunction();
            test_slottedfunction("ok 7/8", "ok 8/8");
            trace("[exit]");
        }
    }
}
