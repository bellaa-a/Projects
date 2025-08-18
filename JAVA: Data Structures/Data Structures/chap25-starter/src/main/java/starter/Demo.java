package starter;

import java.util.Arrays;

public class Demo {

  /**
   * Sorts the input arr.
   *
   * @param arr an array of integers.
   */
  public static void quicksort(int[] arr) {
    quicksort(arr, 0, arr.length);
  }

  // Pre: 0 <= left <= right <= arr.length.
  // Post: arr[left] ... arr[right-1] is sorted.
  private static void quicksort(int[] arr, int left, int right) {
    // if there are at least two elements to sort...
    if (right - left >= 2) {
      int p = partition(arr, left, right);
      quicksort(arr, left, p);
      quicksort(arr, p + 1, right);
    }
  }

  // Partition by taking the right-most element as pivot.
  // Pre: 0 <= left <= right <= arr.length;
  // Post: a[left] <= ... <= pivot <= ... <= a[right - 1]
  // Returns: index of pivot element in array.
  private static int partition(int[] arr, int left, int right) {
    int p = right - 1;
    int i = left;
    int j = p - 1;

    // while indices have not crossed
    while (i <= j) {
      if (arr[i] <= arr[p]) { // advance left index if its element is smaller than pivot
        i++;
      } else if (arr[j] > arr[p]) { // advance right index if its element is larger than pivot
        j--;
      } else {
        swap(arr, i, j);
      }
    }
    // swap pivot into its rightful position
    swap(arr, i, p);

    return i;
  }

  // Swap arr[i] with arr[j]
  private static void swap(int[] arr, int i, int j) {
    int tmp = arr[i];
    arr[i] = arr[j];
    arr[j] = tmp;
  }

  /**
   * Demo starts here.
   *
   * @param args command-line arguments.
   */
  public static void main(String[] args) {
    int[] arr = {20, 13, 7, 71, 31, 10, 5, 50, 100};
    System.out.println("Input: " + Arrays.toString(arr));
    quicksort(arr);
    System.out.println("Output: " + Arrays.toString(arr));
  }
}
