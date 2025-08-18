package starter;

public class MoocRoster2 implements Roster_interface{

    //Have to declare variables
    private Student[] students;
    private int numStudents;

    public MoocRoster2(int size) { //called by subclasses by super()
        students = new Student[size];
        numStudents = 0;
    }

    @Override
    public void add(Student s) {

    }

    @Override
    public void remove(Student s) {

    }

    @Override
    public Student find(String email) {
        return null;
    }
}
