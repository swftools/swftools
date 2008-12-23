package package1 {
    public class Class1 {
        public function Class1() {
            trace("ok 1/2");
        }
    }
package package2 {
    public class Class2 {
        public function Class4() {
            trace("ok 2/2");
        }
    }
}

package {
    import package1.Class1
    import package2.*

    public class Main extends flash.display.MovieClip {
        public function TestImport() {
            new Class1();
            new Class2();
        }
    }
}
