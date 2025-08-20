package starter;

public class ArrayStackTest extends StackTest {

  @Override
  protected Stack<Integer> createStack() {
    return new ArrayStack<>();
  }
}
