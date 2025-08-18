package starter;

import exceptions.EmptyException;
import exceptions.PositionException;
import java.util.Iterator;

/**
 * A doubly linked list implementation of the List ADT.
 *
 * @param <T> Element type.
 */
public class LinkedList<T> implements List<T> {
  private Node<T> head;
  private Node<T> tail;
  private int numElements;

  /**
   * Create an empty list.
   */
  public LinkedList() {
    head = null;
    tail = null;
    numElements = 0;
  }

  // Convert a Position back into a Node.
  // Guards against null positions, positions from other data structures,
  // and positions that belong to other LinkedList objects.
  private Node<T> convert(Position<T> position) throws PositionException {
    try {
      Node<T> node = (Node<T>) position;
      if (node.owner != this) {
        throw new PositionException();
      }
      return node;
    } catch (NullPointerException | ClassCastException e) {
      throw new PositionException();
    }
  }

  @Override
  public int length() {
    // TODO Implement me!
    return 0;
  }

  @Override
  public boolean empty() {
    // TODO Implement me!
    return false;
  }

  @Override
  public Position<T> insertFront(T data) {
    // TODO Implement me!
    Node<T> newNode = new Node<>(data, this);
    newNode.next = head;
    head.prev = newNode;
    head = newNode;
    return newNode;
  }

  @Override
  public Position<T> insertBack(T data) {
    // TODO Implement me!
    return null;
  }

  @Override
  public Position<T> insertBefore(Position<T> position, T data)
      throws PositionException {
    // TODO Implement me!
    return null;
  }

  @Override
  public Position<T> insertAfter(Position<T> position, T data)
      throws PositionException {
    // TODO Implement me!
    return null;
  }

  @Override
  public void remove(Position<T> position) throws PositionException {
    // TODO Implement me!
  }

  @Override
  public void removeFront() throws EmptyException {
    // TODO Implement me!
  }

  @Override
  public void removeBack() throws EmptyException {
    // TODO Implement me!
  }

  @Override
  public Position<T> front() throws EmptyException {
    // TODO Implement me!
    return null;
  }

  @Override
  public Position<T> back() throws EmptyException {
    // TODO Implement me!
    return null;
  }

  @Override
  public boolean first(Position<T> position) throws PositionException {
    // TODO Implement me!
    return false;
  }

  @Override
  public boolean last(Position<T> position) throws PositionException {
    // TODO Implement me!
    return false;
  }

  @Override
  public Position<T> next(Position<T> position) throws PositionException {
    // TODO Implement me!
    return null;
  }

  @Override
  public Position<T> previous(Position<T> position) throws PositionException {
    // TODO Implement me!
    return null;
  }

  @Override
  public Iterator<T> forward() {
    // TODO Implement me!
    return null;
  }

  @Override
  public Iterator<T> backward() {
    // TODO Implement me!
    return null;
  }

  @Override
  public Iterator<T> iterator() {
    // TODO Implement me!
    return null;
  }

  private static class Node<E> implements Position<E> {
    Node<E> next;
    Node<E> prev;
    E data;
    List<E> owner;

    Node(E data, List<E> owner) {
      this.data = data;
      this.owner = owner;
    }

    @Override
    public E get() {
      return data;
    }
  }
}
