package {
    import flash.display.MovieClip
    public class Main extends flash.display.MovieClip {
        var count:int = 1;
        var num:int = 9;
        function assert(b:Boolean) {
            if(b) {
                trace("ok "+count+"/"+num);
            } else {
                trace("error "+count+"/"+num);
            }
            count = count + 1
        }
        function Main() {
            var x:XML = 
                <tests xmlns:test1="http://www.quiss.org/test1/"
                       xmlns:test2="http://www.quiss.org/test2/"
                       xmlns:test3="http://www.quiss.org/test3/">
                    <test1:test>P1</test1:test>
                    <test2:test>P2</test2:test>
                    <test3:test a="b">P3</test3:test>
                </tests>

            var top:Namespace = x.namespace();
            var ns1:Namespace = x.namespace("test1");
            var ns2:Namespace = x.namespace("test2");
            var ns3:Namespace = x.namespace("test3");
            assert(ns1 == new Namespace("http://www.quiss.org/test1/"));
            assert(ns2 == new Namespace("http://www.quiss.org/test2/"));
            assert(x.ns1::test == "P1");
            assert(x.ns2::test == "P2");

            assert(x.ns1::* == "P1")

            default xml namespace = ns2;
            assert(x.test == "P2")

            assert(x.*::* == (x.ns1::*+x.ns2::*+x.ns3::*))
            assert(x..@*::* == "b");

            x.ns2::test = x.ns3::test;
            assert(x..@a.toString() == "bb");

            trace("[exit]");
        }
    }
}
