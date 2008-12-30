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
                    continue;
                trace("ok 1/6");
            }
            if(j==100)
                trace("ok 2/6");
           
            /* test while loop */
            while(i++<200) {
                j++;
                if(i!=150)
                    continue;
                trace("ok 3/6");
            }
            if(j==200)
                trace("ok 4/6");
            
            /* test do-while loop */
            
            do {
                j++;
                if(i!=250)
                    continue;
                trace("ok 5/6");
            } while(i++<300);

            if(j==300)
                trace("ok 6/6");
        }
    }
}

