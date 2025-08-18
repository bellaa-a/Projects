package starter;

public class BubbleSortTest extends SortingAlgorithmTest {
  @Override
  public SortingAlgorithm<Integer> createSortingAlgorithm() {
    return new BubbleSort<>();
  }
}
