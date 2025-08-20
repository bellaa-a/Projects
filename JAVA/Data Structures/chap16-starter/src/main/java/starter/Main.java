package starter;

public class Main {

    /**
     * Driver program to visualize/test this implementation.
     *
     * @param args command-line arguments.
     */
    public static void main(String[] args) {
        BstOrderedSet<Integer> bst = new BstOrderedSet<>();
        bst.insert(7);
        bst.insert(4);
        bst.insert(13);
        bst.insert(1);
        bst.insert(6);
        bst.insert(10);
        bst.insert(15);
        bst.insert(3);

        System.out.println(bst.size());
        System.out.println(bst.has(13));
        System.out.println(bst.has(23));
    }
}
