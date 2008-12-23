package test.package {
    public class Main extends flash.display.MovieClip {
        function Main() {
            var i = 0;
            var j = 0;
            while(i*i<100) {
                i++;
            }
            if(i!=10) trace("error");
            else trace("ok 1/3");
            
            while(j*j<121) {
                i++;
                j++;
            }
            if(i!=21) trace("error");
            else trace("ok 2/3");

            var x1=0;
            var a=1;
            var sum=0;
            while(x1<10) {
                var x2=0;
                var a=2;
                x2 = 0;
                while(x2<10) {
                    var x3=0;
                    var a=3;
                    x3 = 0;
                    while(x3<10) {
                        var a=4;
                        sum+=a;
                        x3++;
                    }
                    sum+=a;
                    x2++;
                }
                sum+=a;
                x1++;
            }
            sum+=a;
            if(sum!=4321) trace("error");
            else trace("ok 3/3");
        }
    }
}

