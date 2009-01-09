package {
    import flash.display.MovieClip
    import flash.system.Capabilities

    static var mode = 3;
    static var message="error";
    /* test code which executes directly in a package */
    if(mode == 3)
        message = "ok 1/1";
    else
        message = "error";

    trace(message);

    /* test access to other classes static fields during our init code */
    static var os;
    os = Capabilities.os;
}
