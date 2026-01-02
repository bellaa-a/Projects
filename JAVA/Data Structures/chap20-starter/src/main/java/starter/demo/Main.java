package starter.demo;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;
import java.util.Random;

@SuppressWarnings("All")
public class Main {
  private static final int MAX_SUBMISSIONS = 10;

  /**
   * Run the demo.
   * @param args command-line arguments.
   */
  public static void main(String[] args) {
    List<String> students = getStudents();
    List<Submission> submissions = getSubmissions(students);

//    System.out.print("\nSubmission:");
//    System.out.println(submissions);

//    System.out.print("\nSubmission sorted (natural ordering):");
//    Collections.sort(submissions);
//    System.out.println(submissions);
//
    System.out.print("\nSubmission sorted (priority ordering):");
    Collections.sort(submissions, new LessSubmissionHigherPriority());
    System.out.println(submissions);
  }

  private static List<Submission> getSubmissions(List<String> students) {
    Random random = new Random();
    List<Submission> submissions = new ArrayList<>();
    for (int i = 0; i < MAX_SUBMISSIONS; i++) {
      submissions.add(new Submission(students.get(i), random.nextInt(10)));
    }
    Collections.shuffle(submissions);
    return submissions;
  }

  private static List<String> getStudents() {
    List<String> students = new ArrayList<>();
    for (int i = 0; i < MAX_SUBMISSIONS; i++) {
      students.add(String.format("%s%2d", "student", i + 1));
    }
    Collections.shuffle(students);
    return students;
  }

  private static class LessSubmissionHigherPriority
      implements Comparator<Submission> { // Comparable

    @Override
    public int compare(Submission s1, Submission s2) {
//      return s1.compareTo(s2);
      return s1.getNumPriorSubmission() - s2.getNumPriorSubmission();
    }
  }
}
