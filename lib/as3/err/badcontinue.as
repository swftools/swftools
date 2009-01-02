package {
    import flash.display.MovieClip
    public class Main extends flash.display.MovieClip {
        function Main() {
            var i:int;
            
            /* test for loop */
            var j:int=0;
            for(i=0;i<100;i++) {
                j++;
                if(i!=50)
                    continue badname;
                trace("ok 1/6");
            }
        }
    }
}

