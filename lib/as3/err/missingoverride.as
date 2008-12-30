package I {
    public interface I1 {
        function f1()
        function f2()
    }
    public class C {
        function f1() {}
        //fail to override f2
    }
}
