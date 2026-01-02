package starter;

import java.util.Arrays;
import java.util.PriorityQueue;

public class Heap {

  private static Integer[] sample1() {
    return new Integer[]{12, 14, 11};
  }

  private static Integer[] sample2() {
    return new Integer[]{10, 5, 8, 1, 6, 14, 12, 3, 7, 20};
  }

  /**
   * Demo starts here.
   * @param args command-line arguments.
   */
  public static void main(String[] args) {
    Integer[] data = sample2();

    System.out.println("Before:");
    System.out.println(Arrays.toString(data));

    sort(data);

    System.out.println("After:");
    System.out.println(Arrays.toString(data));
  }

  // Pre: data != null
  private static void sort(Integer[] data) {

    // SPACE COMPLEXITY (let n = data.length)
    // input: O(n)
    // aux: O(n)
    // total: O(n)

    // TIME COMPLEXITY
    // O(n * lg n)

    // create empty priority queue with appropriate capacity
    PriorityQueue<Integer> priorityQueue = new PriorityQueue<>(data.length); // O(n) : have to initialize default values

    // put every element from data array into priority queue
    for (int i = 0; i < data.length; i++) { // loop runs n times
      priorityQueue.add(data[i]); // O(lg n) : binary heap
    }
    // entire loop is O(n * lg n)

    // take out every element from the priority queue and put it back in the array
    for (int i = 0; i < data.length; i++) { // loop runs n times
      data[i] = priorityQueue.remove(); // O(lg n) : sinks to bottom
    }
    // entire loop is O(n * lg n)

  }
}
