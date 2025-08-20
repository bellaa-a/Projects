package starter;

/**
 * The Insertion Sort algorithm, with minimizing swaps optimization.
 *
 * @param <T> Element type.
 */
public final class InsertionSort<T extends Comparable<T>>
    implements SortingAlgorithm<T> {

  //Start by considering the first element to make up the unsorted portion of the array, and
  //every other element in the unsorted portion. Now. for every element remaining, shift it
  //back (insert it) into the sorted portion at its proper position.

  //Trace: 14, 10, 23, 34, 6, 17, 50, 14
  //  it 0: 14 | 10, 23, 34, 6, 17, 50, 14
  //  it 1: 10, 14 | 23, 34, 6, 17, 50, 14
  //  it 2: 10, 14, 23 | 34, 6, 17, 50, 14
  //  it 3: 10, 14, 23, 34 | 6, 17, 50, 14
  //  it 4: 6, 10, 14, 23, 34 | 17, 50, 14
  //  it 5: 6, 10, 14, 17, 23, 34 | 50, 14
  //  it 6: 6, 10, 14, 17, 23, 34, 50 | 14
  //  it 7: 6, 10, 14, 14, 17, 23, 34, 50 |

  //Algorithm
  //  // for every element from the second one onward
  //  for i t taking values from 1 to (n-1)
  //    // insert that element (shift it down) into its sorted position
  //    j gets i
  //    while j > 0 and val[j] is less than val[j-1]
  //      swap val[j] and val [j-1]
  //      decrement j by 1
  //    // at this point, all elements to the left of index i are sorted

  //Complexity
  //  number of swaps = 1 + 2 + ... + (n-1)
  //  = n*(n-1)/2 which is O(n^2)


  @Override
  public void sort(IndexedList<T> indexedList) {
    // TODO: Implement me
  }

  @Override
  public String name() {
    return "Insertion Sort";
  }
}
