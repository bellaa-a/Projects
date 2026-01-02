package starter;

import java.util.Iterator;

public class Main {
    public static void main(String[] args) {

        int[] myArray = new int[5];
        System.out.println("Print array using regular for loop:");
        for (int i = 0; i < myArray.length; i++) {
            System.out.print(myArray[i] + " ");
        }
        System.out.println();

        IndexedList<Integer> intList = new ArrayIndexedList<>(5, 0);
        System.out.println("Print IndexedList using regular for loop:");
        for (int i = 0; i < intList.length(); i++) {
            System.out.print(intList.get(i) + " ");
        }
        System.out.println();

        System.out.println("Print array using enhanced for loop:");
        for (int number : myArray) { //when u don't care which index, no index out of range
                                     //iterator - linked lists
            System.out.print(number + " ");
        }
        System.out.println();

        System.out.println("Print IndexedList using enhanced for loop:");
        for (int number : intList) { //hides the fact that its using an iterator
            System.out.print(number + " ");
        }
        System.out.println();

        System.out.println("Print IndexedList using iterator explicitly:");
        Iterator<Integer> it = intList.iterator();
        while (it.hasNext()) {
            int number = it.next();
            System.out.print(number + " ");
        }
        System.out.println();
    }
}
