package starter;

public class InsertionSortTest extends SortingAlgorithmTest {
  @Override
  public SortingAlgorithm<Integer> createSortingAlgorithm() {
    return new InsertionSort<>();
  }
}
