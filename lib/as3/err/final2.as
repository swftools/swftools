package {
    class A {
        final public function f() {
        }
    }
  
    public class Main extends A
    {
        //error: f is final:
        override public function f() {
        }
    }
}
