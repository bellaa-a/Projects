package starter;

import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;

public abstract class OrderedSetTest extends SetTest {

  @Override
  @Test
  @DisplayName("iterator works as expected")
  public void iteratorWorks() {
    String[] data = {"Peter", "Paul", "Mary", "Beverly"};
    String[] ordered = {"Beverly", "Mary", "Paul", "Peter"};
    for (String d : data) {
      set.insert(d);
    }

    int index = 0;
    for (String s : set) {
      assertEquals(ordered[index++], s);
    }
  }
}
