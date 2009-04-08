
package {
    import flash.display.MovieClip
    import flash.system.Capabilities
    public class Main extends flash.display.MovieClip {

        static var mode = 3;
        static var message="error";

        /* test code which executes directly in the class' static
           constructor */
        if(mode == 3)
            message = "ok 1/1";
        else
            message = "error";

        /* test access to other classes static fields during our init code */
        static var os;
        os = Capabilities.os;
    
        function Main() {
            trace(Main.message);
            trace("[exit]");
        }
    }
}
