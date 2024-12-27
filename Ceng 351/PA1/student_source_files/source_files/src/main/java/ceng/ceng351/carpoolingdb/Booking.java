package ceng.ceng351.carpoolingdb;

public class Booking {

    private int TripID;
    private int PIN; // Passenger's PIN
    private String booking_status;

    public Booking(int TripID, int PIN, String booking_status) {
        this.TripID = TripID;
        this.PIN = PIN;
        this.booking_status = booking_status;
    }

    public int getTripID() {
        return TripID;
    }

    public void setTripID(int TripID) {
        this.TripID = TripID;
    }

    public int getPIN() {
        return PIN;
    }

    public void setPIN(int PIN) {
        this.PIN = PIN;
    }

    public String getBooking_status() {
        return booking_status;
    }

    public void setBooking_status(String booking_status) {
        this.booking_status = booking_status;
    }

    @Override
    public String toString() {
        return "Booking{" +
                "TripID=" + TripID +
                ", PIN=" + PIN +
                ", booking_status='" + booking_status + '\'' +
                '}';
    }
}
