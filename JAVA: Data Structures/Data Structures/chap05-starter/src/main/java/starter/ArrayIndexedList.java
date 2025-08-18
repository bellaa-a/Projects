package starter;

/**
 * An implementation of IndexedList that takes a default value
 * to plaster over the entire data structure.
 *
 * @param <T> the base type of the items in the IndexedList.
 */
public class ArrayIndexedList<T> implements IndexedList<T> {
  private T[] data;

  /**
   * Construct an ArrayIndexedList with given size and default value.
   *
   * @param size         the capacity of this list.
   * @param defaultValue a default value to plaster over the entire list.
   */
  public ArrayIndexedList(int size, T defaultValue) {
    data = (T[]) new Object[size];
    for (int i = 0; i < size; i++) {
      data[i] = defaultValue;
    }
  }

  @Override
  public void put(int index, T value) {
    data[index] = value;
  }

  @Override
  public T get(int index) throws IndexOutOfBoundsException {
    if (index >= 0 && index < length()) {
      return data[index];
    } else {
      throw new IndexOutOfBoundsException();
    }
  }

  @Override
  public int length() {
    return data.length;
  }
}

