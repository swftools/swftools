package {
    import flash.events.Event;
    import flash.display.MovieClip

    public class Main extends flash.display.MovieClip
    {
	var vars = ["ok 1/2","ok 2/2"];
	public function Main() 
	{
	    for (var p:String in this.vars) {
		if(int(p)==0) trace(vars[p]);
	    }
	    for (p in this.vars) {
		if(int(p)==1) trace(vars[p]);
	    }
	}
    }
}
