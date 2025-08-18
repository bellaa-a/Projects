package starter;

/**
 * The essence of a sorting algorithm.
 * <p>
 * Note that in order to sort, some random type T would not be good enough. Things to be sorted need an *order*, and we
 * need a way to compare things, not just for equality (there's an equals() method on all objects) but for less-than and
 * greater-than as well.
 * </p>
 * <p>
 * This is why we need a *bounded* type parameter: Not just *any* T will do, only those that are at least of type
 * Comparable. That, in turn, ensures that we have a method compareTo() that we can use to establish order. See
 * https://docs.oracle.com/javase/8/docs/api/java/lang/Comparable.html for more.
 * </p>
 *
 * @param <T> Element type.
 */
public interface SortingAlgorithm<T extends Comparable<T>> { //bounded generic, able to sort - needs to be comparable
  /**
   * Sort a collection.
   *
   * @param indexedList collection to sort.
   */
  void sort(IndexedList<T> indexedList);

  /**
   * Name of algorithm.
   *
   * @return Name of algorithm.
   */
  String name();
}