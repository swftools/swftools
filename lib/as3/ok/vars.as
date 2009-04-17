package {
    import flash.display.MovieClip;
    public class Main extends flash.display.MovieClip {

        function test_overload() {
            var x = 3;
            if(true) {
                var x = 4;
                var y = 5;
            }
            if(   x == 4 //flex logic
               || x == 3 //as3compile logic
              ) 
                trace("ok 1/3");

            if(y==5)
                trace("ok 2/3");
        }

        function test_global() {
            var l = 0;
            do {
                var g:int = l++;
            } while(g<10);
            if(l==11) {
                trace("ok 3/3");
            } else {
                trace("error 3/3");
            }
        }

        function Main() {

            test_overload();
            test_global();

            trace("[exit]");
        }
    }
}
