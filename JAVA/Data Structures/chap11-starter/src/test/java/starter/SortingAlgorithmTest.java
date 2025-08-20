package starter;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import java.util.Iterator;

import static org.junit.jupiter.api.Assertions.assertTrue;

public abstract class SortingAlgorithmTest {

  private SortingAlgorithm<Integer> unit;

  @BeforeEach
  public void initSortingAlgorithm() {
    this.unit = createSortingAlgorithm();
  }

  protected abstract SortingAlgorithm<Integer> createSortingAlgorithm();

  @Test
  @DisplayName("The sorting algorithm sorted the input array sample")
  public void sortSample() {
    IndexedList<Integer> indexedList = sample();
    unit.sort(indexedList);
    assertTrue(isSorted(indexedList));
  }

  private static IndexedList<Integer> sample() {
    IndexedList<Integer> indexedList = new ArrayIndexedList<>(8, 0);
    indexedList.put(0, 14);
    indexedList.put(1, 10);
    indexedList.put(2, 23);
    indexedList.put(3, 34);
    indexedList.put(4, 6);
    indexedList.put(5, 17);
    indexedList.put(6, 50);
    indexedList.put(7, 14);
    return indexedList;
  }

  // Checks the elements in "arr" are sorted in "ascending order."
  private static <T extends Comparable<T>> boolean isSorted(IndexedList<T> arr) {
    // Use iterator so to not affect the operation count in arr.
    Iterator<T> it = arr.iterator();
    T prev = it.hasNext() ? it.next() : null;
    while (it.hasNext()) {
      T current = it.next();
      if (prev.compareTo(current) > 0) {
        return false;
      }
      prev = current;
    }
    return true;
  }
}
