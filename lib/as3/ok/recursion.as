
package {
    import flash.display.MovieClip
    import flash.geom.Point

    public class Main extends flash.display.MovieClip {
        
        function write(nr) {
            trace("ok "+nr+"/3");
            if(nr<3)
                write(nr+1);
        }

        public function Main() {
            write(1);
        }
    }
}
