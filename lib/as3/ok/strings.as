package {
    import flash.display.MovieClip
    public class Main extends flash.display.MovieClip {

        function get mystring(s="s\0s\xff") {
            return s;
        }

        function Main() {
            var email = "test@test.de";
            if(email.length == 12) trace("ok 1/6");
            if(email.charAt(4)!='@') trace("error");

            var x;
            var y:Boolean=false;
            for(x=0;x<email.length;x++) {
                if(email.charAt(x)=='.') {
                    y=true;
                }
            }
            if(y) trace("ok 2/6");

            var zero1 = "test\0test";
            if(zero1.charAt(4)!='\0') trace("error");
            else trace("ok 3/6");

            if(this.mystring.charAt(1)!='\0') trace("error");
            else trace("ok 4/6");
            if(this.mystring.charAt(3)!='\xff') trace("error");
            else trace("ok 5/6");

            if("\w" == "\\w")
                trace("ok 6/6");


            trace("[exit]");
        }
    }
}

