package starter;

public class ArrayQueueTest extends QueueTest {

  @Override
  protected Queue<Integer> createQueue() {
    return new ArrayQueue<>();
  }
}
