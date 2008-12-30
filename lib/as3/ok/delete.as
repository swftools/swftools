package {
    import flash.display.MovieClip

    class C {
    }
    public dynamic class Main extends flash.display.MovieClip {
        function Main() {
            this.xx = new C;
            if(this.xx is C) trace("ok 1/5");
            delete this.xx;
            if(this.xx is C) trace("error");

            var a:Array = [1,2,3,4];
            delete a[2];
            if(a[0]==1 &&
               a[1]==2 &&
               String(a[2])=="undefined" &&
               a[3]==4) trace("ok 2/5");

            //TODO: we can also delete from XML
        }
    }
}

