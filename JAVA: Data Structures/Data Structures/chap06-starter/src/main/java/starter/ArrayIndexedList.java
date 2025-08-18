package starter;

import exception.IndexException;
import exception.LengthException;

import java.util.Arrays;
import java.util.Iterator;
import java.util.NoSuchElementException;

/**
 * An implementation of IndexedList that takes a default value
 * to plaster over the entire data structure.
 *
 * @param <T> the base type of the items in the IndexedList.
 */
public class ArrayIndexedList<T> implements IndexedList<T> { //must implement iterator()

  @Override
  public Iterator<T> iterator() {
    //return Arrays.stream(data).iterator(); //cheating way
    return new ArrayIndexedListIterator(); //nested iterator class
  }

  private class ArrayIndexedListIterator implements Iterator<T> {
    int cursor;

    public ArrayIndexedListIterator() {
      cursor = 0;
    }
    @Override
    public boolean hasNext() {
      return cursor < data.length; //access to data field because it's a non static nested class
    }

    @Override
    public T next() {
      if (!hasNext()) {
        throw new NoSuchElementException();
      }
      return data[cursor++]; //first uses original cursor, then updates cursor
    }
  }

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
  public void put(int index, T value) throws IndexException { //if checked, have to catch or throw themselves
    if (index < 0 || index > length()) {
      throw new IndexException(); //could put message
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
}

