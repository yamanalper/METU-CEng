package ceng.ceng351.carpoolingdb;

import java.util.Locale;

public class Driver {

    private int PIN; // References Participant's PIN
    private double rating;

    public Driver(int PIN, double rating) {
        this.PIN = PIN;
        this.rating = rating;
    }

    public int getPIN() {
        return PIN;
    }

    public void setPIN(int PIN) {
        this.PIN = PIN;
    }

    public double getRating() {
        return rating;
    }

    public void setRating(double rating) {
        this.rating = rating;
    }

    @Override
    public String toString() {
        return "Driver{" +
                "PIN=" + PIN +
                ", rating=" + String.format(Locale.US, "%.1f", rating) +
                '}';
    }
}
