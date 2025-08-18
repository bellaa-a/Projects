package starter;

import org.junit.jupiter.api.*;
import static org.junit.jupiter.api.Assertions.*;

//interface plus "Test"
class IndexedListTest {

  private IndexedList<Integer> testList;
  private final static int SIZE = 5; //can change here and changes throughout whole code
  private final static int DEFAULT_VALUE = 1; //usually constants are static

  @BeforeEach //Runs before each method
  void setup() {
    testList = new ArrayIndexedList<>(SIZE, DEFAULT_VALUE);
  }

  @Test
  @DisplayName("Length() returns the correct value after IndexedList is instantiated.")
  void testLengthAfterConstruction() {
    //IndexedList<Integer> testList = new ArrayIndexedList<>(5, 10);
    assertEquals(testList.length(), 5);
  }

  @Test //Indicate that the method below is a test
  @DisplayName("get() returns the default value after IndexedList is instantiated.")
  void testGetAfterConstruction() {
    //IndexedList<Integer> testList = new ArrayIndexedList<>(5, 10);
    for (int i = 0; i < testList.length(); i++) {
      assertEquals(testList.get(i), 10);
    }
  }

  @Test
  @DisplayName("put() changes the default value after IndexedList is instantiated.")
  void testPutChangesValueAfterConstruction() {
    //IndexedList<Integer> testList = new ArrayIndexedList<>(5, 10);
    testList.put(1, 8);
    assertEquals(testList.get(1), 8);
  }

  @Test
  @DisplayName("put() overwrites the existing value at given index to provided value.")
  void testPutUpdatesValueAtGivenIndex() {
    //IndexedList<Integer> testList = new ArrayIndexedList<>(5, 10);
    testList.put(1, 8);
    assertEquals(testList.get(1), 8);
    testList.put(1, -5);
    assertEquals(testList.get(1), -5);

  }
}