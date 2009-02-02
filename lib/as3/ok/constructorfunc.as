package {
    import flash.display.MovieClip;
    
    public class Main extends flash.display.MovieClip {
        function Main() {
            function FooBar(x,y) {
                this.x = x;
                this.y = y;
                this.ok4 = "ok 4/5";
                this.f1 = function() {
                    trace(this.ok4);
                }
            };
            FooBar.prototype.z = "ok 3/5";
            FooBar.prototype.f2 = function () {
                trace(this.ok5);
            }
                
            var foobar = new FooBar("ok 1/5", "ok 2/5");
            foobar.ok5 = "ok 5/5";
            trace(foobar.x);
            trace(foobar.y);
            trace(foobar.z);
            foobar.f1();
            foobar.f2();
            
            trace("[exit]");
        }
    }
}
