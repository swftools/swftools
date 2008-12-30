package {
    import flash.display.MovieClip
    public class Main extends flash.display.MovieClip {
        function Main() {
            var x:int = 0, y:int = 0;

            for(x=0,y=0;x<10;x++,y++) {
            }
            if(y==10) trace("ok 1/2");
            else      trace("error");

            y = 0;
           
            var o,z:uint = 0;
            for(x++,y++;x<20;x++,y++) {
                z++;
            }
            if(z==9) trace("ok 2/2");
            else     trace("error");
        }
    }
}

