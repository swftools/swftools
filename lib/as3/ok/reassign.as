package {
    import flash.display.MovieClip

    public class Main extends flash.display.MovieClip {

        function Main() {
            
            var a = [0,1];
            var i = 0;
            
            a[int(i++)] += 1;

            if(a[0]==1) trace("ok 1/2");
            else        trace("error");
            if(a[1]==1) trace("ok 2/2");
            else        trace("error");
        }
    }
}

