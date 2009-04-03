package {

    import flash.display.MovieClip
    
    public class B extends C {
        static var x:String = "error";

        function f() {
            /* the compiler should prefer the superclass x
               over the static x */
            return x;
        }
    }
    
    public class C extends D {
    }
    
    public class D {
        var x:String = "ok";
    }
    
    
    public class Main extends flash.display.MovieClip {
        function Main() {
            trace((new B()).f());
        }
    }
    
}
