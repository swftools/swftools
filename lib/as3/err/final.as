package {
    final class A {
        function A() {
        }
    }
  
   //error: A is final:
    public class Main extends A
    {
    }
}
