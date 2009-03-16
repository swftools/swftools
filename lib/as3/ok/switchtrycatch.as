package {
    import flash.display.MovieClip;
    public class Main extends flash.display.MovieClip {
        function Main() {
            var x:int;
            for(x=0;x<3;x++) {
                switch(String(x*x)) {
                    case "0": 
                        trace("ok 1/5");break;
                    case "1": 
                        try {
                            trace("ok 2/5");
                            throw new Error();
                        } catch(e:Error) {
                            trace("ok 3/5");
                        }
                    case "4": 
                        trace("ok 4/5");
                    case "fallthrough": 
                        trace("ok 5/5");
                }
            }
        }
    }
}
