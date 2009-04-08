package {
	
	import flash.utils.*;
        import flash.display.MovieClip;
	
	use namespace flash_proxy;
	
	dynamic public class OverrideProxy extends Proxy {
            flash_proxy override function callProperty($name:*, ...$args:Array):* {
                    var returnValue:* = _tweens[$name].apply(null, $args);
                    realign();
                    if (!isNaN(_pauseTime)) {
                            pause(); //in case any tweens were added that weren't paused!
                    }
                    return returnValue;
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
