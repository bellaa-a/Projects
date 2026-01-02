package starter;

import exceptions.PositionException;

public class DoublyLinkedList<T> {
  private Node<T> head;
  private Node<T> tail;
  private int numElements;

  private static class Node<E> implements Position<E>{
    E data;
    Node<E> next;
    Node<E> prev;

    DoublyLinkedList<E> owner;

    Node(E data, DoublyLinkedList<E> owner) {
      this.data = data;
      this.owner = owner;
    }

    @Override
    public E get() { //client only has access to this method
      return data;
    }
  }

  public DoublyLinkedList() {
    head = null;
    tail = null;
    numElements = 0;
  }

  public Position<T> addFirst(T data) {
    Node<T> newNode = new Node<>(data, this);
    if (head == null) {
      head = newNode;
      tail = newNode;
      //could also write: head = tail = newNode;
    } else {
      head.prev = newNode;
      newNode.next = head;
      head = newNode;
    }
    numElements++;
    return newNode; //Node is a position; type substitution
  }

  public Position<T> addLast(T data) {
    Node<T> newNode = new Node<>(data, this);
    if (head == null) {
      return addFirst(data);
    } else {
      tail.next = newNode;
      newNode.prev = tail;
      tail = newNode;
      numElements++;
    }
    return newNode;
  }

  public Position<T> get(int index) { //careful of edge cases
    //starting from head and searching forward
    /*
    Node<T> target = head;
    for (int i = 0; i < index; i++) {
      target = target.next;
    }
    return target;
    */

    //starting from tail and searching backward
    Node<T> target = tail;
    for (int i = numElements - 1; i > index; i--) {
      target = target.prev;
    }
    return target;
  }

  private Node<T> convert(Position<T> position) throws PositionException{
    try {
      Node<T> node = (Node<T>) position;
      if (node.owner != this) { //if this is not owner of the node
        throw new PositionException();
      }
      return node;
    } catch (ClassCastException | NullPointerException ex) {
      throw new PositionException();
    }
  }

  public void delete(Position<T> target) { //EDGE CASES
    //target != null
    Node<T> targetNode = convert(target); //down-casts
    Node<T> prevNode = targetNode.prev;
    Node<T> nextNode = targetNode.next;
    prevNode.next = nextNode;
    nextNode.prev = prevNode;
    numElements--;
  }

  public void insertAfter(Position<T> target, T data) {
    Node<T> targetNode = convert(target);
    Node<T> newNode = new Node<>(data, this);
    Node<T> nextNode = targetNode.next;
    targetNode.next = newNode;
    newNode.prev = targetNode;
    newNode.next = nextNode;
    nextNode.prev = newNode;
    numElements++;
  }

  public void insertBefore(Position<T> target, T data) {
    Node<T> targetNode = convert(target);
    Node<T> newNode = new Node<>(data, this);
    Node<T> prevNode = targetNode.prev;
    prevNode.next = newNode;
    newNode.prev = prevNode;
    newNode.next = targetNode;
    targetNode.prev = newNode;
    numElements++;
  }


}