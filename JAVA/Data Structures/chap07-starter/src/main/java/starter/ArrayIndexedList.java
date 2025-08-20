package starter;

import exceptions.IndexException;
import exceptions.LengthException;
import java.util.Iterator;
import java.util.NoSuchElementException;

/**
 * An implementation of IndexedList that takes a default value
 * to plaster over the entire data structure.
 *
 * @param <T> the base type of the items in the IndexedList.
 */
public class ArrayIndexedList<T> implements IndexedList<T> {
  private final T[] data;

  /**
   * Construct an ArrayIndexedList with given size and default value.
   *
   * @param size         the capacity of this list.
   * @param defaultValue a default value to plaster over the entire list.
   * @throws LengthException when size <= 0
   */
  public ArrayIndexedList(int size, T defaultValue) throws LengthException {
    if (size <= 0) {
      throw new LengthException();
    }

    data = (T[]) new Object[size];
    for (int i = 0; i < size; i++) {
      data[i] = defaultValue;
    }
  }

  @Override
  public void put(int index, T value) throws IndexException {
    if (index < 0 || index > length()) {
      throw new IndexException();
    }

    data[index] = value;
  }

  @Override
  public T get(int index) throws IndexException {
    if (index >= 0 && index < length()) {
      return data[index];
    } else {
      throw new IndexException();
    }
  }

  @Override
  public int length() {
    return data.length;
  }


  @Override
  public Iterator<T> iterator() {
    return new ArrayIndexedListIterator();
  }

  private class ArrayIndexedListIterator implements Iterator<T> {
    private int cursor;

    ArrayIndexedListIterator() {
      cursor = 0;
    }

    @Override
    public boolean hasNext() {
      return cursor < data.length;
    }

    @Override
    public T next() {
      if (!hasNext()) {
        throw new NoSuchElementException();
      }
      return data[cursor++]; //since its a non-static inner class, can call data[]
    }
  }
}

