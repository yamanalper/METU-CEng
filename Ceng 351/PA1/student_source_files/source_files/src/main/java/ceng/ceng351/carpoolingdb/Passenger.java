package ceng.ceng351.carpoolingdb;

public class Passenger {

    private int PIN; // References Participant's PIN
    private String membership_status;

    public Passenger(int PIN, String membership_status) {
        this.PIN = PIN;
        this.membership_status = membership_status;
    }

    public int getPIN() {
        return PIN;
    }

    public void setPIN(int PIN) {
        this.PIN = PIN;
    }

    public String getMembership_status() {
        return membership_status;
    }

    public void setMembership_status(String membership_status) {
        this.membership_status = membership_status;
    }

    @Override
    public String toString() {
        return "Passenger{" +
                "PIN=" + PIN +
                ", membership_status='" + membership_status + '\'' +
                '}';
    }
}
