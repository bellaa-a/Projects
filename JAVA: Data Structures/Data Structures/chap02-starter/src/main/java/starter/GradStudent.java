package starter;

public class GradStudent extends Student{
  private String advisor;

  //Constructors are NOT inherited
  public GradStudent(String name, String email) {
    super(name, email); //has to be the first call
    //or use "setters" if there is any

    //Can't do this bc doesnt have access
    //this.name = name;
    //this.email = email;
  }

  public GradStudent(String name, String email, String advisor) {
    super(name, email); //has to be the first call
    this.advisor = advisor;
  }

  public String getAdvisor() {
    return advisor;
  }

  public void setAdvisor(String advisor) {
    this.advisor = advisor;
  }

  public String toString() {
    return getName() + getEmail();

    //still can;t access; use super when we override
    //return super.name + super.email;

    //can't do this bc name and email are private
    //return name + email;
  }
}

