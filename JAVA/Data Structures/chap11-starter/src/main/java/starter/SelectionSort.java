package starter;

/**
 * The basic Selection Sort algorithm.
 *
 * @param <T> Element type.
 */
public final class SelectionSort<T extends Comparable<T>>
    implements SortingAlgorithm<T> {

  //Finds smallest and move to end
  //Start by considering all elements to be in the unsorted portion
  //While there are unsorted elements remaining, find the smallest element in the unsorted
  //region and put it at the end of the sorted region

  //Trace: 14, 10, 23, 34, 6, 17, 50, 14
  //  it 0: | 14, 10, 23, 34, 6, 17, 50, 14
  //  it 1: 6 | 10, 23, 34, 14, 17, 50, 14
  //  it 2: 6, 10 | 23, 34, 14, 17, 50, 14
  //  it 3: 6, 10, 14 | 34, 23, 17, 50, 14
  //  it 4: 6, 10, 14, 14 | 23, 17, 50, 34
  //  it 5: 6, 10, 14, 14, 17 | 23, 50, 34

  //Algorithm
  //  Let n be the length of the val array
  //  For i taking values from 0 to (n-2)
  //    // scan the array from i onward for smallest element
  //    Min = i
  //    For j taking values from (i+1) to (n-1)
  //      If val[j] less than val[min]
  //        Min = j
  //      Swap val[min] and val[i]

  //Complexity
  //  Each inner pass compares each element in the unsorted part of the array with the current
  //  smallest
  //  There is only one swap to place the smallest unsorted element in the correct position
  //  In the worst case, when one has the array in descending order, there is no change to the
  //  number of comparisons and swaps. However, the index to the smallest element is updated
  //  at each comparison
  //    Then there would be (n-1) + (n-2) + ... + 1 comparisons and n-1 swaps. We know this sum
  //    works out to be n*(n-1)/2 comparisons which is (1/2)n^2 - (1/2)*n, which is O(n^2)


  @Override
  public void sort(IndexedList<T> indexedList) {
    // We try to put "correct" values into a[0], a[1], ... a[n-2];
    // once a "correct" value is in a[n-2], the very last value
    // has to be the largest one anyway; thus it's also "correct".
    for (int i = 0; i < indexedList.length() - 1; i++) {
      // We're trying to put the "correct" element in a[i].
      // We need to find the smallest element in a[i..n-1].
      // We start by assuming a[i] is the smallest one.
      int min = i;
      // Now we try to find a smaller one in a[i+1..n-1].
      for (int j = i + 1; j < indexedList.length(); j++) {
        if (this.less(indexedList.get(j), indexedList.get(min))) {
          min = j;
        }
      }
      // Now we have the "true" minimum at a[min], and we
      // swap it with a[i], unless i == min of course.
      if (min != i) {
        T t = indexedList.get(i);
        indexedList.put(i, indexedList.get(min));
        indexedList.put(min, t);
      }
    }
  }

  // is a less than b?
  private boolean less(T a, T b) {
    return a.compareTo(b) < 0;
  }

  @Override
  public String name() {
    return "Selection Sort";
  }
}