package starter;

public class Main {
    public static void main(String[] args) {
        IndexedList<Integer> intList = new ArrayIndexedList<>(5, -1);

        for (int i = 0; i < intList.length(); i++) {
            System.out.print(intList.get(i) + " ");
        }
        System.out.println();

        IndexedList<String> strList = new ArrayIndexedList<>(5, "---");
        strList.put(0, "Alice");
        strList.put(1, "Bob");
        strList.put(2, "Cathy");

        for (int i = 0; i < strList.length(); i++) {
            System.out.print(strList.get(i) + " ");
        }
        System.out.println();

    }
}
