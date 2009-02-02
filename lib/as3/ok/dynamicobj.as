
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
            
            if(x["two"]==2) trace("ok 1/2");

            x.f = function() {trace("ok 2/2")};
            x.f();
            
            trace("[exit]");
        }
    }
}

