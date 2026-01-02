package starter;

import exceptions.IndexException;
import exceptions.LengthException;
import java.util.Iterator;

/**
 * Linked (singly linked-list) implementation of IndexedList.
 *
 * @param <T> Element type.
 */
public class LinkedIndexedList<T> implements IndexedList<T> {

  private Node<T> head;

  /**
   * Constructs a new LinkedIndexedList of length size
   * with default value of defaultValue.
   *
   * @param size Length of list, expected: size > 0.
   * @param defaultValue Default value to store in each slot.
   * @throws LengthException if size <= 0.
   */
  public LinkedIndexedList(int size, T defaultValue) throws LengthException {
    // TODO Implement me!
  }

  @Override
  public void put(int index, T value) throws IndexException {
    // TODO Implement me!
  }

  @Override
  public T get(int index) throws IndexException {
    return null; // TODO Implement me!
  }

  @Override
  public int length() {
    return 0; // TODO Implement me!
  }

  @Override
  public Iterator<T> iterator() {
    return new LinkedIndexListIterator();
  }

  // Node does not have access to members of LinkedIndexedList
  // because it is static
  private static class Node<T> {
    T data;
    Node<T> next;
  }

  // An iterator to traverse the linked list from front (head) to back.
  private class LinkedIndexListIterator implements Iterator<T> {

    @Override
    public boolean hasNext() {
      return false; // TODO Implement me!
    }

    @Override
    public T next() {
      return null; // TODO Implement me!
    }
  }
}
