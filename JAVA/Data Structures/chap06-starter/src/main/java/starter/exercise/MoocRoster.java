package starter.exercise;

@SuppressWarnings("all")
public class MoocRoster implements Roster {
  private final Student[] students;
  private int numStudents;

  public MoocRoster(int size) {
    students = new Student[size];
    numStudents = 0;
  }

  @Override
  public void add(Student s) {
    if (numStudents < students.length) {
      students[numStudents++] = s;
    }
  }

  @Override
  public void remove(Student s) {
    // stub - leave it as is
  }

  @Override
  public Student find(String email) {
    return null; // stub - leave it as is
  }
}
