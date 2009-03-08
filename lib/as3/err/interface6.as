package {
    interface A {
        var x;

        // error: interface can not have top-level code
        if(x==0) {
        } else {
        }
    }
}
