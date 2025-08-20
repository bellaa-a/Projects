package starter.exercise;

@SuppressWarnings("all")
public interface Roster {

  void add(Student s);

  void remove(Student s);

  Student find(String email);
}
