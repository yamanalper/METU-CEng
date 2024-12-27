package ceng.ceng351.carpoolingdb;

public class Car {

    private int CarID;
    private int PIN; // Driver's PIN
    private String color;
    private String brand;

    public Car(int CarID, int PIN, String color, String brand) {
        this.CarID = CarID;
        this.PIN = PIN;
        this.color = color;
        this.brand = brand;
    }

    public int getCarID() {
        return CarID;
    }

    public void setCarID(int CarID) {
        this.CarID = CarID;
    }

    public int getPIN() {
        return PIN;
    }

    public void setPIN(int PIN) {
        this.PIN = PIN;
    }

    public String getColor() {
        return color;
    }

    public void setColor(String color) {
        this.color = color;
    }
  
    public String getBrand() {
        return brand;
    }
  
    public void setBrand(String brand) {
        this.brand = brand;
    }
  
    @Override
    public String toString() {
        return "Car{" +
                "CarID=" + CarID +
                ", PIN=" + PIN +
                ", color='" + color + '\'' +
                ", brand='" + brand + '\'' +
                '}';
    }
}
