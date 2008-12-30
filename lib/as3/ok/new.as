package some.other.packag3 {
    public class Class4 {
        public function Class4(x=1) {
            if(x) {
                trace("ok 4/6");
                return new Class4(0);
            }
        }
    }
}

package {
    public class Class1 {
        public function Class1() {
            trace("ok 1/6");
        }
    }
    public class Class2 {
        public function Class2() {
            trace("ok 2/6");
        }
    }
    public class Class3 {
        public function Class3(x,s) {
            trace(s)
        }
    }

    public class Class5 {
        public function Class5() {
            trace("ok 5/6")
        }
    }
    public class Class6 {
        public function Class6() {
            trace("ok 6/6")
        }
    }

    import flash.display.MovieClip
    public class Main extends flash.display.MovieClip {
        public function Main() {
            new Class1()
            new Class2
            new Class3(0,"ok 3/6")
            import some.other.packag3.Class4
            new Class4();

            var x = Class5;
            new Class5;
            var y:Class = Class6;
            new Class6();
        }
    }
}
