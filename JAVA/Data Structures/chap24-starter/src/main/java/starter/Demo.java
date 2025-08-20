package starter;

import java.util.Arrays;

public class Demo {

  /**
   * Sorts the input arr.
   * @param arr an array of integers.
   */
  public static void mergesort(int[] arr) {
    mergesort(arr, 0, arr.length);
  }

  // Pre: left and right are valid indices such that
  //        0 <= left <= right <= arr.length;
  // Post: arr[left] ... arr[right-1] is sorted.
  private static void mergesort(int[] arr, int left, int right) {
    // if there are at least two elements to sort...
    if (right - left >= 2) {
      int mid = (left + right)/2;
      mergesort(arr, left, mid);
      mergesort(arr, mid, right);
      merge(arr, left, mid, right);
    }
    // else: do nothing; a subarray with 0 or 1 elements is trivially sorted
  }

  // Pre:
  //  1) left, mid and right are valid indices such that
  //        0 <= left <= mid < right <= arr.length;
  //  2) arr[left] ... arr[mid-1] is sorted
  //  3) arr[mid] ... arr[right-1] is sorted
  // Post: arr[left] ... arr[right-1] is sorted.
  private static void merge(int[] arr, int left, int mid, int right) {
    // if last element of left <= first element of right, subarray is sorted
    // otherwise...
    if (arr[mid-1] > arr[mid]) {
      int[]tmp = new int[right - left];
      int i = left;
      int j = mid;
      int k = 0;

      // as long as both halves contain values
      while (i < mid && j < right) {
        // take the smallest front value and place it in my destination array
        tmp[k++] = arr[i] < arr[j] ? arr[i++] : arr[j++];
      }
      // if necessary, finish copying the left half of the values
      while (i < mid) {
        tmp[k++] = arr[i++];
      }
      // if necessary, finish copying the right half of the values
      while (j < right) {
        tmp[k++] = arr[j++];
      }
      // copy everything backujkmn
      for (int n = 0; n < k; n++) {
        arr[left + n] = tmp[n];
      }
    }

  }

  /**
   * Demo starts here.
   * @param args command-line arguments.
   */
  public static void main(String[] args) {
    int[] arr = {5, 1, 3, 0, 8, 2, 4, 9};
    System.out.println("Input: " + Arrays.toString(arr));
    mergesort(arr);
    System.out.println("Output: " + Arrays.toString(arr));
  }
}