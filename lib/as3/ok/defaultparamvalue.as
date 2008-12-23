package {
    import flash.display.MovieClip
    public class Main extends flash.display.MovieClip {
        public function myfunction(x:String="ok 2/4") 
        {
            trace(x);
        }
        public function myfunction2(i:int,x:String="ok 4/4") 
        {
            trace(x);
        }
        public function Main() {
            myfunction("ok 1/4");
            myfunction();
            myfunction2(0,"ok 3/4");
            myfunction2(0);
        }
    }
}
