package starter;

public interface Roster_interface { //Can only have abstract methods
    public abstract void add(Student s);

    public abstract void remove(Student s);

    public abstract Student find(String email);
}
