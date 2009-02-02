package {
    import flash.display.MovieClip
    public class Main extends flash.display.MovieClip {

        var o; //undefined
        var s:String; //null
        var b:Boolean; //false
        var i:int; //0
        var u:uint; //0
        var f:Number; //NaN
        var m:Main; //null

        public function Main() {
            var o; //undefined
            var s:String; //null
            var b:Boolean; //false
            var i:int; //0
            var u:uint; //0
            var f:Number; //NaN
            var m:Main; //null

            if(String(o)=="undefined") trace("ok 1/14");
            if(s===null) trace("ok 2/14");
            if(b===false) trace("ok 3/14");
            if(i==0) trace("ok 4/14");
            if(u==0) trace("ok 5/14");
            if(String(f)=="NaN") trace("ok 6/14");
            if(m===null) trace("ok 7/14");
            
            if(String(this.o)=="undefined") trace("ok 8/14");
            if(this.s===null) trace("ok 9/14");
            if(this.b===false) trace("ok 10/14");
            if(this.i==0) trace("ok 11/14");
            if(this.u==0) trace("ok 12/14");
            if(String(this.f)=="NaN") trace("ok 13/14");
            if(this.m===null) trace("ok 14/14");
            
            trace("[exit]");
        }
    }
}
