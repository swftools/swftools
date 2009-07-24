
package {
    import flash.display.MovieClip
    public class Main extends flash.display.MovieClip {
        var count:int = 1;
        var num:int = 30;
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
               * typeof is tested in typeof.as
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

	    /* test or */
	    var y = 0;
	    var x = y || 1;
	    assert(x);
	    var z = x && 1;
	    assert(z);

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
            assert(!(0xaaaaaa & 0x555555))
            assert((0xaa | 0x55) == 0xff);
            assert((0xff ^ 0x55) == 0xaa);
            assert((1 & ~1) == 0);
            assert((1|~1)==~0);
            
            assert((1|2|4|8|16|32|64|128) == 0xff);
           
            /* test shift operations */
            trace("[shift operations]");
            assert((0xff<<8)==0xff00);
            assert((0xff>>4)==0x0f);
            assert((-1>>1)==-1);
            assert((-1>>1)==-1);
            assert((-1>>>1)>0);
 
            /* test void */
            trace("[void]");
            var v = void;
            assert(String(v)=="undefined");
            v = void 3;
            assert(String(v)=="undefined");

            /* test comma */
            trace("[comma]");
            assert( (1,2,3,4) == 4);
            
            trace("[exit]");
        }
    }
}

