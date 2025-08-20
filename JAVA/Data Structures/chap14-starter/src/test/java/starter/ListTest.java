package starter;

import exceptions.EmptyException;
import exceptions.PositionException;
import org.junit.jupiter.api.*;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Iterator;
import java.util.NoSuchElementException;

import static org.junit.jupiter.api.Assertions.*;

@SuppressWarnings("All")
public abstract class ListTest {
  protected List<String> list;

  @BeforeEach
  public void setup() {
    list = createList();
  }

  protected abstract List<String> createList();

  @Test
  @DisplayName("new list is empty")
  public void newListEmpty() {
    assertTrue(list.empty());
    assertEquals(0, list.length());

    countElementUsingIterator(0);
  }

  @Test
  @DisplayName("Calling front on an empty list throws exception")
  public void newListNoFront() {
    try {
      list.front();
      fail("Expected exception was not thrown");
    } catch (EmptyException e) {
      return;
    }
  }

  @Test
  @DisplayName("calling back on an empty list throws exception")
  public void newListNoBack() {
    try {
      list.back();
      fail("Expected exception was not thrown");
    } catch (EmptyException e) {
      return;
    }
  }

  @Test
  @DisplayName("Attempting to remove front on an empty list throws exception")
  public void newListRemoveFront() {
    try {
      list.removeFront();
      fail("Expected exception was not thrown");
    } catch (EmptyException e) {
      return;
    }
  }

  @Test
  @DisplayName("Attempting to remove back on an empty list throws exception")
  public void newListRemoveBack() {
    try {
      list.removeBack();
      fail("Expected exception was not thrown");
    } catch (EmptyException e) {
      return;
    }
  }

  @Test
  @DisplayName("Calling next() on empty list iterator throws exception")
  public void testIteratorNext_emptyList() {
    Iterator<String> it = list.iterator();
    try {
      it.next();
      fail("Expected exception was not thrown");
    } catch (NoSuchElementException e) {
      return;
    }
  }

  @Test
  @DisplayName("hasNext() returns false on empty list iterator")
  public void testIteratorHasNext_emptyList() {
    Iterator<String> it = list.iterator();
    assertFalse(it.hasNext());
  }

  @Test
  @DisplayName("insertFront on empty list")
  public void insertFrontWorks_oneElement() {
    Position<String> one = list.insertFront("One");
    // [One]
    assertFalse(list.empty());
    assertEquals(1, list.length());
    assertEquals(one, list.front());
    assertEquals(one, list.back());
    assertTrue(list.first(one));
    assertTrue(list.last(one));
    countElementUsingIterator(1);
  }

  @Test
  @DisplayName("insertFront on non-empty list")
  public void insertFrontWorks_multipleElements() {
    Position<String> one = list.insertFront("One");
    Position<String> two = list.insertFront("Two");
    Position<String> three = list.insertFront("Three");
    // [Three, Two, One]
    assertFalse(list.empty());
    assertEquals(3, list.length());
    assertEquals(three, list.front());
    assertEquals(one, list.back());
    assertTrue(list.first(three));
    assertTrue(list.last(one));
    countElementUsingIterator(3);
  }

  @Test
  @DisplayName("insertBack on empty list")
  public void insertBackWorks_oneElement() {
    Position<String> one = list.insertBack("One");
    // [One]
    assertFalse(list.empty());
    assertEquals(1, list.length());
    assertEquals(one, list.front());
    assertEquals(one, list.back());
    assertTrue(list.first(one));
    assertTrue(list.last(one));
    countElementUsingIterator(1);
  }

  @Test
  @DisplayName("insertBack on non-empty list")
  public void insertBackWorks_multipleElements() {
    Position<String> one = list.insertBack("One");
    Position<String> two = list.insertBack("Two");
    Position<String> three = list.insertBack("Three");
    // [One, Two, Three]
    assertFalse(list.empty());
    assertEquals(3, list.length());
    assertEquals(one, list.front());
    assertEquals(three, list.back());
    assertTrue(list.first(one));
    assertTrue(list.last(three));
    countElementUsingIterator(3);
  }

