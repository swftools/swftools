package {
    public namespace ns1 = "http://www.namespace.com/"

    public class Test {
        public ns1 function name() {
            // error: either namespace *or* public
        }
    }
}
