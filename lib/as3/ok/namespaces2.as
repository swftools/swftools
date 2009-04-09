package {
	
	import flash.utils.*;
        import flash.display.MovieClip;
	
	use namespace flash_proxy;
	
	dynamic public class OverrideProxy extends Proxy {
            flash_proxy override function callProperty($name:*, ...$args:Array):* {
                    return none;
            }
        }
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
                trace("[exit]");
            }
	    
	}
}
