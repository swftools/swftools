package {
    import flash.display.MovieClip
    public class Main extends flash.display.MovieClip {

        var ci:int = 0;

        function Main() {
            var a:Array = new Array(1,2,3,4,6,5,7,8,9,10);
            var j:int = 0;
            for(var x in a) {
                j += x;
            }
            if(j==45) trace("ok 1/6");
           
            var y;
            j = 0;
            for(y in a) {
                j += y;
            }
            if(j==45) trace("ok 2/6");
            
            j = 0;
            for each(var x in a) {
                j += x;
            }
            if(j==55) trace("ok 3/6");
            
            var z = 0;
            j = 0;
            for each(z in a) {
                j += z;
            }
            if(j==55) trace("ok 4/6");
            
            var i:int = 0;
            j = 0;
            for each(i in a) {
                j += i;
                if(i==6)
                    break;
                else
                    continue;
            }
            if(j==16) trace("ok 5/6");

            j = 0;
            for each(ci in a) {
                j += ci;
            }
            if(j == 55) trace("ok 6/6");

            trace("[exit]");
        }
    }
}

