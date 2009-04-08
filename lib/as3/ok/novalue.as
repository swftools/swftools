package {
    import flash.display.MovieClip

    public class Main extends flash.display.MovieClip {
        
        var count:int = 1;
        var num:int = 6;
        function assert(b) {
            if(b) {
                trace("ok "+count+"/"+num);
            } else {
                trace("error "+count+"/"+num);
            }
            count = count + 1
        }
        function compare(x,y) {
            assert(x==y);
        }

        function Main() {
            var x:int=0;
            var y:int=0;

            /* test for &&/|| as "inline if" */

            x++;
            assert(x==1);
            
            x++ || y++;
            assert(x==2);
            assert(y==0);
            
            x++ && y++;
            assert(x==3);
            assert(y==1);

            false && y++;
            assert(y==1);

            trace("[exit]");
        }

    }
}

