package starter;

public class Student {
  private final String name;
  //private: inherited class can't access
  //protected: subclasses can access
  private final String email;

  public Student(String name, String email) {
    this.name = name; //uses "this" because one shadows the other
    this.email = email;
  }

  public String getName() {
    return name;
  }

  public String getEmail() {
    return email;
  }
}
