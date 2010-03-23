package {
    import flash.events.Event;
    import flash.display.MovieClip

    public class Main extends flash.display.MovieClip
    {
	public function test1(e:Event=null)
	{
	    try {
		var a:Array = new Array
		throw new Error();
	    } catch (e:Error) { 
		if(e instanceof Error)
		    trace("ok 1/4");
	    }
	    var b = a[0]; // to trigger forward variable resolving
	}
	public function test2()
	{
	    var e:Event;
	    try {
		var a:Array = new Array
		throw new Error();
	    } catch (e:Error) { 
		if(e instanceof Error)
		    trace("ok 2/4");
	    }
	    var b = a[0]; // to trigger forward variable resolving
	}

	public function test3()
	{
	    if(x == null)
		trace("ok 3/4");

	    var x:Error;
	    try {
		if(true) throw new Error();
	    } catch (x:Error) { 
		trace("ok 4/4");
	    }
	}

	public function Main() {
	    test1();
	    test2();
	    test3();
	    trace("[exit]");
	}
    }
}
