package starter;

/**
 * The Bubble Sort algorithm with the optimized "quick" break to exit if the array is sorted.
 *
 * @param <T> The type being sorted.
 */
public final class BubbleSort<T extends Comparable<T>>
    implements SortingAlgorithm<T> {

  //For every pair of sequential elements, compare them. If they are out of order, swap them.
  //Then go to the next pair. After each such iteration, the largest element will have "bubbled
  //up" to the sorted position of the array, which grows by one with each iteration.

  //Trace: 14, 10, 23, 34, 6, 17, 50, 14
  //  it 0: 14, 10, 23, 34, 6, 17, 50, 14 |
  //  it 1: 10, 14, 23, 6, 17, 34, 14 | 50
  //  it 2: 10, 14, 6, 17, 23, 14 | 34, 50
  //  it 3: 10, 6, 14, 17, 14 | 23, 34, 50
  //  it 4: 6, 10, 14, 14 | 17, 23, 34, 50

  //Algorithm
  //  for i taking values from (n-1) to 1 (inclusive)
  //    for j taking values from 0 to i-1
  //      if val[j] is greater than val[j+!]
  //        swap val[j] and val[j+1]
  //      //at this point, the largest element in the unsorted portion of the array has
  //      //bubbled up (elements from i onward are sorted)

  //Complexity
  //  In the worst case, we have (n-1) swaps in the first iteration, (n-2) in the second, and so on
  //  Therefore, the total work is a constant factor times n*(n-1)/2 which is O(n^2).

  //Optimization
  //  If no swaps are triggered, then it is already sorted, can stop early
  //  for i taking values from (n-1) to 1 (inclusive)
  //    made_swaps = false
  //    for j taking values from 0 to i-1
  //      if val[j] is greater than val[j+!]
  //        swap val[j] and val[j+1]
  //    if not made_swaps
  //    return // terminate early (array is now sorted)

  @Override
  public void sort(IndexedList<T> indexedList) {
    boolean swapped;
    for (int i = indexedList.length() - 1; i > 0; i--) {
      swapped = false;
      for (int j = 0; j < i; j++) {
        if (less(indexedList.get(j + 1), indexedList.get(j))) {
          swap(indexedList, j, j + 1);
          swapped = true;
        }
      }
      if (!swapped) {
        return;
      }
    }
  }

  // is a less than b?
  private boolean less(T a, T b) {
    return a.compareTo(b) < 0;
  } //return a < b

  // Pre: i & j are valid indices.
  // Post: elements at i & j are swapped.
  private void swap(IndexedList<T> indexedList, int i, int j) {
    T t = indexedList.get(i);
    indexedList.put(i, indexedList.get(j));
    indexedList.put(j, t);
  }

  @Override
  public String name() {
    return "Bubble Sort";
  }
}
