package starter;


public class BstOrderedSetTest extends OrderedSetTest {
  @Override
  protected Set<String> createSet() {
    return new BstOrderedSet<>();
  }
}
