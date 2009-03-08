/* import1+import2: two files for testing pass 1 class import */

package import1 {
    import flash.display.MovieClip;
    
    public interface II1 {
    }
    public interface II2 {
    }
    public class CC1 implements II1 {
    }
    public class CC2 extends CC1 implements II2 {
    }

    import import2.I1
    import import2.I2
    import import2.C1
    import import2.C2

    public class Main extends flash.display.MovieClip {
        public function Main() {
            var x:C2 = new C2();
            if(x is C1) trace("ok 1/8");
            if(x is C2) trace("ok 2/8");
            if(x is I1) trace("ok 3/8");
            if(x is I2) trace("ok 4/8");
            var a:I1 = x;
            var b:I2 = x;
            var c:C1 = x;
            var d:C2 = x;
            if(x === a) trace("ok 5/8");
            if(x === b) trace("ok 6/8");
            if(x === c) trace("ok 7/8");
            if(x === d) trace("ok 8/8");
        }
    }
}