  @Test
  @DisplayName("multiple insertAfter on empty list")
  public void insertAfterWorks() {
    Position<String> one = list.insertBack("One");
    Position<String> three = list.insertBack("Three");
    Position<String> two = list.insertAfter(one, "Two");
    // [One, Two, Three]
    assertFalse(list.empty());
    assertEquals(3, list.length());
    assertEquals(one, list.front());
    assertEquals(three, list.back());
    assertTrue(list.first(one));
    assertTrue(list.last(three));
    countElementUsingIterator(3);
  }

  @Test
  @DisplayName("insertAfter after insertFront")
  public void insertAfterSpecialCase() {
    Position<String> one = list.insertFront("One");
    Position<String> two = list.insertAfter(one, "Two");
    // [One, Two]
    assertFalse(list.empty());
    assertEquals(2, list.length());
    assertEquals(one, list.front());
    assertEquals(two, list.back());
    assertTrue(list.first(one));
    assertTrue(list.last(two));
    countElementUsingIterator(2);
  }

  @Test
  @DisplayName("insertAfter given null position throws exception")
  public void insertAfterThrowsException_nullPosition() {
    try {
      list.insertAfter(null, "One");
      fail("Expected exception was not thrown");
    } catch (PositionException e) {
      return;
    }
  }

  @Test
  @DisplayName("insertAfter given invalid position throws exception")
  public void insertAfterThrowsException_PositionNotInThisList() {
    List<String> other = createList();
    Position<String> p = other.insertBack("One");
    try {
      list.insertAfter(p, "Two");
      fail("Expected exception was not thrown");
    } catch (PositionException e) {
      return;
    }
  }

  @Test
  @DisplayName("multiple insertBefore on empty list")
  public void insertBeforeWorks() {
    Position<String> one = list.insertBack("One");
    Position<String> three = list.insertBack("Three");
    Position<String> two = list.insertBefore(three, "Two");
    // [One, Two, Three]
    assertFalse(list.empty());
    assertEquals(3, list.length());
    assertEquals(one, list.front());
    assertEquals(three, list.back());
    assertTrue(list.first(one));
    assertTrue(list.last(three));
    countElementUsingIterator(3);
  }

  @Test
  @DisplayName("insertBefore after insertBack")
  public void insertBeforeSpecialCase() {
    Position<String> two = list.insertBack("Two");
    Position<String> one = list.insertBefore(two, "One");
    // [One, Two]
    assertFalse(list.empty());
    assertEquals(2, list.length());
    assertEquals(one, list.front());
    assertEquals(two, list.back());
    assertTrue(list.first(one));
    assertTrue(list.last(two));
    countElementUsingIterator(2);
  }

  @Test
  @DisplayName("insertBefore given null position throws exception")
  public void insertBeforeThrowsException_nullPosition() {
    try {
      list.insertBefore(null, "One");
      fail("Expected exception was not thrown");
    } catch (PositionException e) {
      return;
    }
  }

  @Test
  @DisplayName("insertBefore given invalid position throws exception")
  public void insertBeforeThrowsException_PositionNotInThisList() {
    List<String> other = createList();
    Position<String> p = other.insertBack("Two");
    try {
      list.insertBefore(p, "One");
      fail("Expected exception was not thrown");
    } catch (PositionException e) {
      return;
    }
  }

  @Test
  @DisplayName("previous() works!")
  public void previousWorks() {
    Position<String> one = list.insertBack("One");
    Position<String> two = list.insertBack("Two");
    Position<String> three = list.insertBack("Three");
    // [One, Two, Three]
    assertEquals(two, list.previous(three));
    assertEquals(one, list.previous(two));
  }

  @Test
  @DisplayName("previous() throws exception when called on only element")
  public void previousThrowsException_lastPosition() {
    Position<String> one = list.insertBack("One");
    // [One]
    try {
      list.previous(one);
      fail("Expected exception was not thrown");
    } catch (PositionException e) {
      return;
    }
  }

  @Test
  @DisplayName("previous() throws exception given invalid position")
  public void previousThrowsException_invalidPosition() {
    List<String> other = createList();
    Position<String> one = other.insertBack("One");
    Position<String> two = other.insertBack("Two");
    try {
      list.previous(two);
      fail("Expected exception was not thrown");
    } catch (PositionException e) {
      return;
    }
  }

