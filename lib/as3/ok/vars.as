package {
    import flash.display.MovieClip;
    public class Main extends flash.display.MovieClip {
        function Main() {
            
            var x = 3;
            if(true) {
                var x = 4;
                var y = 5;
            }
            if(   x == 4 //flex logic
               || x == 3 //as3compile logic
              ) 
                trace("ok 1/2");

            if(y==5)
                trace("ok 2/2");

            trace("[exit]");
        }
    }
}
