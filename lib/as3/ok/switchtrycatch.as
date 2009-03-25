package {
    import flash.display.MovieClip;
    public class Main extends flash.display.MovieClip {
        function Main() {
            var x:int;
            for(x=0;x<3;x++) {
                switch(String(x*x)) {
                    case "0": 
                        trace("ok 1/6");break;
                    case "1": 
                        try {
                            trace("ok 2/6");
                            throw new Error();
                        } catch(e:Error) {
                            trace("ok 3/6");
                        }
                    case "2": 
                        trace("ok 4/6");
                        break;
                    case "4": 
                        trace("ok 5/6");
                    case "fallthrough": 
                        trace("ok 6/6");
                }
            }
            trace("[exit]");
        }
    }
}
