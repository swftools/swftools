
package {
    import flash.display.MovieClip;

    public class Main extends flash.display.MovieClip {
        function Main() {
            var a:Array = new Array(1,2,3);
            if(a[0]==1) trace("ok 1/8");
            if(a[1]==2) trace("ok 2/8");
            if(a[2]==3) trace("ok 3/8");
            if(a.length==3) trace("ok 4/8");

            var b:Array = [1,2,3];
            if(b[0]==1) trace("ok 5/8");
            if(b[1]==2) trace("ok 6/8");
            if(b[2]==3) trace("ok 7/8");
            if(b.length==3) trace("ok 8/8");
          
            // test for ]+<int> parser bug:
            var check = (3 == a[0]+1);
            
            trace("[exit]");
        }
    }
}
