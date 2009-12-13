package {
    import flash.display.MovieClip
    public class Main extends flash.display.MovieClip
    {
	public function Main():void
	{
	    if(ok==null) trace("ok 1/2");
	    var ok:String = "foo";
	    if(ok=="foo") trace("ok 2/2");
	    trace("[exit]");
	}
    }
}

