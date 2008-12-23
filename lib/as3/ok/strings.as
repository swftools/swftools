package {
    import flash.display.MovieClip
    public class Main extends flash.display.MovieClip {
        function Main() {
            var email = "test@test.de";
            if(email.length == 12) trace("ok 1/2");
            if(email.charAt(4)!='@') trace("error");
            var x;
            var y:Boolean=false;
            for(x=0;x<email.length;x++) {
                if(email.charAt(x)=='.') {
                    y=true;
                }
            }
            if(y) trace("ok 2/2");
        }
    }
}

