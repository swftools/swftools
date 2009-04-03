package test1 {

    import flash.display.MovieClip

    var x;

    TEST::DEBUG {
        import jj.xx.jj;
        var x:int;
        x();
    }
    
    public class Main extends flash.display.MovieClip {
        function Main() {
            trace("ok");
        }

    }
}
