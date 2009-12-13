package {
    import flash.events.*;
    import flash.utils.*;
    import flash.display.MovieClip;

    dynamic public class ExtendProxy1 extends Proxy
    {
	import flash.utils.flash_proxy;
	flash_proxy override function callProperty($name:*, ...$args:Array):* 
	{
	    trace("ok 1/2");
	}
    }
    dynamic public class ExtendProxy2 extends Proxy 
    {
	import flash.utils.*;
	flash_proxy override function callProperty($name:*, ...$args:Array):* 
	{
	    trace("ok 2/2");
	}
    }

    public class Main extends flash.display.MovieClip
    {
	public function Main()
	{
	    new ExtendProxy1().callProperty();
	    new ExtendProxy2().callProperty();
	    trace("[exit]");
	}
    }
}
