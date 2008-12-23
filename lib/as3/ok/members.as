package package1 {
    public class Class1 {
        public var a;
        public var b;
        public var c:int;
        public var d = 1;
        public var e:int = 1;

        function Class1() {
        }
        public function write() {
            if(a==1)
                trace("ok 1/5");
            if(b==1)
                trace("ok 2/5");
            if(c==1)
                trace("ok 3/5");
            if(d==1)
                trace("ok 4/5");
            if(e==1)
                trace("ok 5/5");
        }
    }
}
package package2 {
    import package1.Class1

    public class Main extends flash.display.MovieClip {
        public function Main() {
            var cls = new Class1();
            cls.a = 1;
            cls.b++;
            cls.c = c.a;
            cls.write();

            cls.c += 1;
            cls.c -= c.a;
        }
    }
}
