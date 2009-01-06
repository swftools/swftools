/* same as finally.as, only for larger finally blocks which require a
   lookupswitch */

package {
    import flash.display.MovieClip;

    public class Main extends flash.display.MovieClip {
        var i=0;
        function test_return():int {
            try {
                if(i==0) return 0;
                if(i==1) return 2;
                if(i==2) return 4;
                if(i==3) return 7;
                if(i==4) return 9;
                trace("error");
            } finally {
                i = 0;
                trace("ok","1/11");
            }
            trace("error");
            return 0;
        }
       
        function test_break() {
            do {
                try {
                    if(i==0) break;
                    trace("error");
                    if(i==3) break;
                    if(i==7) break;
                    if(i==9) break;
                    if(i==11) break;
                    trace("error");
                } finally {
                    i = 0;
                    trace("ok","2/11");
                }
                trace("error");
            } while(true);
        }
        
        function test_fallthrough() {
            try {
                var x = 1+1;
                if(i==1) return;
                if(i==2) return;
                if(i==3) return;
                if(i==4) return;
                if(i==5) return;
            } finally {
                i = 0;
                trace("ok","3/11");
            }
        }
       
        function test_exception() {
            try {
                try {
                    if(i==0)
                        throw new Error();
                    if(i==1)
                        return;
                    if(i==2)
                        return;
                    if(i==3)
                        throw new Object();
                    trace("error");
                } finally {
                    i = 0;
                    trace("ok","4/11");
                }
                trace("error");
            } catch(e:Error) {
                i = 0;
                trace("ok","5/11");
            }
        }
        
        function test_exception2() {
            var x:int = 0;
            try {
                if(i==1)
                    return;
                if(i==0)
                    throw new Error();
                if(i==3)
                    throw new Error();
                trace("error");
            } catch(e:Error) {
                x=1;
            } finally {
                new Array(1,2,3,4,5,6);
                i = 0;
                x*=2;
            }
            if(x==2)
                trace("ok 6/11");
        }
        
        function fail1() {
            try {
                if(i==0) throw new Error();
                if(i==7) throw new Error();
                if(i==11) throw new Error();
                if(i==15) throw new Error();
                if(i==17) throw new Error();
            } finally {
                new Array(1,2,3,4,5,6);
                i = 0;
                trace("ok","7/11");
            }
        }
        
        function fail2() {
            try {
                fail1();
            } finally {
                new Array(1,2,3,4,5,6);
                i = 0;
                trace("ok","8/11");
            }
        }
        
        function test_exception3() {
            try {
                fail2();
            } catch(e:Error) {
                trace("ok 9/11");
            }
        }
        
        function test_empty() {
            try {
            } finally {
            }
            trace("ok 10/11");
        }

        function test_mixed():int {
            while(true) {
                try {
                    if(i==3) continue;
                    if(i==2) throw new Error();
                    if(i==1) return 1;
                    if(i==0) break;
                } finally {
                    trace("ok","11/11");
                    i = 0;
                }
            }
            return 0;
        }

        function Main() {
            test_return();
            test_break();
            test_fallthrough();
            test_exception();
            test_exception2();
            test_exception3();
            test_empty();
            test_mixed();
        }
    }
}
