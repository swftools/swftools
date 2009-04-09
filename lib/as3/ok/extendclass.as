
package {

    import flash.display.MovieClip
    
    public class Main extends flash.display.MovieClip {
        function Main() {
            if(!this instanceof ExtendMain) {
                /* If we don't check what class we're in, we get a infinite
                   recursion, and flash player outputs this message:
                   Error: Error #2136: The SWF file extendclass.swf
                   contains invalid data.
                */
                var p = new ExtendMain();
            }
        }
    }
    
    public class ExtendMain extends Main {
    }
}
