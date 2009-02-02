package {
    import flash.display.MovieClip;

    public class Main extends flash.display.MovieClip {
        public function test2(s,...numbers) {
        }
        public function test(s,...numbers) {
            
            //test2(s,...numbers);

            if(s=="ok")
                trace("ok 1/3");
            if(numbers[0]==1 &&
               numbers[1]==2 &&
               numbers[2]==3 &&
               numbers[3]==4) trace("ok 2/3");
            if(numbers.length==4) trace("ok 3/3");
        }
        function Main() {
            test("ok", 1,2,3,4);
            trace("[exit]");
        }
    }
}
