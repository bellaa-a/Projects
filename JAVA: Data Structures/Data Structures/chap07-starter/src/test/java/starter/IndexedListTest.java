package starter;

import exceptions.IndexException;
import exceptions.LengthException;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.fail;

abstract class IndexedListTest {
  protected final static int size = 5; //should be all capitalized
  protected final static int defaultValue = 10;
  private IndexedList<Integer> numbers;

  protected abstract IndexedList<Integer> createUnit(int length, int value);

  @BeforeEach
  void setUp() {
    numbers = createUnit(size, defaultValue);
  }

  @Test
  @DisplayName("constructor throws exception if size is <= 0.")
  void testConstructorThrowsExceptionForInvalidSize() {
    try {
      numbers = createUnit(-size, defaultValue);
      fail("LengthException was not thrown where it was expected!");
    } catch (LengthException ex) {
      return;
    }
  }

  @Test
  @DisplayName("get() returns the default value after IndexedList is instantiated.")
  void testGetAfterConstruction() {
    if (numbers.length() != size) {
      fail();
    }
    for (int index = 0; index < numbers.length(); index++) {
      assertEquals(defaultValue, numbers.get(index));
    }
  }

  @Test
  @DisplayName("get() throws exception if index is below the valid range.")
  void testGetWithIndexBelowRangeThrowsException() {
    try {
      numbers.get(-1);
      fail("IndexException was not thrown for index < 0");
    } catch (IndexException ex) {
      return;
    }
  }

  @Test
  @DisplayName("get() throws exception if index is above the valid range.")
  void testGetWithIndexAboveRangeThrowsException() {
    try {
      numbers.get(size + 1);
      fail("IndexException was not thrown for index > length");
    } catch (IndexException ex) {
      return;
    }
  }

  @Test
  @DisplayName("put() throws exception if index is below the valid range.")
  void testPutWithIndexBelowRangeThrowsException() {
    try {
      numbers.put(-1, 10);
      fail("IndexException was not thrown for index < 0");
    } catch (IndexException ex) {
      return;
    }
  }

  @Test
  @DisplayName("put() throws exception if index is above the valid range.")
  void testPutWithIndexAboveRangeThrowsException() {
    try {
      numbers.put(size + 1, 10);
      fail("IndexException was not thrown for index > length");
    } catch (IndexException ex) {
      return;
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
  @DisplayName("test iterator after IndexedList is instantiated.")
  void testEnhancedLoopAfterConstruction() {
    int counter = 0;
    for (int element : numbers) {
      assertEquals(defaultValue, element);
      counter++;
    }
    assertEquals(size, counter);
  }
}
