package package1 {
    public class A {
        public var x;
        public static function f() 
        {
            x = 3; //error: this member is not static
        }
    }
}
