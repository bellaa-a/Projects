package starter;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.fail;

class IndexedListTest {
  private IndexedList<Integer> numbers;
  private final static int size = 5;
  private final static int defaultValue = 10;

  @BeforeEach
  void setUp() {
    numbers = new ArrayIndexedList<>(size, defaultValue);
  }

  @Test
  @DisplayName("get() returns the default value after IndexedList is instantiated.")
  void testGetAfterConstruction() {
    for (int index = 0; index < numbers.length(); index++) {
      assertEquals(defaultValue, numbers.get(index));
    }
  }

  @Test
  @DisplayName("put() changes the default value after IndexedList is instantiated.")
  void testPutChangesValueAfterConstruction() {
    numbers.put(2, 7);
    assertEquals(7, numbers.get(2));
  }

  @Test
  @DisplayName("put() overwrites the existing value at given index to provided value.")
  void testPutUpdatesValueAtGivenIndex() {
    numbers.put(1, 8);
    assertEquals(8, numbers.get(1));
    numbers.put(1, -5);
    assertEquals(-5, numbers.get(1));
  }

  @Test
  @DisplayName("length() returns the correct size after IndexedList is instantiated.")
  void testLengthAfterConstruction() {
    assertEquals(size, numbers.length());
  }

  @Test
  @DisplayName("get() will throw exception if condition is violated.")
  void testException() {
    try {
      numbers.get(-1);
      fail("Exception was not thrown.");
    } catch (IndexOutOfBoundsException ex) {
      return;
    }
  }
}
