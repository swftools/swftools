package {
    import flash.display.MovieClip;
    public class Main extends flash.display.MovieClip {

        function ok1():Boolean {trace("ok 7/16");return true;}
        function ok2():Boolean {trace("ok 9/16");return true;}
        function ok3():Boolean {trace("ok 13/16");return true;}
        function ok4():Boolean {trace("ok 15/16");return true;}

        function error():Boolean {trace("error");return true;}

        function Main() {
            trace(false || "ok 1/16");
            trace("ok 2/16" || "error");

            if(false && true) trace("error") else trace("ok 3/16");
            trace("error" && "ok 4/16");

            if(!false) trace("ok 5/16");

            if(!true) trace("error");
            else      trace("ok 6/16");

            /* test left associativity for && */
            if(true && true && ok1()) trace("ok 8/16");
            if(false && true && error()) trace("error");
            if(true && false && error()) trace("error");
            if(true && ok2() && false) trace("error1");
            if(false && error() && true) trace("error2");

            if(false || false || true || error()) trace("ok 10/16");
            
            /* test left associativity for || */
            if(true || false || error()) trace("ok 11/16");
            if(true || error() || false) trace("ok 12/16");
            if(ok3() || true || true) trace("ok 14/16");
            if(false || false || ok4()) trace("ok 16/16");
        }
    }
}
