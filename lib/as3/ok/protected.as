package {

    public class A 
    {
        protected var xx:Array = [];
        
        public function A()
        {
            trace("ok");
        }
    }

    public class B extends A
    {
        public function B()
        {
            super();
        }
    }

    import flash.display.MovieClip
    public class Main extends flash.display.MovieClip 
    {
        public function Main() 
        {
            var b:B = new B();
        }
    }

}
