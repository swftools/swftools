package {
	
	import flash.utils.*;
        import flash.display.MovieClip;
	
	use namespace flash_proxy;

	
	dynamic public class OverrideProxy extends Proxy {
            flash_proxy override function callProperty($name:*, ...$args:Array):* {
                    return null;
            }
        }

	var ns2:Namespace = flash_proxy;

	dynamic public class Main extends flash.display.MovieClip {
		
	    flash_proxy function tf(x:int):*
	    {
                trace("ok "+x+"/3");
		return null;
	    }


            public function Main()
            {
                this.tf(1);
                tf(2);
                var ns:Namespace = flash_proxy;

		if(ns == ns2) 
		    trace("ok 3/3");

                trace("[exit]");
            }
	    
	}
}
