package test.package {
    public class Main extends flash.display.MovieClip {
        function Main() {
            var i:int;
            /* test breaking loop */
            for(i=0;i<100;i++) {
                var x=-1;
                if(i*i==49)
                    break;
            }
            if(i!=7) trace("error");
            else trace("ok 1/4");
           
            /* test not breaking loop */
            for(;i<10;i++) {
                var x="String";
                if(i==20)
                    break;
            }
            if(i!=10) trace("error");
            else trace("ok 2/4");
           
            /* test breaking while */
            while(i<20) {
                var x=1.0;
                if(i==15)
                    break;
                i++;
            }
            if(i!=15) trace("error");
            else trace("ok 3/4");
            
            /* test non-breaking while */
            while(i<30) {
                var x=new Object;
                if(i==40)
                    break;
                i++;
            }
            if(i!=30) trace("error");
            else trace("ok 4/4");
            
            trace("[exit]");
        }
    }
}

