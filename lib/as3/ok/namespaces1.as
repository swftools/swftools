package my.namespaces { 
    public namespace ns1 = "http://www.some.other/namespace";
    public namespace ns2 = "http://www.swftools.org/";
    public namespace ns3 = "http://yet.some.other/namespace";
    public namespace ns4 = "bla bla";

    public class Test {

        ns1 static function test() {
            trace("error");
        }
        ns2 static function test() {
            trace("ok 2/4");
            use namespace my.namespaces.ns2;
            (new Test()).test2()
        }
        ns3 static function test() {
            trace("error");
        }

        ns1 function test2() {trace("error");}
        ns2 function test2() {trace("ok 3/4");}
        ns3 function test2() {trace("error");}

        ns4 static function test3() {
            trace("ok 4/4");
        }
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
                trace("ok 1/4");
            }
            my.namespaces.Test.test()

            my.namespaces.Test.test3()
        }
    }
}

