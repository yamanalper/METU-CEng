package ceng.ceng351.carpoolingdb;

public class Trip {

    private int TripID;
    private int CarID;
    private String date;
    private String departure;
    private String destination;
    private int num_seats_available;

    public Trip(int TripID, int CarID, String date, String departure, String destination, int num_seats_available) {
        this.TripID = TripID;
        this.CarID = CarID;
        this.date = date;
        this.departure = departure;
        this.destination = destination;
        this.num_seats_available = num_seats_available;
    }

    public int getTripID() {
        return TripID;
    }

    public void setTripID(int TripID) {
        this.TripID = TripID;
    }

    public int getCarID() {
        return CarID;
    }

    public void setCarID(int CarID) {
        this.CarID = CarID;
    }

    public String getDate() {
        return date;
    }

    public void setDate(String date) {
        this.date = date;
    }
  
    public String getDeparture() {
        return departure;
    }
  
    public void setDeparture(String departure) {
        this.departure = departure;
    }
  
    public String getDestination() {
        return destination;
    }
  
    public void setDestination(String destination) {
        this.destination = destination;
    }
  
    public int getNum_seats_available() {
        return num_seats_available;
    }
  
    public void setNum_seats_available(int num_seats_available) {
        this.num_seats_available = num_seats_available;
    }
  
    @Override
    public String toString() {
        return "Trip{" +
                "TripID=" + TripID +
                ", CarID=" + CarID +
                ", date='" + date + '\'' +
                ", departure='" + departure + '\'' +
                ", destination='" + destination + '\'' +
                ", num_seats_available=" + num_seats_available +
                '}';
    }
}
