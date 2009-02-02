
package {
    import flash.display.MovieClip
    public class Main extends flash.display.MovieClip {

        function Main() {
            var ok1 = "ok \
1/5";
            var ok2 = "ok \x32/\x35";
            var ok3 = "ok \u0033/\u0035";
            trace(ok1);
            trace(ok2);
            trace(ok3);
            var x = "\uFFED\uFFEE"
            if(x.charAt(1) == "\uFFEE") {
                trace("ok 4/5");
            }

            var y = "\n\r\t\\\7";
            if(y.charAt(0) == "\n" &&
               y.charAt(1) == "\r" &&
               y.charAt(2) == "\t" &&
               y.charAt(3) == "\\" &&
               y.charAt(4) == "\7")
            trace("ok 5/5")
            trace("[exit]");
        }
    }
}

