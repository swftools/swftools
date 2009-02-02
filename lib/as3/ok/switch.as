package {
    import flash.display.MovieClip;
    public class Main extends flash.display.MovieClip {
        function Main() {
            var x:int;
            for(x=0;x<5;x++) {
                switch(String(x*x)) {
                    case "0": trace("ok 1/5");break;
                    case "1": trace("ok 2/5");break;
                    case "2": trace("error");break;
                    case "3": trace("error");break;
                    case "4": trace("ok 3/5");break;
                    case "9": //fallthrough
                    case "10":
                    trace("ok 4/5");break;
                    default:
                    trace("ok 5/5");break;
                }
            }

            switch(1) {
                case 2: trace("error");break;
            }
            
            switch(1) {
                case 10:
            }
            
            switch(1) {
                case 1: late1 = false; break;
            }
            trace("[exit]");
        }
        var late1:Boolean;
    }
}
