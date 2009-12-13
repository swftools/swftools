
package {

    import flash.display.MovieClip
    
    public class Main extends flash.display.MovieClip {
        public function Main() {
            if(!(this instanceof ExtendMain)) {
                var p = new ExtendMain();
		if(p instanceof ExtendMain) trace("ok");
            }
	    trace("[exit]");
        }
    }
    
    public class ExtendMain extends Main {
    }
}
