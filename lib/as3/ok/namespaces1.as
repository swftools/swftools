package my.namespaces { 
    public namespace ns1 = "http://www.some.other/namespace";
    public namespace ns2 = "http://www.swftools.org/";
    public namespace ns3 = "http://yet.some.other/namespace";
    public namespace ns4 = "bla bla";
    
    public namespace ns5 = "bla bla 2";

    public class Test {

        ns1 static function test() {
            trace("error");
        }
        ns2 static function test() {
            trace("ok 2/6");
            use namespace my.namespaces.ns2;
            (new Test()).test2()
        }
        ns3 static function test() {
            trace("error");
        }

        ns1 function test2() {trace("error");}
        ns2 function test2() {trace("ok 3/6");}
        ns3 function test2() {trace("error");}

        ns4 static function test3() {
            trace("ok 4/6");
        }

        ns1 function test4() {trace("ok 5/6");}
        ns2 function test4() {trace("ok 6/6");}
    }
}
    
package {

    import flash.display.MovieClip
    import flash.utils.Proxy
    import flash.utils.flash_proxy

    use namespace flash.utils.flash_proxy;
    
    public namespace ns4clone = "bla bla";
    use namespace ns4clone;
    
    public class Main extends flash.display.MovieClip {

        use namespace my.namespaces.ns2;
        import my.namespaces.*;
        use namespace ns5;

        private function test1() {
            my.namespaces.Test.test()
        }

        public function Main() 
        {
            var p:Proxy = new Proxy
            try {
                p.hasProperty("test")
            } catch(e:Error) {
                // Proxy is similar to an interface, and will throw some kind
                // of "class not implemented" exception
                trace("ok 1/6");
            }
            my.namespaces.Test.test()
            my.namespaces.Test.test3()

            use namespace my.namespaces.ns1;
            use namespace my.namespaces.ns2;
            var test:Test = new Test();
            test.ns1::test4();
            test.ns2::test4();

            trace("[exit]");
        }
    }
}

