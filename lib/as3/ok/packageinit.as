package some.other.module {
    public var msg = "ok 2/3";

    public function getmsg() {
        return "ok 3/3";
    }
}

package {
    import flash.display.MovieClip
    import flash.system.Capabilities
    import some.other.module.*

    var mode = 3;
    var message="error";
    /* test code which executes directly in a package */
    if(mode == 3)
        message = "ok 1/1";
    else
        message = "error";

    trace(message);

    /* test access to other classes static fields during our init code */
    var os;
    os = Capabilities.os;

    trace(msg);
    trace(getmsg());
}
