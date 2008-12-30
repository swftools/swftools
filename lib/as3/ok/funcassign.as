package {
    import flash.display.MovieClip;
    public class Main extends flash.display.MovieClip {

        function ok1() {
            trace("ok 1/3");
        }
        function ok2(a:uint) {
            if(a==2)
                trace("ok 2/3");
        }
        function ok3():Boolean {
            trace("ok 3/3");
            return true;
        }

        var a;
        var b:Function;
        var c;

        function Main() {
            this.a = ok1;
            this.a();
            this.b = ok2;
            this.b(2);
            this.c = ok3;
            if(!this.c()) trace("error");
        }
    }
}
