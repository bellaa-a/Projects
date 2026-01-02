package starter;

import exceptions.EmptyException;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.junit.jupiter.api.Assertions.fail;

public abstract class StackTest {

  protected Stack<Integer> stack;

  @BeforeEach
  public void setUp() throws Exception {
    stack = createStack();
  }

  protected abstract Stack<Integer> createStack();

  @Test
  public void newStackIsEmpty() {
    assertTrue(stack.empty());
  }

  @Test
  public void topThrowsExceptionOnEmptyStack() {
    try {
      stack.top();
      fail("EmptyException was not thrown where it was expected.");
    } catch (EmptyException e) {
      return;
    }
  }

  @Test
  public void popThrowsExceptionOnEmptyStack() {
    try {
      stack.pop();
    } catch (EmptyException e) {
      return;
    }
  }

  @Test
  public void stackNotEmptyAfterPush() {
    stack.push(1);
    assertFalse(stack.empty());
  }

  @Test
  public void topReturnsLastPushed() {
    stack.push(1);
    stack.push(2);
    stack.push(3);
    assertEquals(3, stack.top().intValue());
  }

  @Test
  public void popRemovesLastPushed() {
    stack.push(1);
    stack.push(2);
    stack.push(3);
    stack.pop();
    assertEquals(2, stack.top().intValue());
  }
}