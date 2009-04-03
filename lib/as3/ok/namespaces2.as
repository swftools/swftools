package {
	
	import flash.utils.*;
        import flash.display.MovieClip;
	
	use namespace flash_proxy;
	
	/**
	 * This class acts as the public interface for both the flash api and the javascript api
	 * It is a proxy class that wraps ApiInternal. It provides a method interface, as well as
	 * exposing an event interface to flash. 
	 */
	dynamic public class Main extends flash.display.MovieClip {
		
	    flash_proxy function tf(x:int):*
	    {
                trace("ok "+x+"/2");
		return null;
	    }

            public function Main()
            {
                this.tf(1);
                tf(2);
            }
	    
	}
}
