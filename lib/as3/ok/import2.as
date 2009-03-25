package import2 {
    import flash.display.MovieClip;
    
    public interface I1 {
    }
    public interface I2 {
    }
    public class C1 implements I1 {
    }
    public class C2 extends C1 implements I2 {
    }
    
    import import1.II1
    import import1.II2
    import import1.CC1
    import import1.CC2

    public class Main extends flash.display.MovieClip {
        public function Main() {
            var x:CC2 = new CC2();
            if(x is CC1) trace("ok 1/8");
            if(x is CC2) trace("ok 2/8");
            if(x is II1) trace("ok 3/8");
            if(x is II2) trace("ok 4/8");
            var a:II1 = x;
            var b:II2 = x;
            var c:CC1 = x;
            var d:CC2 = x;
            if(x === a) trace("ok 5/8");
            if(x === b) trace("ok 6/8");
            if(x === c) trace("ok 7/8");
            if(x === d) trace("ok 8/8");
        }
    }
}
