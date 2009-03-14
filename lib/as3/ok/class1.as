package test1 {

    import flash.display.MovieClip
    
    public class Test {
        protected var num6:Number = 0;
        
        public function g()
        {
            trace("ok "+num6+"/6");
        }
    }
    public class ExtendTest extends Test {
        public function f() 
        {
            // test super
            super.num6 = 6;
        }
    }

    public class Main extends flash.display.MovieClip {

        const ok1:String = "ok 1/6", ok2:String = "ok 2/6";
        var ok3:String = "ok 3/6", ok4:String = "ok 4/6";
        var num5:Number = 4;

        var xx:Main=null;
        function Main() {
            trace(ok1);
            trace(this.ok2);
            trace(this.ok3);
            trace(ok4);
            
            num5++;
            trace("ok "+num5+"/6");
            
            var p = new ExtendTest();
            p.f();
            p.g();

            trace("[exit]");
        }

    }
    /* explicitly internal */
    internal class InternalClass {
    }
    /* implicitly internal */
    class ImplicitInternalClass {
    }
   
}
package test2 {
    public class PublicClass extends test1.Main {
    }
}
