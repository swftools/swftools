import flash.system.Capabilities

trace("ok 1/3");
if(Capabilities.os is String) {
    trace("ok 2/3");
}
function f()
{
    trace("ok 3/3");
}
f()
