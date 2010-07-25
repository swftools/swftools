package
{
    import flash.display.MovieClip;
    import flash.events.Event;
    import flash.system.Security;
    import flash.utils.ByteArray;

	public final class Main extends MovieClip
	{
		public function Main()
		{
			try_catch1();
		}
		
		public static function try_catch1():void
		{
			var keyStr:String = "";
			var valueStr:String = "";
			try {
				var key:ByteArray = new ByteArray();
			} catch (e:Error) {
			}
			trace("ok 1/1");
		}
	}
}
		
