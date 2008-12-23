package some.other.package {
    public class Class4 {
        public function Class4() {
            trace("ok 4/4");
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

    public class Main extends flash.display.MovieClip {
        public function Main() {
            new Class1()
            new Class2
            new Class3(0,"ok 3/3")
            import some.other.package.Class4
            new Class4();
        }
    }
}