  @Test
  @DisplayName("next() works!")
  public void nextWorks() {
    Position<String> one = list.insertBack("One");
    Position<String> two = list.insertBack("Two");
    Position<String> three = list.insertBack("Three");
    // [One, Two, Three]
    assertEquals(two, list.next(one));
    assertEquals(three, list.next(two));
  }

  @Test
  @DisplayName("next() throws exception when called on only element")
  public void nextThrowsException_lastPosition() {
    Position<String> one = list.insertBack("One");
    // [One]
    try {
      list.next(one);
      fail("Expected exception was not thrown");
    } catch (PositionException e) {
      return;
    }
  }

  @Test
  @DisplayName("next() throws exception when given invalid position")
  public void nextThrowsException_invalidPosition() {
    List<String> other = createList();
    Position<String> one = other.insertBack("One");
    Position<String> two = other.insertBack("Two");

    try {
      list.next(one);
      fail("Expected exception was not thrown");
    } catch (PositionException e) {
      return;
    }
  }

  @Test
  @DisplayName("test forward interator")
  public void iterateForward() {
    ArrayList<String> items = new ArrayList<>(Arrays.asList("One", "Two"));

    for (String item : items) {
      list.insertBack(item);
    }

    int counter = 0;
    Iterator<String> it = list.forward();
    while (it.hasNext()) {
      assertEquals(items.get(counter++), it.next());
    }
  }

  @Test
  @DisplayName("test backward interator")
  public void iterateBackward() {
    ArrayList<String> items = new ArrayList<>(Arrays.asList("One", "Two"));

    for (String item : items) {
      list.insertFront(item);
    }

    int counter = 0;
    Iterator<String> it = list.backward();
    while (it.hasNext()) {
      assertEquals(items.get(counter++), it.next());
    }
  }

  @Test
  @DisplayName("remove front on non-empty list")
  public void removeFrontWorks() {
    Position<String> one = list.insertFront("One");
    Position<String> two = list.insertFront("Two");
    Position<String> three = list.insertFront("Three");
    // [Three, Two, One]
    list.removeFront();
    list.removeFront();
    // [One]
    assertFalse(list.empty());
    assertEquals(1, list.length());
    assertTrue(list.first(one));
    assertTrue(list.last(one));
    assertEquals(one, list.front());
    assertEquals(one, list.back());

    countElementUsingIterator(1);
  }

  @Test
  @DisplayName("remove back on non-empty list")
  public void removeBackWorks() {
    Position<String> one = list.insertFront("One");
    Position<String> two = list.insertFront("Two");
    Position<String> three = list.insertFront("Three");
    // [Three, Two, One]
    list.removeBack();
    list.removeBack();
    // [Three]
    assertFalse(list.empty());
    assertEquals(1, list.length());
    assertTrue(list.first(three));
    assertTrue(list.last(three));
    assertEquals(three, list.front());
    assertEquals(three, list.back());

    countElementUsingIterator(1);
  }


  @Test
  @DisplayName("remove on non-empty list")
  public void removeWorks() {
    Position<String> one = list.insertFront("One");
    Position<String> two = list.insertFront("Two");
    Position<String> three = list.insertFront("Three");
    // [Three, Two, One]
    list.remove(two);
    // [Three, One]
    assertEquals(2, list.length());
    assertEquals(three, list.front());
    assertEquals(one, list.back());
    assertTrue(list.first(three));
    assertTrue(list.last(one));

    countElementUsingIterator(2);
  }

  /**
   * By using iterator, we can make sure the "next" and "prev"
   * pointers are set correctly.
   *
   * @param expectedCount, i.e. length of the list
   */
  private void countElementUsingIterator(int expectedCount) {
    int counter = 0;
    for (String s : list) {
      counter++;
    }

    int countForward = 0;
    Iterator<String> it = list.forward();
    while (it.hasNext()) {
      it.next();
      countForward++;
    }

    int countBackward = 0;
    it = list.backward();
    while (it.hasNext()) {
      it.next();
      countBackward++;
    }

    assertEquals(expectedCount, counter);
    assertEquals(expectedCount, countForward);
    assertEquals(expectedCount, countBackward);
  }
}

