package {
    import flash.display.MovieClip
    import flash.utils.ByteArray

    public interface I10 {}
                          public interface I2 extends I10,I1 {}
               public interface I8            extends I10,I3 {}
    public interface I9 extends I10,I8 {}
                          public interface I3 extends I10,I1 {}
                          public interface I5 extends I10,I1 {}
               public interface I7 extends I10,I5 {}
               public interface I6 extends I10,I5 {}
               public interface I4 extends I10,I2 {}
                                     public interface I1 {}


                          public class C2 extends C1 implements I3 {}
               public class C8            extends C3 implements I4 {}
    public class C9 extends C8 implements I5,I2  {}
                          public class C3 extends C1 implements I8, I1 {}
                          public class C5 extends C1 implements I9, I5, I6 {}
               public class C7 extends C5 implements I3 {}
               public class C6 extends C5 implements I4 {}
               public class C4 extends C2 implements I1, I4{}
                                     public class C1 implements I8, I2 {}

    public class Main extends MovieClip {
        function Main() {
            var c:C9 = new C9;
            trace("ok");
            trace("[exit]");
        }
    }
}
