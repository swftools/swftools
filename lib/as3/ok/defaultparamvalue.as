package {
    public class Main extends flash.display.MovieClip {
        public myfunction(var x:String="ok 2/4") 
        {
            trace(x);
        }
        public myfunction2(var i:int,var x:String="ok 4/4") 
        {
        }
        public function HelloWorld() {
            myfunction("ok 1/4");
            myfunction();
            myfunction2(0,"ok 3/4");
            myfunction2(0);
        }
    }
}
