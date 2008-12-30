package {
    import flash.display.MovieClip;
    
    public class Main extends flash.display.MovieClip {
        function Main() {
            function MySuper() {
                this.ok3="ok 3/3";
            }
            function MyClass(ok1,ok2) {
                this.ok1 = ok1;
                this.ok2 = ok2;
            };
            MyClass.prototype = new MySuper;

            var m = new MyClass("ok 1/3", "ok 2/3");
            trace(m.ok1);
            trace(m.ok2);
            trace(m.ok3);
        }
    }
}
