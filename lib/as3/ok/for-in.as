package {
    import flash.display.MovieClip
    public class Main extends flash.display.MovieClip {
        function Main() {
            var a:Array = new Array(1,2,3,4,6,5,7,8,9,10);
            var j:int = 0;
            for(var x in a) {
                j += x;
            }
            if(j==45) trace("ok 1/4");
           
            var y;
            j = 0;
            for(y in a) {
                j += y;
            }
            if(j==45) trace("ok 2/4");
            
            j = 0;
            for each(var x in a) {
                j += x;
            }
            if(j==55) trace("ok 3/4");
            
            var y = 0;
            j = 0;
            for each(y in a) {
                j += y;
            }
            if(j==55) trace("ok 4/4");
        }
    }
}

