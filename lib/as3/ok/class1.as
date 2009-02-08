package test1 {

    public class Main extends flash.display.MovieClip {
        var x:Main=null;
        function Main() {
            trace("ok");
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
