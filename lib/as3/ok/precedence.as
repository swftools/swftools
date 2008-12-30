package {
    
    import flash.display.MovieClip

    class C {
        function f() {return new Array(1,2,3);}
    }
    public class Main extends flash.display.MovieClip {
        var count:int = 1;
        var num:int = 53;
        function assert(b) {
            if(b) {
                trace("ok "+count+"/"+num);
            } else {
                trace("error "+count+"/"+num);
            }
            count = count + 1
        }
        function compare(x,y) {
            assert(x==y);
        }
            
        var a:Array = new Array(1,2,3);
        var b:int=10;
        function f() {return a;}
        function g() {return b;}
        var i=0;

        function Main() {

            // test that ., [], (), @, ::, .. {x:y}, new all have the same precedence
            // TODO: @, ::, ..

            compare(this.f()[2] , (((this).f)())[2])
            //compare(new C.f()[1],2); //breaks
            compare(new C().f()[1],2);

            // test that ., [], () have higher precedence than ++, --, -, ~, !, delete, typeof
            // TODO: @, delete, typeof
            var i:int=5,j:int;
            this.i=0;
            this.i++;compare(this.i,1);compare(i,5);
            ++(this.a)[2]; compare(this.a[2],4);
            delete this.f()[0];compare(String(this.a[0]), "undefined");

            // test that ++ has higher precedence than unary -,~,!
            compare(-i++,-5);
            compare(!i++,false);
            compare(~i++,~7);

            // test that * / % have the same precedence
            compare(3*10/15%4, 2)
            
            // test that ++,--,~ have higher precedence than * / %
            i = 0;j = 1;compare(++i*j++, 1)
            i = 0;j = 1;compare(++i/j++, 1)
            i = 2;j = 2;compare(++i%j++, 1)
            compare( (~1*2) & 1, 0);

            // test that +,- have lower precedence than * / %
            compare(1+1*2, 3)
            compare(5-4/2, 3)
            compare(3-4%2, 3)
            compare(2+1%2, 3)

            // test that +, - have higher precedence than >>,<<,>>>
            compare(4>>1+1, 1)
            compare(4>>1-1, 4)
            compare(4>>>1+1, 1)
            compare(4>>>1-1, 4)
            compare(1<<1+1, 4)
            compare(4<<1-1, 4)
            
            // test that >>,<< have higher precedence than <,>,<=,>=,==
            compare(3 < 1<<2, true)
            compare(4 <= 1<<2, true)
            compare(2 > 8>>3, true)
            compare(1 >= 8>>3, true)
            compare(1 == 1<<0, true)
            compare(1 == 1>>0, true)
            
            // test that <,>,<=,>= have higher precedence than as, in
            compare(1<2 as Boolean, true)
            //compare(1<2 in [true,true,true], true)
            
            // test that as,in have higher precedence than ==,!=
            compare(1==1 as Boolean, false);
            compare(1!=1 as Boolean, true);
            compare(false == true is Boolean, false);
            compare(true != true is Boolean, false);

            // test that >,<,>=,<= have higher precedence than ==, !=, ===, !==
            compare(true == 3<4, true)
            compare(true != 3>4, true)
            compare(true === 3<=4, true)
            compare(true !== 3>=4, true)
            
            // test that ==,!= have higher precedence than &
            compare(3&4==4, 1)
            compare(3&0!=4, 1)

            // test that & has higher precedence than ^
            compare(1^1&4, 1)
            
            // test that ^ has higher precedence than |
            compare(5|4^4, 5)
            
            // test that | has higher precedence than &&
            compare(false && 0|5, false)
            
            // test that && has higher precedence than ||
            compare(false && true || true, true)
            
            // test that || has higher precedence than ?:
            compare(true || false?11:0, 11)
            
            // test that ?: and = have same precedence 
            var y = 0;
            var x = false ? y=3 : y=4;
            compare(y,4)
            false ? y=3 : y=5;
            compare(y,5)
            
            // test that = is right-associative
            x = y = 3;
            compare(x,3)
            compare(y,3)
            
            // test that = has higher precedence than +=,-=,*=,/=,%=
            x=4;y=10;
            x += y = 4;
            compare(x,8);

            // test that +=,-=,*=,/=,%=,>>=,<<=,>>>= all have the same associativity
            // TODO: %=,/=,-=,>>=,<<=,>>>=
            x=2;y=3;
            x *= y += 4;
            assert(x==14 && y==7);

            // test that , has lower precedence than +=
            x=0;
            var v=0;
            v += 3,4;
            compare(v,3)

        }

    }
}

