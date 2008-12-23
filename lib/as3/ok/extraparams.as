package {
    import flash.display.MovieClip;

    public class Main extends flash.display.MovieClip {
        public function test(s,...numbers) {
            if(numbers[0]==1 &&
               numbers[1]==2 &&
               numbers[2]==3 &&
               numbers[3]==4) trace("ok 1/2");
            if(numbers.length==4) trace("ok 2/2");
        }
        function Main() {
            test("", 1,2,3,4);
        }
    }
}
