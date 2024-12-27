package ceng.ceng351.carpoolingdb;

public class Participant {

    private int PIN;
    private String p_name;
    private int age;

    public Participant(int PIN, String p_name, int age) {
        this.PIN = PIN;
        this.p_name = p_name;
        this.age = age;
    }

    public int getPIN() {
        return PIN;
    }

    public void setPIN(int PIN) {
        this.PIN = PIN;
    }

    public String getP_name() {
        return p_name;
    }

    public void setP_name(String p_name) {
        this.p_name = p_name;
    }

    public int getAge() {
        return age;
    }

    public void setAge(int age) {
         this.age = age;
    }

    @Override
    public String toString() {
        return "Participant{" +
                "PIN=" + PIN +
                ", p_name='" + p_name + '\'' +
                ", age=" + age +
                '}';
    }
}
