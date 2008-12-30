
package {
    import flash.display.MovieClip
    public class Main extends flash.display.MovieClip {
        var count:int = 1;
        var num:int = 27;
        function assert(b:Boolean) {
            if(b) {
                trace("ok "+count+"/"+num);
            } else {
                trace("error "+count+"/"+num);
            }
            count = count + 1
        }
        function Main() {
            /* operations not tested here:
               * comparison is tested in compare.as
               * 'as' and 'is' are tested in typecast.as and extends.as
               * ++ and -- are tested in assignments.as
               * 'in' is tested by in.as
               * || and && are tested in boolvalue.as
               * <op>= is tested in assignments.as
            */

            /* test unary minus */
            assert(-(3) == -3);

            /* test tenary operator */
            assert((true?1:2) == 1);
            true?assert(1):assert(0);
            false?assert(0):assert(1);

            /* test other operators */
            trace("[arithmetric]");
            assert(3*3==9);
            assert(3/3==1);
            assert(10%4==2);
            assert(2+3==5);
            assert(2-3==-1);

            /* test not */
            trace("[not]");
            assert(!false);
            
            /* test strict equals/unequals */
            trace("[strict equals]");
            assert(3===3);
            assert(3!==4);
            assert(!(3===4));
            assert(!(3!==3));

            /* test bit operations */
            trace("[bit operations]");
            assert(!(0xaaaaaaaa & 0x55555555))
            assert((0xaa | 0x55) == 0xff);
            assert((0xff ^ 0x55) == 0xaa);
            assert((1 & ~1) == 0);
            assert((1|~1)==~0);
           
            /* test shift operations */
            trace("[shift operations]");
            assert((0xff<<8)==0xff00);
            assert((0xff>>4)==0x0f);
            assert((-1>>1)==-1);
            assert((-1>>1)==-1);
            assert((-1>>>1)>0);

            /* test typeof */
            trace("[typeof]");
            assert(typeof(3)=="number" && 
                   typeof("")=="string" &&
                   typeof(this)=="object" &&
                   typeof(undefined)=="undefined" &&
                   typeof(null)=="object" &&
                   typeof(assert)=="function" &&
                   typeof(Main)=="object");
            
            /* test void */
            trace("[void]");
            var v = void;
            assert(String(v)=="undefined");

            /* test comma */
            assert( (1,2,3,4) == 4);
        }
    }
}

