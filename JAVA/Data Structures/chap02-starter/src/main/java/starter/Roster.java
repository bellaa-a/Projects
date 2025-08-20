package starter;

public class Roster {
  private final Student[] students;
  private final int numStudents;

  //private GradStudent[] gradStudents;
  //private int numGradStudents;

  public Roster(int size) {
    students = new Student[size];
    numStudents = 0;
  }

  public void add(Student s) {
    // stub
    //can take as parameter any subclasses of Student as well
  }

  public void remove(Student s) {
    // stub
  }

  // Assumption: students' emails are unique
  public Student find(String email) {
    return null; // stub
  }

  private static String greetAdvisor(Student s) {
    // Use instanceof operator to check
    // if the object belongs to a specific type
    if (s instanceof GradStudent) {
      GradStudent gs = (GradStudent) s;
      return "Hi, " + gs.getAdvisor();
    } else {
      return "This student has no advisor!";
    }
  }

  //apparent type: declare a variable -> Student Jane
  //actual type: instantiate -> new GradStudent


  //public void addGrad(GradStudent s) { ... }
  //public void removeGrad(GradStudent s) { ... }
  //public GradStudent findGrad(String email) { ... }

}
