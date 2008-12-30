
package {
    import flash.display.MovieClip;

    public class Main extends flash.display.MovieClip {
        function Main() {
            var a:Array = new Array(1,2,3,4);
            if(a[0]==1) trace("ok 1/4");
            if(a[1]==2) trace("ok 2/4");
            if(a[2]==3) trace("ok 3/4");
            if(a[3]==4) trace("ok 4/4");
        }
    }
}
