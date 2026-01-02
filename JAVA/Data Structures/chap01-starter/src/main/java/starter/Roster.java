package starter;

public class Roster {
    private Student[] students;
    private int numStudents;

    public Roster(int size) {
        students = new Student[size];
        numStudents = 0;
    }

    public void add(Student s) {
        // stub
        students[numStudents] = new Student(s.getName(), s.getEmail());
        numStudents++;
    }

    public void remove(Student s) {
        // stub
        numStudents--;
    }

    // Assumption: students' emails are unique.
    public Student linFind(String email) {
        //for every element in our student array
        for (int i = 0; i < numStudents; ++i) {
            //compare its email to the target email
            if (students[i].getEmail().equals(email)) {
                //if matches, return student
                return students[i];
            }
        }
        //if we made it here, student was not in roster
        return null;
    }

    public Student binFind(String email) {
        return binFind(email, 0, numStudents - 1);
    }

    //first and last are inclusive of the range
    private Student binFind(String email, int first, int last) {
        if (first > last) {
            return null;
        }
        int middle = (first + last)/2;
        int compare = students[middle].getEmail().compareTo(email);

        if (compare < 0) {
            return binFind(email, middle, last);
        } else if (compare > 0) {
            return binFind(email, first, middle);
        } else {
            return students[middle];
        }
    }
}
