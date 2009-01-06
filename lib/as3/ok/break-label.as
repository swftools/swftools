package {
    import flash.display.MovieClip
    public class Main extends flash.display.MovieClip {
        function Main() {
            var i,j,k;

            outerloop:
            for(i=0;i<100;i++) {
                var x1:String;
                for(j=0;j<100;j++) {
                    var x2:String;
                    for(k=0;k<100;k++) {
                        var x3:String;
                        trace("ok 1/2");
                        break outerloop;
                        trace("error");
                    }
                    trace("error");
                }
                trace("error");
            }
            
            for(i=0;i<100;i++) {
                var x1:String;
                midloop:
                for(j=0;j<100;j++) {
                    var x2:String;
                    for(k=0;k<100;k++) {
                        var x3:String;
                        trace("ok 2/2");
                        break midloop;
                        trace("error");
                    }
                    trace("error");
                }
                break;
                trace("error");
            }
        }
    }
}

