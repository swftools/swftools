package {
    import flash.display.MovieClip
    public class Main extends flash.display.MovieClip {
        var count:int = 1;
        var num:int = 29;
        function assert(b:Boolean) {
            if(b) {
                trace("ok "+count+"/"+num);
            } else {
                trace("error "+count+"/"+num);
            }
            count = count + 1
        }
        function Main() {
            /* ------ basic sanity tests ------ */
            var x:XML = <test>}</test>;

            <test>This XML doesn't do anything!
            </test>
            ;
            /Neither does this regexp!/
            ;
            
            var y = 3;
            <test>test</test>

            assert(true);

            var f2 = function(s:String) {assert(true);} 
            var f3 = function(s:String) {assert(true);} 

            f2(<![CDATA[ bla bla bla /* package String uint 
             int finally try for ]]>);
            
            //f3(<![CDATA[ ]]]>);

            /* ----- xml objects ----- */
    
            var x1:XML = <tree name="test">
                            <branch bname="t1">Branch 1</branch>
                            <branch bname="t2">Branch 2</branch>
                        </tree>

            assert(typeof(x1)=="xml");

            /* ------ access --------- */

            assert(x1. * == x1.children())
            assert((x1.*)[0].text() == "Branch 1")
            assert(x1.branch[0] == x1.child("branch")[0])
            assert((x1.*.@bname)[0] == "t1");

            assert(x1.@name.parent() == x1);
            assert(x1.@["name"].parent() == x1);
            var s1= <t x="a" y="b" z="c" />
            assert(s1.@* == s1.attributes())

            /* ------ comments ------- */

            var x3 = <x><!-- </x>xxxx -->Test</x>;
            assert(x3.toString() == "Test")

            /* ------ descendants ---- */

            var x4 = <l1>
                        <l2>
                            <l3>v1</l3>
                            <x3>e1</x3>
                        </l2>
                        <r2>
                            <x3>e2</x3>
                            <l3>v2</l3>
                        </r2>
                     </l1>
            assert(x4..l3[0] == x4.l2.l3)

            var s:String="";
            for each(var c:XML in x4..l3) {
                s += c.toString();
            }
            assert(s=="v1v2");
           
            s="";
            for each(var e:XML in x1..@bname) {
                s += e.toString();
            }
            assert(s=="t1t2");
            
            s="";
            for each(var e in x1..@*) {
                s += e.toString();
            }
            assert(s=="testt1t2");

            /* ----- filtering ----- */
            
            var x5:XML = <participants>
                            <person gender="m" id="1">John Doe</person>
                            <person gender="f" id="2">Ellen Doe</person>
                            <person gender="m">John Smith</person>
                            <person gender="f" id="4">Jane Smith</person>
                        </participants>

            var l:XMLList = x5.*;
            var l2:XMLList = l.(@gender=="m");
            assert(l2.length()==2 && l2[0].text()=="John Doe" && l2[1].text()=="John Smith");
            l2 = x5.*.(@gender=="f").(hasOwnProperty("@id") && @id>2)
            assert(l2.text() =="Jane Smith");
            
            /* ----- replacement ----- */

            var x10:XML = <a><b a="x"><c/></b></a>
            var x11:XML = <a><b a="y"><d/></b></a>
            
            assert(x10 != x11);
            x11.b.d = <c/>;
            assert(x10 != x11);
            x11.b.@a = "x";
            assert(x10 == x11);

            var x12:XML = <x><a/></x>;
            x12.a += <b/>;

            assert(x12 == <x><a/><b/></x>);

            x12.* = <c/>;
            assert(x12 == <x><c/></x>);
            x12.* = <b/>;

            /* ----- deletion ----- */

            delete x12.a
            assert(x12 == <x><b/></x>);
            delete x12.*
            assert(x12 == <x/>);
            delete x10.b.c
            delete x10.b.@a
            assert(x10 == <a><b/></a>);
            delete x11.b.c
            delete x11.b.@*
            assert(x10 == x11);

            /* ----- special characters ----- */

            trace("[special characters]");

            var x20 = <x> >& </x>;
            assert(x20.toString() == ">&");
            var x21 = <x a="&quot;&lt;&gt;"/>;
            assert(x21.@a == "\"<>");
            var x22 = <x/>;

            /* ----- substitution ----- */

            trace("[substitution]");

            var x2:XML = <{"tree"} {"na"+"me"}="test" {"t=x"}>
                             <{{three: "branch"}["three"]} bname={"t1"}>{"Branch" + " " + 1}</{"branch"}>
                            <branch bname="t2">Branch 2</branch>
                        </tree>
            assert(x1 == x2);

            trace("[exit]");
        }
    }
}
