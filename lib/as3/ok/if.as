package test.mypackage {
    import flash.display.MovieClip;
    public class Main extends flash.display.MovieClip {
        function Main() {
            if(true) trace("ok 1/4");
            if(false) trace("error");
            if(1+1==2) trace("ok 2/4");
            if(1+1==3) trace("error");
            if(1+1!=2) trace("error");
            if(1+2==3) trace("ok 3/4");

            if(1+1==3) {} else {trace("ok 4/4");}
        }
    }
}
