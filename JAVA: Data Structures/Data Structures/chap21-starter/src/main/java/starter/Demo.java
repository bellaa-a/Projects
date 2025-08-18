package starter;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class Demo {

  /**
   * Program execution starts here.
   * @param args command-line arguments.
   */
  public static void main(String[] args) {
    List<Integer> values = new ArrayList<>(Arrays.asList(1, 2, 3, 4, 5, 6));
    List<List<Integer>> permutations = getPermutations(values);
    int optimal = (int) Math.floor(Math.log(values.size()) / Math.log(2));
    int best = 0;
    int worst = 0;
    for (List<Integer> perm : permutations) {
      BinarySearchTree bst = new BinarySearchTree();
      for (Integer value: perm) {
        bst.insert(value);
      }
      int height = bst.getHeight();
      System.out.println("Input: " + perm + ", BST Height: " + height
          + ", Balanced? " + bst.isBalanced());

      best = height <= optimal ? best + 1 : best;
      worst = height >= values.size() - 1 ? worst + 1 : worst;
    }

    report(permutations, best, worst);
  }

  private static void report(List<List<Integer>> permutations,
                             int best, int worst) {
    System.out.println("\n#BSTs " + permutations.size()
        + "\n#BSTs with O(log N) height: " + best
        + "\n#BSTs with O(N) height: " + worst);
  }

  private static List<List<Integer>> getPermutations(List<Integer> values) {
    List<List<Integer>> all = new ArrayList<>();
    permute(values, new ArrayList<>(), all);
    return all;
  }

  private static void permute(List<Integer> data, List<Integer> perm,
                               List<List<Integer>> all) {
    if (data.size() == 0) {
      all.add(new ArrayList<>(perm));
    } else {
      for (int i = 0; i < data.size(); i++) {
        Integer val = data.remove(i);
        perm.add(val);
        permute(data, perm, all);
        perm.remove(val);
        data.add(i, val);
      }
    }
  }
}
