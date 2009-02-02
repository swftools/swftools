package {
    import flash.display.MovieClip;
    public class Main extends flash.display.MovieClip {
        function Main() {
            if(true) trace("ok 1/5");
            if(false) trace("error");
            if(1+1==2) trace("ok 2/5");
            if(1+1==3) trace("error");
            if(1+1!=2) trace("error");
            if(1+2==3) trace("ok 3/5");

            if(1+1==3) {} else {trace("ok 4/5");}

            if(false) trace("error");
            else if(false) trace("error");
            else if(true) trace("ok 5/5");
            else if(false) trace("error");
            trace("[exit]");
        }
    }
}
