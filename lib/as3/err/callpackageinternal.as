package some.other.module {
    function getmsg() {
        trace("error");
    }
}

package {
    import some.other.module.*;
    getmsg();
}
