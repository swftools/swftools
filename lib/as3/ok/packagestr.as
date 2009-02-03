package {
    import flash.display.MovieClip
    import flash.utils.*

    public class Main extends flash.display.MovieClip {
        function Main() {
            /* static class member, with full path */
            var x:String = flash.events.DataEvent.DATA;
            if(x == "data")
                trace("ok 1/4");

            var flash:Object = new Object();
            flash["events"] = new Object();

            import flash.events.DataEvent
            //trace(flash.events); should fail

            /* class, with full path */
            var y = new flash.events.DataEvent("");
            if(y is flash.events.Event)
                trace("ok 2/4");

            /* package variable, with full path */
            var z = flash.utils.flash_proxy;
            if(z is Namespace)
                trace("ok 3/4");

            /* package method, with full path */
            var t = flash.utils.getTimer();
            if(t is int)
                trace("ok 4/4");

            trace("[exit]");
        }
        function test2() {
            /* this test breaks in Flex */
            var flash:Object = new Object();
            flash["events"] = new Object();
            trace(flash.events); //should work- no active import
        }
    }
}
