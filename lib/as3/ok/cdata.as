package {
    import flash.display.MovieClip
    
    public class Main extends flash.display.MovieClip {
       
	function test()
	{ 
	    return (<![CDATA[[0][]]<]>]]].]].]>.]]]>).toString();
	}

        function Main() {
	    if(this.test() == "[0][]]<]>]]].]].]>.]")
		trace("ok");
	    else
		trace("error");
            trace("[exit]");
        }
    }
}
