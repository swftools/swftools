
package {
    import flash.display.MovieClip;

    public class Main extends flash.display.MovieClip {
        function Main() {
            var a:Array = new Array(1,2,3);
            if(a[0]==1) trace("ok 1/10");
            if(a[1]==2) trace("ok 2/10");
            if(a[2]==3) trace("ok 3/10");
            if(a.length==3) trace("ok 4/10");

            var b:Array = [1,2,3];
            if(b[0]==1) trace("ok 5/10");
            if(b[1]==2) trace("ok 6/10");
            if(b[2]==3) trace("ok 7/10");
            if(b.length==3) trace("ok 8/10");
          
            // test for ]+<int> parser bug:
            var check = (3 == a[0]+1);

	    var list:Array = ["ok 9/10", "ok 10/10"];
	    for each(var s:String in list) {
		trace(s);
	    }                    

            trace("[exit]");
        }
    }
}
