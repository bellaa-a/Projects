package starter;

import exceptions.EmptyException;

/**
 * Array Implementation of the Queue ADT.
 *
 * @param <T> base type.
 */
public class ArrayQueue<T> implements Queue<T> {

  private T[] data;
  private int numElements;
  private int front;
  private int back;

  @Override
  public void enqueue(T value) {
    // TODO: Implement me! - back points to next availiable space
    data[back] = value;
    back = (back + 1) % data.length;
    numElements++;
  }

  @Override
  public void dequeue() throws EmptyException {
    // TODO: Implement me!
    front = (front + 1) % data.length;
    numElements--;
  }

  @Override
  public T front() throws EmptyException {
    return data[front]; // TODO: Implement me!
  }

  @Override
  public boolean empty() {
    return numElements == 0; // TODO: Implement me!
  }
}
