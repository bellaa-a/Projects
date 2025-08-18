package starter;

public class LinkedIndexedListTest extends IndexedListTest {
  @Override
  protected IndexedList<Integer> createUnit(int length, int value) {
    return new LinkedIndexedList<>(length, value);
  }
}
