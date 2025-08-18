package starter;

import exceptions.IndexException;

import java.util.Iterator;
import java.util.NoSuchElementException;

// TODO some of the sanity checks are convoluted and need to be refactored!
public class LinkedList<T> implements Iterable<T> {
  private Node<T> head;
  private int numElements;

  public LinkedList() {
    head = null;
    numElements = 0;
  }

  public void addFirst(T t) {
    Node<T> node = new Node<>(t);
    node.next = head;
    head = node;
    ++numElements;
  }

  public void addLast(T t) {
    Node<T> newNode = new Node<>(t);
    Node<T> cur = head;
    if (head == null) {
      addFirst(t);
    } else {
      while (cur.next != null) {
        cur = cur.next;
      }
      cur.next = newNode;
      ++numElements;
    }
  }

  public void traverse() {
    Node<T> cur = head;
    for (int i = 0; i < numElements; i++) {
      System.out.print(cur.data + " ");
      cur = cur.next;
    }
    System.out.println();
  }

  public T get(int index) throws IndexException {
    if (0 <= index && index < numElements) {
      Node<T> pos = head;
      for (int i = 0; i < index; ++i) {
        pos = pos.next;
      }
      return pos.data;
    } else {
      throw new IndexException();
    }
  }


  public void insert(int index, T t) {
    if (0 <= index && index <= numElements) {
      if (index == numElements) {
        addLast(t);
      } else if (index == 0) {
        addFirst(t);
      }
      Node<T> target = head;
      for (int i = 0; i < index - 1; ++i) {
        target = head.next;
      }
      Node<T> newNode = new Node<>(t);
      newNode.next = target.next;
      target.next = newNode;
    } else {
      throw new IndexException();
    }
  }

  public void delete(int index) {
    if (0 <= index && index < numElements) {
      if (index == 0) {
        head = head.next;
      } else {
        Node<T> beforeTarget = head;
        for (int i = 0; i < index - 1; ++i) {
          beforeTarget = beforeTarget.next;
        }
        beforeTarget.next = beforeTarget.next.next;
      }
      --numElements;
    } else {
      throw new IndexException();
    }
  }

  @Override
  public Iterator<T> iterator() {
    return new LinkedListIterator();
  }

  public int length() {
    return 0;  // TODO Implement me!
  }

  private static class Node<E> { //declaration of the node class (nested); static - if not will have redundant references
    E data;
    Node<E> next;

    Node(E data) {
      this.data = data;
    }
  }

  private class LinkedListIterator implements Iterator<T> { //innerclass(nonstatic) still have reference to data array

    Node<T> current;

    public LinkedListIterator() {
      current = head;
    }

    @Override
    public T next() {
      if (!hasNext()) {
        throw new NoSuchElementException();
      }
      T data = current.data;
      current = current.next;
      return data;
    }

    @Override
    public boolean hasNext() {
      return current != null;     // TODO Implement me!
    }
  }

}
