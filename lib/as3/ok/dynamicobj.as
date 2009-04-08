
package {
    import flash.display.MovieClip
    public class Main extends flash.display.MovieClip {

        function test_empty_curlies() {
            var y:Object = {};
            if(true) {}
            {}
        }

        function Main() {
            var x:Object = 
            {"one": 1,
             "two": 2,
             "three": 3
            };
            
            if(x["two"]==2) trace("ok 1/3");

            x.f = function() {trace("ok 2/3")};
            x.f();

            var str:String = "key";
            var y:Object = {str:{}};

            var z:Object = 
            {one: 1,two: 2,three: 3};

            if(z.two == 2) trace("ok 3/3");

            trace("[exit]");
        }
    }
}

