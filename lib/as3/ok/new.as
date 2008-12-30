package some.other.packag3 {
    public class Class4 {
        public function Class4(x=1) {
            if(x) {
                trace("ok 4/4");
                return new Class4(0);
            }
        }
    }
}

package {
    public class Class1 {
        public function Class1() {
            trace("ok 1/4");
        }
    }
    public class Class2 {
        public function Class2() {
            trace("ok 2/4");
        }
    }
    public class Class3 {
        public function Class3(x,s) {
            trace(s)
        }
    }

    import flash.display.MovieClip
    public class Main extends flash.display.MovieClip {
        public function Main() {
            new Class1()
            new Class2
            new Class3(0,"ok 3/4")
            import some.other.packag3.Class4
            new Class4();
        }
    }
}
