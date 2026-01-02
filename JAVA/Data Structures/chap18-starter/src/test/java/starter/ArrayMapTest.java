package starter;

public class ArrayMapTest extends MapTest {
  @Override
  protected Map createMap() {
    return new ArrayMap();
  }
}
