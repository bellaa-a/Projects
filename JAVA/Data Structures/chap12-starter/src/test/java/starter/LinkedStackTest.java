package starter;

public class LinkedStackTest extends StackTest {

  @Override
  protected Stack<Integer> createStack() {
    return new LinkedStack<>();
  }
}
