package some.other.packag3 {
    public class Class4 {
        public function Class4(x=1) {
            if(x) {
                trace("ok 4/9");
                new Class4(0);
            }
        }
    }
    
    public class Class9 {public function Class9() {trace("ok 9/9");}}
}

package {
    public class Class1 {
        public function Class1() {
            trace("ok 1/9");
        }
    }
    public class Class2 {
        public function Class2() {
            trace("ok 2/9");
        }
    }
    public class Class3 {
        public function Class3(x,s) {
            trace(s)
        }
    }

    public class Class5 {
        public function Class5() {
            trace("ok 5/9")
        }
    }
    public class Class6 {
        public function Class6() {
            trace("ok 6/9")
        }
    }
    public class Class7 {
        public function Class7() {
            trace("ok 7/9")
        }
    }
    public class ClassX {
        public function ClassX(x:uint) {
            trace("ok "+x+"/9")
        }
    }

    import flash.display.MovieClip
    public class Main extends flash.display.MovieClip {
        public function Main() {
            new Class1()
            new Class2
            new Class3(0,"ok 3/9")
            import some.other.packag3.Class4
            new Class4();

            var x = Class5;
            new x;
            var y:Class = Class6;
            new y;
            
            var a = [Class7, ClassX];
            new a[0];
            new a[1](8);

            new some.other.packag3.Class9;

            trace("[exit]");
        }
    }
}
