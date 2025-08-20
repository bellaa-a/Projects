package starter;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

/**
 * Testing implementations of the Set interface.
 */
@SuppressWarnings("All")
public abstract class SetTest {
  protected Set<String> set;

  protected abstract Set<String> createSet();

  @BeforeEach
  void setupTests() {
    set = this.createSet();
  }

  @Test
  @DisplayName("Set is empty as soon as it is constructed")
  void newSetEmpty() {
    assertEquals(0, set.size());
  }

  @Test
  @DisplayName("Test iterator after construction")
  void newSetEmptyIterator() {
    int c = 0;
    for (String s : set) {
      c++;
    }
    assertEquals(0, c);
  }

  @Test
  @DisplayName("Set is not empty after insertion")
  public void insertNotEmpty() {
    set.insert("Magda");
    assertEquals(1, set.size());
  }

  @Test
  @DisplayName("Inserting duplicate values does not change set size")
  public void insertDuplicateSizeConsistent() {
    set.insert("one");
    set.insert("one");
    assertEquals(1, set.size());
  }

  @Test
  @DisplayName("Set contains the element which was inserted")
  public void insertHas() {
    assertFalse(set.has("one"));
    set.insert("one");
    assertTrue(set.has("one"));
  }

  @Test
  @DisplayName("Set does not contain the element which was removed")
  public void insertRemoveHas() {
    set.insert("one");
    set.remove("one");
    assertFalse(set.has("one"));
  }

  @Test
  @DisplayName("Complex set operation: many insert one remove")
  public void manyInsertOneRemove() {
    set.insert("one");
    set.insert("two");
    set.remove("one");
    set.insert("three");
    assertEquals(2, set.size());
    assertFalse(set.has("one"));
    assertTrue(set.has("two"));
    assertTrue(set.has("three"));
  }

  @Test
  @DisplayName("size operation works as expected")
  public void insertManySizeConsistent() {
    set.insert("one");
    set.insert("two");
    set.insert("three");
    assertEquals(3, set.size());
  }

  @Test
  @DisplayName("iterator works as expected")
  public void iteratorWorks() {
    String[] data = {"Peter", "Paul", "Mary", "Beverly"};
    for (String d : data) {
      set.insert(d);
    }
    for (String s : set) {
      int count = 0;
      for (String d : data) {
        if (s.equals(d)) {
          count += 1;
        }
      }
      assertEquals(1, count);
    }
  }
}
