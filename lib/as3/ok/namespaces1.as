package {

    import flash.display.MovieClip
    import flash.utils.Proxy
    import flash.utils.flash_proxy
    
    use namespace flash.utils.flash_proxy;

    public class Main extends flash.display.MovieClip {

        public function Main() 
        {
            var p:Proxy = new Proxy
            try {
                p.hasProperty("test")
            } catch(e:Error) {
            }
            trace("ok 1/1");
        }
    }
}

