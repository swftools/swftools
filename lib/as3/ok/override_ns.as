package {
    import flash.events.*;
    import flash.utils.*;
    import flash.display.MovieClip;

    dynamic public class ExtendProxy extends Proxy
    {
	flash_proxy override function callProperty($name:*, ...$args:Array):* 
	{
	    trace("ok");
	}
    }

    public class Main extends flash.display.MovieClip
    {
	public function Main()
	{
	    new ExtendProxy().callProperty();
	    trace("[exit]");
	}
    }
}
