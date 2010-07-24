package {
    import flash.events.Event;
    import flash.display.MovieClip

    public class Main extends flash.display.MovieClip
    {
		public function test(s:String, f)
		{
			if(f!=null)
				trace("ok 2/3");
			f(null);
		}

		public function Main() 
		{
			if(typeof(x) == "number")
				trace("ok 1/3");

			test( "resize", function (e:Event):void {
				trace("ok 3/3");
			});

			var x:int = 3;
		}
	}
}
