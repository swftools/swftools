package {
    import flash.display.MovieClip
    
    public class C {
        public function toString() {
            return "ok 7/7";
        }
    }
    
    public class D extends C {
    }
    
    public class E extends D {
    }

    public class Main extends flash.display.MovieClip {
        function Main() {
            var e = new E;
            
            if(e is E) trace("ok 1/7");
            C(e);
            var d:D = e as D;
            if(e is E) trace("ok 2/7");
            var c:C = C(e)
            if(e is E) trace("ok 3/7");

            var x = int(4.93);
            if(x==4) trace("ok 4/7");

            if(int(true)==1) trace("ok 5/7");
            if(int(false)==0) trace("ok 6/7");

            trace(String(new C)) //will call toString()
        }
    }
}
