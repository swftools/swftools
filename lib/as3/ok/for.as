
package {
    import flash.display.MovieClip
    public class Main extends flash.display.MovieClip {
        function Main() {
            var a1 = 0;
            for(a1=0;a1<100;a1++) {
            }
            if(a1==100) trace("ok 1/4");
            else        trace("error");

            if(a1<50) trace("error");
            else      trace("ok 2/4");

            if(a1<150) trace("ok 3/4");
            else       trace("error");
            
            for(var a2=0;a2<99;a2++) {
            }
            if(a2==99) trace("ok 4/4");
        }
    }
}

