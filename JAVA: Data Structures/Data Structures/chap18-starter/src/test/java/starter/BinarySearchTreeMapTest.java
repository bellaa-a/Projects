package starter;

@SuppressWarnings("All")
public class BinarySearchTreeMapTest extends OrderedMapTest {

  @Override
  protected Map createMap() {
    return new BinarySearchTreeMap();
  }
}