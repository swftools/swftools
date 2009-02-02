package {
    import flash.display.MovieClip
    public class Main extends flash.display.MovieClip {

        function get mystring(s="s\0s\xff") {
            return s;
        }

        function Main() {
            var email = "test@test.de";
            if(email.length == 12) trace("ok 1/5");
            if(email.charAt(4)!='@') trace("error");

            var x;
            var y:Boolean=false;
            for(x=0;x<email.length;x++) {
                if(email.charAt(x)=='.') {
                    y=true;
                }
            }
            if(y) trace("ok 2/5");

            var zero1 = "test\0test";
            if(zero1.charAt(4)!='\0') trace("error");
            else trace("ok 3/5");

            if(this.mystring.charAt(1)!='\0') trace("error");
            else trace("ok 4/5");
            if(this.mystring.charAt(3)!='\xff') trace("error");
            else trace("ok 5/5");
            trace("[exit]");
        }
    }
}

