package package1 {
    public class Class1 {
        public function Class1() {
            trace("ok 1/3");
        }
    }
}
package package2 {
    public class Class2 {
        public function Class2() {
            trace("ok 2/3");
        }
    }
}

package {
    import package1.Class1
    import package2.*
    import flash.events.TextEvent

    public class Main extends flash.display.MovieClip {
        public function Main() {
            new Class1();
            new Class2();
            var t = new TextEvent("type",true,true,"ok 3/3");
            trace(t.text);
        }
    }
}
