package {
    import flash.display.MovieClip

    public class Main extends flash.display.MovieClip {

        var count:int = 1;
        var num:int = 16;

        function istrue(b:Boolean) {
            if(b) {
                trace("ok "+count+"/"+num);
            } else {
                trace("error "+count+"/"+num);
            }
            count = count + 1
        }
        function isfalse(b:Boolean) {
            istrue(!b);
        }

        function Main() {
            trace("ok");
            
             istrue(1 < 2 < 3);
            isfalse(2 < 1 < 3);
            isfalse(3 < 2 < 1);
            isfalse(1 < 3 < 2);
            isfalse(2 < 3 < 1);
            isfalse(3 < 1 < 2);
             
            isfalse(1 > 2 > 3);
            isfalse(2 > 1 > 3);
             istrue(3 > 2 > 1);
            isfalse(1 > 3 > 2);
            isfalse(2 > 3 > 1);
            isfalse(3 > 1 > 2);

            trace("[exit]");
        }
    }
}

