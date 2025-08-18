package starter.exercise;

@SuppressWarnings("all")
public class IteratorExercise {
  public static void main(String[] args) {
    Roster myRoster = new MoocRoster(10);
    myRoster.add(new Student("John Doe", "john@email.com"));
    myRoster.add(new Student("Jane Doe", "jane@email.com"));
    myRoster.add(new Student("Johnny Roe", "john@email.com"));
    myRoster.add(new Student("Janie Roe", "john@email.com"));

//    for(Student s: myRoster) {
//      System.out.println(s);
//    }
  }
}
