package test1 {

    public class Main extends flash.display.MovieClip {

        const ok1:String = "ok 1/4", ok2:String = "ok 2/4";
        var ok3:String = "ok 3/4", ok4:String = "ok 4/4";

        var xx:Main=null;
        function Main() {
            trace(ok1);
            trace(this.ok2);
            trace(this.ok3);
            trace(ok4);
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
