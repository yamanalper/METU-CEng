package ceng.ceng351.carpoolingdb;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;

/**
 * FileOperations class for reading data files and writing output files.
 */
public class FileOperations {

    public static FileWriter createFileWriter(String path) throws IOException {
        File f = new File(path);

        FileWriter fileWriter = null;

        if (f.isDirectory() && !f.exists())
            f.mkdirs();
        else if (!f.isDirectory() && !f.getParentFile().exists())
            f.getParentFile().mkdirs();

        if (!f.isDirectory() && f.exists())
            f.delete();

        fileWriter = new FileWriter(f, false);

        return fileWriter;
    }

    // Helper method to get BufferedReader from resource
    private static BufferedReader getBufferedReader(String resourcePath) {
        InputStream inputStream = FileOperations.class.getClassLoader().getResourceAsStream(resourcePath);

        if (inputStream == null) {
            return null;
        }

        return new BufferedReader(new InputStreamReader(inputStream));
    }

    public static Participant[] readParticipantsFile() {
        List<Participant> participantList = new ArrayList<>();

        try (BufferedReader bufferedReader = getBufferedReader("data/Participants.txt")) {

            if (bufferedReader == null) {
                System.err.println("Participants data file not found.");
                return new Participant[0];
            }

            String strLine;

            // Example strLine: PIN    p_name    age

            // Skip the header line
            bufferedReader.readLine();

            while ((strLine = bufferedReader.readLine()) != null) {
                // Parse strLine
                String[] words = strLine.split("\t");

                if (words.length < 3) {
                    System.out.println("There is a problem in Participants File Reading phase");
                } else {
                    try {
                        int PIN = Integer.parseInt(words[0]);
                        String p_name = words[1];
                        int age = Integer.parseInt(words[2]);

                        Participant participant = new Participant(PIN, p_name, age);
                        participantList.add(participant);
                    } catch (NumberFormatException e) {
                        System.err.println("Invalid data format in Participants file: " + strLine);
                        e.printStackTrace();
                    }
                }
            }


        } catch (Exception e) {
            e.printStackTrace();
        }

        return participantList.toArray(new Participant[0]);
    }

    public static Passenger[] readPassengersFile() {
        List<Passenger> passengerList = new ArrayList<>();

        try (BufferedReader bufferedReader = getBufferedReader("data/Passengers.txt")) {

            if (bufferedReader == null) {
                System.err.println("Passengers data file not found.");
                return new Passenger[0];
            }

            String strLine;

            // Example strLine: PIN    membership_status

            // Skip the header line
            bufferedReader.readLine();

            while ((strLine = bufferedReader.readLine()) != null) {
                // Parse strLine
                String[] words = strLine.split("\t");

                if (words.length < 2) {
                    System.out.println("There is a problem in Passengers File Reading phase");
                } else {
                    int PIN = Integer.parseInt(words[0]);
                    String membership_status = words[1];

                    Passenger passenger = new Passenger(PIN, membership_status);
                    passengerList.add(passenger);
                }

            } // End of while

        } catch (Exception e) {
            e.printStackTrace();
        }

        return passengerList.toArray(new Passenger[0]);
    }

    public static Driver[] readDriversFile() {
        List<Driver> driverList = new ArrayList<>();

        try (BufferedReader bufferedReader = getBufferedReader("data/Drivers.txt")) {

            if (bufferedReader == null) {
                System.err.println("Drivers data file not found.");
                return new Driver[0];
            }

            String strLine;

            // Example strLine: PIN    rating

            // Skip the header line
            bufferedReader.readLine();

            while ((strLine = bufferedReader.readLine()) != null) {
                // Parse strLine
                String[] words = strLine.split("\t");

                if (words.length < 2) {
                    System.out.println("There is a problem in Drivers File Reading phase");
                } else {
                    int PIN = Integer.parseInt(words[0]);
                    double rating = Double.parseDouble(words[1]);

                    Driver driver = new Driver(PIN, rating);
                    driverList.add(driver);
                }

            } // End of while

        } catch (Exception e) {
            e.printStackTrace();
        }

        return driverList.toArray(new Driver[0]);
    }

    public static Car[] readCarsFile() {
        List<Car> carList = new ArrayList<>();

        try (BufferedReader bufferedReader = getBufferedReader("data/Cars.txt")) {

            if (bufferedReader == null) {
                System.err.println("Cars data file not found.");
                return new Car[0];
            }

            String strLine;

            // Example strLine: CarID    PIN    color    brand

            // Skip the header line
            bufferedReader.readLine();

            while ((strLine = bufferedReader.readLine()) != null) {
                // Parse strLine
                String[] words = strLine.split("\t");

                if (words.length < 4) {
                    System.out.println("There is a problem in Cars File Reading phase");
                } else {
                    int CarID = Integer.parseInt(words[0]);
                    int PIN = Integer.parseInt(words[1]);
                    String color = words[2];
                    String brand = words[3];

                    Car car = new Car(CarID, PIN, color, brand);
                    carList.add(car);
                }

            } // End of while

        } catch (Exception e) {
            e.printStackTrace();
        }

        return carList.toArray(new Car[0]);
    }

    public static Trip[] readTripsFile() {
        List<Trip> tripList = new ArrayList<>();

        try (BufferedReader bufferedReader = getBufferedReader("data/Trips.txt")) {

            if (bufferedReader == null) {
                System.err.println("Trips data file not found.");
                return new Trip[0];
            }

            String strLine;

            // Example strLine: TripID    CarID    date    departure    destination    num_seats_available

            // Skip the header line
            bufferedReader.readLine();

            while ((strLine = bufferedReader.readLine()) != null) {
                // Parse strLine
                String[] words = strLine.split("\t");

                if (words.length < 6) {
                    System.out.println("There is a problem in Trips File Reading phase");
                } else {
                    int TripID = Integer.parseInt(words[0]);
                    int CarID = Integer.parseInt(words[1]);
                    String date = words[2];
                    String departure = words[3];
                    String destination = words[4];
                    int num_seats_available = Integer.parseInt(words[5]);

                    Trip trip = new Trip(TripID, CarID, date, departure, destination, num_seats_available);
                    tripList.add(trip);
                }

            } // End of while

        } catch (Exception e) {
            e.printStackTrace();
        }

        return tripList.toArray(new Trip[0]);
    }

    public static Booking[] readBookingsFile() {
        List<Booking> bookingList = new ArrayList<>();

        try (BufferedReader bufferedReader = getBufferedReader("data/Bookings.txt")) {

            if (bufferedReader == null) {
                System.err.println("Bookings data file not found.");
                return new Booking[0];
            }

            String strLine;

            // Example strLine: TripID    PIN    booking_status

            // Skip the header line
            bufferedReader.readLine();

            while ((strLine = bufferedReader.readLine()) != null) {
                // Parse strLine
                String[] words = strLine.split("\t");

                if (words.length < 3) {
                    System.out.println("There is a problem in Bookings File Reading phase");
                } else {
                    int TripID = Integer.parseInt(words[0]);
                    int PIN = Integer.parseInt(words[1]);
                    String booking_status = words[2];

                    Booking booking = new Booking(TripID, PIN, booking_status);
                    bookingList.add(booking);
                }

            } // End of while

        } catch (Exception e) {
            e.printStackTrace();
        }

        return bookingList.toArray(new Booking[0]);
    }
}
