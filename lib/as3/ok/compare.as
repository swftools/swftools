package test.package {
    public class Main extends flash.display.MovieClip {
        function Main() {
            var a=3;
            var b=10;
            var c=15;
            var d=10;

            if(a<b) trace("ok 1/18");else trace("error");
            if(b<c) trace("ok 2/18");else trace("error");
            if(a<c) trace("ok 3/18");else trace("error");
            
            if(b==d) trace("ok 4/18");else trace("error");
            if(b>=d) trace("ok 5/18");else trace("error");
            if(b<=d) trace("ok 6/18");else trace("error");
            
            if(a<=b) trace("ok 7/18");else trace("error");
            if(b<=c) trace("ok 8/18");else trace("error");
            if(a<=c) trace("ok 9/18");else trace("error");
            
            if(b>=a) trace("ok 10/18");else trace("error");
            if(c>=b) trace("ok 11/18");else trace("error");
            if(c>=a) trace("ok 12/18");else trace("error");
            
            if(b>a) trace("ok 13/18");else trace("error");
            if(c>b) trace("ok 14/18");else trace("error");
            if(c>a) trace("ok 15/18");else trace("error");
            
            if(b<d) trace("error");else trace("ok 16/18");
            if(b>d) trace("error");else trace("ok 17/18");

            trace("ok 18/18");
        }
    }
}

