
package {
    import flash.display.MovieClip

    public class Main extends flash.display.MovieClip {
        public function Main() {
            /* test matching */
            var r:RegExp = /ok \d\/\d/;
            trace("ok 1/7".match(r));

            /* test searching */
            var s:String = "error OK 9/7 ok 2/7"
            trace(s.substring(s.search(r)));
            
            /* test /.../i syntax */
            var r2:RegExp = /ok \d\/\d/i;
            var s2:String = "error OK 3/7"
            var s3:String = "error ok 3/7"
            trace(s3.substring(s2.search(r2)));

            /* test \n,\r,\t */
            if(" .\n\r\t".search(/[\n][\r][\t]/) == 2) trace("ok 4/7");

            /* test * at end of regexp */
            if("  xxx  ".search(/xx*/) == 2) trace("ok 5/7");
            
            /* test quotes */
            if(" \" ".search(/"/) == 1) trace("ok 6/7");
            if(' \' '.search(/'/) == 1) trace("ok 7/7");
            
            trace("[exit]");
        }
    }
}

