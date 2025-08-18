package starter;

import exceptions.EmptyException;

/**
 * Linked implementation of Queue ADT.
 *
 * @param <T> base type.
 */
public class LinkedQueue<T> implements Queue<T> {

  private Node<T> front;
  private Node<T> back;

  @Override
  public void enqueue(T value) {
    // TODO: append at back
    Node<T> newNode = new Node<>();
    newNode.data = value;
    back.next = newNode;

  }

  @Override
  public void dequeue() throws EmptyException {
    // TODO: get rid of first node
    if (empty()) { throw new EmptyException(); }
    front.next = front.next.next;
  }

  @Override
  public T front() throws EmptyException {
    if (empty()) { throw new EmptyException(); }
    return front.data; // TODO: Implement me!
  }

  @Override
  public boolean empty() {
    return front == null; // TODO: Implement me!
  }

  private static class Node<T> {
    T data;
    Node<T> next;
  }
}
