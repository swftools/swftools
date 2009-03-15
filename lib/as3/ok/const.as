package {
    import flash.display.MovieClip
    public class Main extends flash.display.MovieClip {
        
        const ok1:String = "ok 1/4";
        static const ok2:String = "ok 2/4";
        
        const ok3:String = "ok 3/4";
        static const ok4:String = "ok 4/4";
        
        const ok5:String = "ok 3/4";
        static const ok6:String = "ok 4/4";

        function Main() {
            trace(ok1);
            trace(ok2);
            trace(this.ok3);
            trace(Main.ok4);
            f1();
            f2();
            
            trace("[exit]");
        }
        function f0(x:Number=1000)
        {
        }
        function f1(x:String=ok5)
        {
            trace(x);
        }
        function f2(x:String=ok6)
        {
            trace(x);
        }
    }
}
