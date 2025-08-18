package starter;

import exceptions.EmptyException;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.junit.jupiter.api.Assertions.fail;

public abstract class QueueTest {

  protected Queue<Integer> queue;

  @BeforeEach
  public void setUp() throws Exception {
    queue = createQueue();
  }

  protected abstract Queue<Integer> createQueue();

  @Test
  public void newQueueIsEmpty() {
    assertTrue(queue.empty());
  }

  @Test
  public void frontOnEmptyQueueThrowsException() {
    try {
      queue.front();
      fail("EmptyException was expected but not thrown.");
    } catch (EmptyException e) {
      return;
    }
  }

  @Test
  public void dequeueOnEmptyQueueThrowsException() {
    try {
      queue.dequeue();
      fail("EmptyException was expected but not thrown.");
    } catch (EmptyException e) {
      return;
    }
  }

  @Test
  public void queueNotEmptyAfterEnqueue() {
    queue.enqueue(1);
    assertFalse(queue.empty());
  }

  @Test
  public void frontReturnsFirstIn() {
    queue.enqueue(1);
    queue.enqueue(2);
    queue.enqueue(3);
    assertEquals(1, queue.front().intValue());
  }

  @Test
  public void dequeueRemovesFirstIn() {
    queue.enqueue(1);
    queue.enqueue(2);
    queue.enqueue(3);
    queue.enqueue(4);
    queue.dequeue();
    assertEquals(2, queue.front().intValue());
  }
}