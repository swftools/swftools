
/* I can't make this work with Flash 10, with any compiler.

   Error: Error #2136: The SWF file extendclass.swf
   contains invalid data.
*/

package {

    import flash.display.MovieClip
    
    public class Main extends flash.display.MovieClip {
        function Main() {
            var p = new ExtendMain();
        }
    }
    
    public class ExtendMain extends Main {
    }
}
