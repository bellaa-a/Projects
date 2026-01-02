package starter.demo;

import java.util.concurrent.atomic.AtomicInteger;

@SuppressWarnings("All")
public class Submission implements Comparable<Submission> {

  @Override
  public int compareTo(Submission other) {
    return this.positionInQueue - other.positionInQueue;
//    return this.numPriorSubmission - other.numPriorSubmission;
  }

  private static final AtomicInteger COUNT = new AtomicInteger(0);
  private int positionInQueue;
  private String student;
  private int numPriorSubmission;

  /**
   * Create a submission for the given student.
   * @param student name of the student who made this submission.
   * @param numPriorSubmission number of prior submission for this homework by this student.
   */
  public Submission(String student, int numPriorSubmission) {
    this.student = student;
    this.numPriorSubmission = numPriorSubmission;
    // all submissions are positioned one after another in order they are received.
    this.positionInQueue = COUNT.incrementAndGet();
  }

  public int getNumPriorSubmission() {
    return numPriorSubmission;
  }



  @Override
  public String toString() {
    return "\n\t{position=" + positionInQueue + ", numPriorSubmission=" + numPriorSubmission + "}";
  }
}

