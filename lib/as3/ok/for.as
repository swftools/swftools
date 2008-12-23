
package {
    import flash.display.MovieClip
    public class Main extends flash.display.MovieClip {
        function Main() {
            var a1 = 0;
            for(a1=0;a1<100;a1++) {
            }
            if(a1==100) trace("ok 1/4");
            else        trace("error");

            var a3:int;
            for(a3=0;a3<100;a3++) {
                a1++;
            }
            if(a1==200) trace("ok 2/4");
            else        trace("error");

            if(a3==100) trace("ok 3/4");
            else        trace("error");
            
            for(var a2=0;a2<99;a2++) {
                if(a2==80) trace("ok 4/4");
            }
        }
    }
}

