package starter;

public class LinkedQueueTest extends QueueTest {
  @Override
  protected Queue<Integer> createQueue() {
    return new LinkedQueue<>();
  }
}
