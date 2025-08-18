package starter;

public class SelectionSortTest extends SortingAlgorithmTest {
  @Override
  public SortingAlgorithm<Integer> createSortingAlgorithm() {
    return new SelectionSort<>();
  }
}
