package starter.exercise;

@SuppressWarnings("all")
public class Student {
  private final String name;
  private final String email;

  public Student(String name, String email) {
    this.name = name;
    this.email = email;
  }

  public String getName() {
    return name;
  }

  public String getEmail() {
    return email;
  }

  @Override
  public String toString() {
    return name + " (" + email + ")";
  }
}
