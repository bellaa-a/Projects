package starter;

public abstract class Roster {
  private Student[] students;
  private int numStudents;

  public Roster(int size) { //called by subclasses by super()
    students = new Student[size];
    numStudents = 0;
  }

  public abstract void add(Student s); //no curly braces for abstract methods

  public abstract void remove(Student s);

  public abstract Student find(String email);

  public int getNumStudents() {
    return numStudents;
  }

}