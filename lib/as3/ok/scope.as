package some.multilevel.package {
    public class ClassInMultiLevelPackage {
    }
}
package some.other.multilevel.package {
    import some.multilevel.package.*
    public class ClassInOtherMultiLevelPackage extends ClassInMultiLevelPackage {
    }
}

package I {
    public interface I1 {
    }
    public interface I2 {
    }
    public interface I3 {
    }
}
package A {

    class A1 {
    }
    class A2 extends A1{
    }
    public class A3 extends A2 {
    }

}
package B {
    class B1 extends A.A3 implements I.I1,I.I2,I.I3{
    }
}
package C {
    public class Main extends flash.display.MovieClip {
        function Main() {
            trace("ok");
            trace("[exit]");
        }
    }
}
