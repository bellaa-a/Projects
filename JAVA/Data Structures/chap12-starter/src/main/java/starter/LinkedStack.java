package starter;

import exceptions.EmptyException;

/**
 * Stack ADT implemented using linked nodes.
 *
 * @param <T> base type.
 */
public class LinkedStack<T> implements Stack<T> {

  private Node<T> list;

  /**
   * Construct an ListStack.
   */
  public LinkedStack() {
    list = null;
  }

  @Override
  public boolean empty() {
    return list == null;
  }

  @Override
  public T top() throws EmptyException {
    if (empty()) {throw new EmptyException(); }
    return list.data;
  }

  @Override
  public void pop() throws EmptyException {
    if (empty()) { throw new EmptyException(); }
    list = list.next;
  }

  @Override
  public void push(T t) {
    Node<T> newNode = new Node<T>();
    newNode.data = t;
    newNode.next = list;
    list = newNode;
  }

  private static class Node<T> {
    T data;
    Node<T> next;
  }
}
