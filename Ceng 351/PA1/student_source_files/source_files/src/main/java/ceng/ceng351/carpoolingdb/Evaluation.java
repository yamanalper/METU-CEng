package ceng.ceng351.carpoolingdb;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.sql.*;
import java.io.File;
import java.util.Locale;

public class Evaluation {

    private static String user = "sa";          // H2 default username
    private static String password = "";        // H2 default password
    private static String url = "jdbc:h2:mem:carpoolingdb;DB_CLOSE_DELAY=-1"; // In-memory database
    private static Connection connection = null;

    public static void connect() {
        try {
            // Class.forName("org.h2.Driver"); // Not needed for H2
            connection = DriverManager.getConnection(url, user, password);
        } catch (SQLException e) {
            System.out.println("Cannot connect to database!");
            e.printStackTrace();
        }
    }

    public static void disconnect() {
        if (connection != null) {
            try {
                connection.close();
                // Clean up the database
                DriverManager.getConnection(url + ";INIT=DROP ALL OBJECTS", user, password).close();
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }
    }
    public static void addInputTitle(String title, BufferedWriter bufferedWriter) throws IOException {
        bufferedWriter.write("*** " + title + " ***" + System.lineSeparator());
    }

    public static void printLine(String result, BufferedWriter bufferedWriter) throws IOException {
        bufferedWriter.write(result + System.lineSeparator());
    }

    public static void addDivider(BufferedWriter bufferedWriter) throws IOException {
        bufferedWriter.write(System.lineSeparator() + "--------------------------------------------------------------" + System.lineSeparator());
    }


    public static void main(String[] args) {
    	

        int numberOfInsertions = 0;
        int numberOfTablesCreated = 0;
        int numberOfTablesDropped = 0;
        int numberOfRowsUpdated =0;

        FileWriter fileWriter = null;
        BufferedWriter bufferedWriter = null;

        // Connect to the database
        connect();

        // Initialize CarPoolingSystem object
        CarPoolingSystem carPoolingSystem = new CarPoolingSystem();
        carPoolingSystem.initialize(connection); // Pass the H2 connection

        String city = "New York"; 
        String city_Chicago="Chicago";
        String city_Seattle="Seattle";
        String date_NY_C = "2024-12-07";
        int driverPIN = 464159;
        String startDate="2024-12-05";
        String endDate="2024-12-15";
        int GivenTripID=1698;

        try {
            // Create output directory if it doesn't exist
            String outputDirectory = System.getProperty("user.dir") + System.getProperty("file.separator") + "output";
            File outputDir = new File(outputDirectory);
            if (!outputDir.exists()) {
                outputDir.mkdir();
            }

            //Create File Writer
            fileWriter = new FileWriter(outputDirectory + System.getProperty("file.separator") + "Output.txt");
            bufferedWriter = new BufferedWriter(fileWriter);


            //5.17 Task 17 Drop tables
            addDivider(bufferedWriter);
            addInputTitle("Drop tables", bufferedWriter);
            numberOfTablesDropped = 0;
            try {
                numberOfTablesDropped = carPoolingSystem.dropTables();
                // Check if tables are dropped
                printLine("Dropped " + numberOfTablesDropped + " tables.", bufferedWriter);
            } catch (Exception e) {
                e.printStackTrace();
                printLine("Exception occurred during dropTables(): "  + e.toString(), bufferedWriter);
            }
            addDivider(bufferedWriter);

            
            //5.1 Task 1 Create tables
            addDivider(bufferedWriter);
            addInputTitle("Create tables", bufferedWriter);
            numberOfTablesCreated = 0;
            try {
                numberOfTablesCreated = carPoolingSystem.createTables();
                // Check if tables are created
                printLine("Created " + numberOfTablesCreated + " tables.", bufferedWriter);
            } catch (Exception e) {
                e.printStackTrace();
                printLine("Exception occurred during createTables(): " + e.toString(), bufferedWriter);
            }
            addDivider(bufferedWriter);

            
            //5.2 Task 2 Insert Participants
            addDivider(bufferedWriter);
            addInputTitle("Insert Participants", bufferedWriter);
            numberOfInsertions = 0;
            Participant[] participants = FileOperations.readParticipantsFile();
            numberOfInsertions = carPoolingSystem.insertParticipants(participants);
            printLine(numberOfInsertions + " participants were inserted.", bufferedWriter);
            addDivider(bufferedWriter);

            //5.2 Task 2 Insert Passengers
            addDivider(bufferedWriter);
            addInputTitle("Insert Passengers", bufferedWriter);
            numberOfInsertions = 0;
            Passenger[] passengers = FileOperations.readPassengersFile();
            numberOfInsertions = carPoolingSystem.insertPassengers(passengers);
            printLine(numberOfInsertions + " passengers were inserted.", bufferedWriter);
            addDivider(bufferedWriter);

            //5.2 Task 2 Insert Drivers
            addDivider(bufferedWriter);
            addInputTitle("Insert Drivers", bufferedWriter);
            numberOfInsertions = 0;
            Driver[] drivers = FileOperations.readDriversFile();
            numberOfInsertions = carPoolingSystem.insertDrivers(drivers);
            printLine(numberOfInsertions + " drivers were inserted.", bufferedWriter);
            addDivider(bufferedWriter);

            //5.2 Task 2 Insert Cars
            addDivider(bufferedWriter);
            addInputTitle("Insert Cars", bufferedWriter);
            numberOfInsertions = 0;
            Car[] cars = FileOperations.readCarsFile();
            numberOfInsertions = carPoolingSystem.insertCars(cars);
            printLine(numberOfInsertions + " cars were inserted.", bufferedWriter);
            addDivider(bufferedWriter);

            //5.2 Task 2 Insert Trips
            addDivider(bufferedWriter);
            addInputTitle("Insert Trips", bufferedWriter);
            numberOfInsertions = 0;
            Trip[] trips = FileOperations.readTripsFile();
            numberOfInsertions = carPoolingSystem.insertTrips(trips);
            printLine(numberOfInsertions + " trips were inserted.", bufferedWriter);
            addDivider(bufferedWriter);

            //5.2 Task 2 Insert Bookings
            addDivider(bufferedWriter);
            addInputTitle("Insert Bookings", bufferedWriter);
            numberOfInsertions = 0;
            Booking[] bookings = FileOperations.readBookingsFile();
            numberOfInsertions = carPoolingSystem.insertBookings(bookings);
            printLine(numberOfInsertions + " bookings were inserted.", bufferedWriter);
            addDivider(bufferedWriter);
            

            /***********************************************************/
            /******************** Testing Methods **********************/
            /***********************************************************/
            
            //5.3 Task 3 Find all participants who are recorded as both drivers and passengers
            addDivider(bufferedWriter);
            addInputTitle("Find participants who are both passengers and drivers", bufferedWriter);
            try {
                Participant[] bothPassengersAndDrivers = carPoolingSystem.getBothPassengersAndDrivers();
                if (bothPassengersAndDrivers != null && bothPassengersAndDrivers.length > 0) {
                    for (Participant participant : bothPassengersAndDrivers) {
                        printLine(participant.toString(), bufferedWriter);
                    }
                } else {
                    printLine("No participants found who are both passengers and drivers.", bufferedWriter);
                }
            } catch (Exception e) {
                e.printStackTrace();
                printLine("Exception occurred during getBothPassengersAndDrivers(): " + e.toString(), bufferedWriter);
            }
            addDivider(bufferedWriter);

            

            //5.4 Task 4 Find the PINs, names, ages, and ratings of drivers who do not own any cars
            addDivider(bufferedWriter);
            addInputTitle("Find PINs, names, ages, and ratings of drivers who do not own any cars", bufferedWriter);
            try {
            	QueryResult.DriverPINNameAgeRating[] driverNames = carPoolingSystem.getDriversWithNoCars();
                if (driverNames != null && driverNames.length > 0) {
                    for (QueryResult.DriverPINNameAgeRating driver : driverNames) {
                    	printLine(driver.toString(), bufferedWriter);   
                    }
                } else {
                    printLine("No drivers without cars found.", bufferedWriter);
                }
            } catch (Exception e) {
                e.printStackTrace();
                printLine("Exception occurred during getDriversWithNoCars(): " + e.toString(), bufferedWriter);
            }
            addDivider(bufferedWriter);
            
            
            
            //5.5 Task 5 Delete Drivers who do not own any cars
            addDivider(bufferedWriter);
            addInputTitle("Delete Drivers with No Cars", bufferedWriter);
            int rowsDeleted = 0;
            rowsDeleted = carPoolingSystem.deleteDriversWithNoCars();
            printLine(rowsDeleted + " drivers were deleted from Drivers.", bufferedWriter);
            addDivider(bufferedWriter);
            
            
            
            //Testing 5.5: Find the PINs, names, ages, and ratings of drivers who do not own any cars
            addDivider(bufferedWriter);
            addInputTitle("Testing 5.5: Find the PINs, names, ages, and ratings of drivers who do not own any cars", bufferedWriter);
            try {
            	QueryResult.DriverPINNameAgeRating[] driverNames = carPoolingSystem.getDriversWithNoCars();
                if (driverNames != null && driverNames.length > 0) {
                    for (QueryResult.DriverPINNameAgeRating driver : driverNames) {
                    	printLine(driver.toString(), bufferedWriter);
                    }
                } else {
                    printLine("No drivers without cars found.", bufferedWriter);
                }
            } catch (Exception e) {
                e.printStackTrace();
                printLine("Exception occurred during getDriversWithNoCars(): " + e.toString(), bufferedWriter);
            }
            addDivider(bufferedWriter);
            
            
            
            //5.6 Task 6 Find all cars that are not taken part in any trips
            addDivider(bufferedWriter);
            addInputTitle("Find all cars that are not taken part in any trips", bufferedWriter);
            try {
                Car[] carsWithNoTrips = carPoolingSystem.getCarsWithNoTrips();
                if (carsWithNoTrips != null && carsWithNoTrips.length > 0) {
                    for (Car car : carsWithNoTrips) {
                        printLine(car.toString(), bufferedWriter);
                    }
                } else {
                    printLine("No cars without trips found.", bufferedWriter);
                }
            } catch (Exception e) {
                e.printStackTrace();
                printLine("Exception occurred during getCarsWithNoTrips(): " + e.toString(), bufferedWriter);
            }
            addDivider(bufferedWriter);

            
            
            //5.7 Task 7 Find all passengers who didn't book any trips
            addDivider(bufferedWriter);
            addInputTitle("Find all passengers who didn’t book any trips", bufferedWriter);
            try {
                Passenger[] passengersWithNoBooks = carPoolingSystem.getPassengersWithNoBooks();
                if (passengersWithNoBooks != null && passengersWithNoBooks.length > 0) {
                    for (Passenger passenger : passengersWithNoBooks) {
                        printLine(passenger.toString(), bufferedWriter);
                    }
                } else {
                    printLine("No passengers without bookings found.", bufferedWriter);
                }
            } catch (Exception e) {
                e.printStackTrace();
                printLine("Exception occurred during getPassengersWithNoBooks(): " + e.toString(), bufferedWriter);
            }
            addDivider(bufferedWriter);
            
            
            
            //5.8 Task 8 Find all trips that depart from the specified city to specified destination city on specific date
            addDivider(bufferedWriter);
            addInputTitle("Find trips departing from: " + city + " to: " + city_Chicago + " on: " + date_NY_C, bufferedWriter);
            try {
                Trip[] tripsFromToCitiesOnSpecDate = carPoolingSystem.getTripsFromToCitiesOnSpecificDate(city, city_Chicago, date_NY_C);
                if (tripsFromToCitiesOnSpecDate != null && tripsFromToCitiesOnSpecDate.length > 0) {
                    for (Trip trip : tripsFromToCitiesOnSpecDate) {
                    	printLine(trip.toString(), bufferedWriter);
                    }
                } else {
                    printLine("No trips found departing from " + city + " to " + city_Chicago + " on " + date_NY_C, bufferedWriter);
                }
            } catch (Exception e) {
                e.printStackTrace();
                printLine("Exception occurred during getTripsFromToCitiesOnSpecificDate(): " + e.toString(), bufferedWriter);
            }
            addDivider(bufferedWriter);

            
            
            //5.9 Task 9 Find the PINs, names, ages, and membership_status of passengers who have bookings on all trips destined at a particular city
            addDivider(bufferedWriter);
            addInputTitle("Find passengers booked on all trips destined at " + city_Seattle, bufferedWriter);
            try {
                QueryResult.PassengerPINNameAgeMembershipStatus[] passengerNames = carPoolingSystem.getPassengersWithBookingsToAllTripsForCity(city_Seattle);
                if (passengerNames != null && passengerNames.length > 0) {
                    for (QueryResult.PassengerPINNameAgeMembershipStatus passenger : passengerNames) {
                    	printLine(passenger.toString(), bufferedWriter);
                    }
                } else {
                    printLine("No passengers found booked on all trips destined at " + city_Seattle, bufferedWriter);
                }
            } catch (Exception e) {
                e.printStackTrace();
                printLine("Exception occurred during getPassengersBookedAllDestined(): " + e.toString(), bufferedWriter);
            }
            addDivider(bufferedWriter);

            
            
            //5.10 Task 10 For a given driver PIN, find the CarIDs that the driver owns and were booked at most twice.
            addDivider(bufferedWriter);
            addInputTitle("Find CarIDs owned by driver " + driverPIN + " with at most 2 bookings", bufferedWriter);
            try {
                Integer[] carIDs = carPoolingSystem.getDriverCarsWithAtMost2Bookings(driverPIN);
                if (carIDs != null && carIDs.length > 0) {
                    for (Integer carID : carIDs) {
                        printLine("CarID: " + carID, bufferedWriter);
                    }
                } else {
                    printLine("No cars found for driver PIN " + driverPIN + " with at most 2 bookings.", bufferedWriter);
                }
            } catch (Exception e) {
                e.printStackTrace();
                printLine("Exception occurred during getDriverCarsWithAtMost2Bookings(): " + e.toString(), bufferedWriter);
            }
            addDivider(bufferedWriter);

            
            
            //5.11 Task 11 Find the average age of passengers with "Confirmed" bookings (i.e., booking_status is ”Confirmed”) on trips departing from a given city and within a specified date range
            addDivider(bufferedWriter);
            addInputTitle("Find average age of passengers with confirmed bookings for trips departing from city " + city + " within the date range: " + startDate + " to " + endDate, bufferedWriter);
            try {
                Double averageAge = carPoolingSystem.getAvgAgeOfPassengersDepartFromCityBetweenTwoDates(city, startDate, endDate);
                if (averageAge != null) {
                    String formattedAverageAge = String.format(Locale.US, "%.3f", averageAge);

                    printLine("Average Age: " + formattedAverageAge, bufferedWriter);
                } else {
                    printLine("No data found for city " + city + " within the date range: " + startDate + " to " + endDate, bufferedWriter);
                }
            } catch (Exception e) {
                e.printStackTrace();
                printLine("Exception occurred during getAvgAgeOfPassengersDepartFromCityBetweenTwoDates(): " + e.toString(), bufferedWriter);
            }
            addDivider(bufferedWriter);

            
            
            //5.12 Task 12 Find Passengers in a Given Trip.
            addDivider(bufferedWriter);
            addInputTitle("Find Passengers in a Given Trip " +GivenTripID, bufferedWriter);
            try {
                QueryResult.PassengerPINNameAgeMembershipStatus[] passengersInTrip = carPoolingSystem.getPassengerInGivenTrip(GivenTripID);
                if (passengersInTrip != null && passengersInTrip.length > 0) {
                    for (QueryResult.PassengerPINNameAgeMembershipStatus passenger : passengersInTrip) {
                        printLine(passenger.toString(), bufferedWriter);
                    }
                } else {
                    printLine("No passengers found for TripID " + GivenTripID + ".", bufferedWriter);
                }
            } catch (Exception e) {
                e.printStackTrace();
                printLine("Exception occurred during getPassengerInGivenTrip(): " + e.toString(), bufferedWriter);
            }
            addDivider(bufferedWriter);

            
            
            //5.13 Task 13 Find Drivers’ Scores
            addDivider(bufferedWriter);
            addInputTitle("Find Drivers' Scores", bufferedWriter);
            try {
                QueryResult.DriverScoreRatingNumberOfBookingsPIN[] driverScores = carPoolingSystem.getDriversScores();
                if (driverScores != null && driverScores.length > 0) {
                    for (QueryResult.DriverScoreRatingNumberOfBookingsPIN driverScore : driverScores) {
                        printLine(driverScore.toString(), bufferedWriter);
                    }
                } else {
                    printLine("No driver scores found.", bufferedWriter);
                }
            } catch (Exception e) {
                e.printStackTrace();
                printLine("Exception occurred during getDriversScores(): " + e.toString(), bufferedWriter);
            }
            addDivider(bufferedWriter);

            
            
            //5.14 Task 14 Find average ratings of drivers who have trips destined to each city
            addDivider(bufferedWriter);
            addInputTitle("Find Average Ratings of Drivers Who Have Trips Destined to Each City", bufferedWriter);
            try {
                QueryResult.CityAndAverageDriverRating[] averageRatings = carPoolingSystem.getDriversAverageRatingsToEachDestinatedCity();
                if (averageRatings != null && averageRatings.length > 0) {
                    for (QueryResult.CityAndAverageDriverRating cityRating : averageRatings) {
                        printLine(cityRating.toString(), bufferedWriter);
                    }
                } else {
                    printLine("No average ratings found.", bufferedWriter);
                }
            } catch (Exception e) {
                e.printStackTrace();
                printLine("Exception occurred during getDriversAverageRatingsToEachDestinatedCity(): " + e.toString(), bufferedWriter);
            }
            addDivider(bufferedWriter);

            
            
            //5.15 Task 15 Find total number of bookings of passengers for each membership status
            addDivider(bufferedWriter);
            addInputTitle("Find Total Number of Bookings of Passengers for Each Membership Status", bufferedWriter);
            try {
                QueryResult.MembershipStatusAndTotalBookings[] totalBookings = carPoolingSystem.getTotalBookingsEachMembershipStatus();
                if (totalBookings != null && totalBookings.length > 0) {
                    for (QueryResult.MembershipStatusAndTotalBookings bookingDetail : totalBookings) {
                        printLine(bookingDetail.toString(), bufferedWriter);
                    }
                } else {
                    printLine("No total bookings found for any membership status.", bufferedWriter);
                }
            } catch (Exception e) {
                e.printStackTrace();
                printLine("Exception occurred during getTotalBookingsEachMembershipStatus(): " + e.toString(), bufferedWriter);
            }
            addDivider(bufferedWriter);
            
            
            
            //5.16 Task 16 For the drivers' ratings, if rating is smaller than 2.0 or equal to 2.0, update the rating by adding 0.5.
            addDivider(bufferedWriter);
            addInputTitle("Update drivers' ratings where the rating is smaller than or equal to 2.0", bufferedWriter);
            try {
            	numberOfRowsUpdated = carPoolingSystem.updateDriverRatings();
                printLine("Number of rows updated: " + numberOfRowsUpdated+".", bufferedWriter);
            } catch (Exception e) {
                e.printStackTrace();
                printLine("Exception occurred during updateDriverRatings(): " + e.toString(), bufferedWriter);
            }
            addDivider(bufferedWriter);

            
            
            //Testing 5.16: All Drivers after Updating the Ratings
            addDivider(bufferedWriter);
            addInputTitle("Testing 5.16: All Drivers after Updating the Ratings", bufferedWriter);
            try {
                Driver[] driversAfterUpdate = carPoolingSystem.getAllDrivers();
                if (driversAfterUpdate != null && driversAfterUpdate.length > 0) {
                    for (Driver driver : driversAfterUpdate) {
                        printLine(driver.toString(), bufferedWriter);
                    }
                } else {
                    printLine("No drivers found after updating the ratings.", bufferedWriter);
                }
            } catch (Exception e) {
                e.printStackTrace();
                printLine("Exception occurred during getAllDrivers(): " + e.toString(), bufferedWriter);
            }
            addDivider(bufferedWriter);
            
            
            
            //6.1 (Optional) Task 18 Find trips departing from the given city
            addDivider(bufferedWriter);
            addInputTitle("Find trips departing from the city: " + city, bufferedWriter);
            try {
                Trip[] tripsFromCity = carPoolingSystem.getTripsFromCity(city);
                if (tripsFromCity != null && tripsFromCity.length > 0) {
                    for (Trip trip : tripsFromCity) {
                    	printLine(trip.toString(), bufferedWriter);
                    }
                } else {
                    printLine("No trips found departing from " + city, bufferedWriter);
                }
            } catch (Exception e) {
                e.printStackTrace();
                printLine("Exception occurred during getTripsFromCity(): " + e.toString(), bufferedWriter);
            }
            addDivider(bufferedWriter);
            
            
            
            //6.2 (Optional) Task 19 Find all trips that have never been booked
            addDivider(bufferedWriter);
            addInputTitle("Find all trips that have never been booked", bufferedWriter);
            try {
                Trip[] tripsWithNoBooks = carPoolingSystem.getTripsWithNoBooks();
                if (tripsWithNoBooks != null && tripsWithNoBooks.length > 0) {
                    for (Trip trip : tripsWithNoBooks) {
                        printLine(trip.toString(), bufferedWriter);
                    }
                } else {
                    printLine("No trips without bookings found.", bufferedWriter);
                }
            } catch (Exception e) {
                e.printStackTrace();
                printLine("Exception occurred during getTripsWithNoBooks(): " + e.toString(), bufferedWriter);
            }
            addDivider(bufferedWriter);
            
            
            
            //6.3 (Optional) Task 20 For each driver, find the trip(s) with the highest number of bookings
            addDivider(bufferedWriter);
            addInputTitle("Find the most booked trips per driver", bufferedWriter);
            try {
                QueryResult.DriverPINandTripIDandNumberOfBookings[] mostBookedTrips = carPoolingSystem.getTheMostBookedTripsPerDriver();
                if (mostBookedTrips != null && mostBookedTrips.length > 0) {
                    for (QueryResult.DriverPINandTripIDandNumberOfBookings tripDetail : mostBookedTrips) {
                    	printLine(tripDetail.toString(), bufferedWriter);
                    }
                } else {
                    printLine("No trips found.", bufferedWriter);
                }
            } catch (Exception e) {
                e.printStackTrace();
                printLine("Exception occurred during getTheMostBookedTripsPerDriver(): " + e.toString(), bufferedWriter);
            }
            addDivider(bufferedWriter);
            
            
            
            //6.4 (Optional) Task 21 Find Full Cars
            addDivider(bufferedWriter);
            addInputTitle("Find Full Cars", bufferedWriter);
            try {
                QueryResult.FullCars[] fullCars = carPoolingSystem.getFullCars();
                if (fullCars != null && fullCars.length > 0) {
                    for (QueryResult.FullCars fullCar : fullCars) {
                        printLine(fullCar.toString(), bufferedWriter);
                    }
                } else {
                    printLine("No full cars found.", bufferedWriter);
                }
            } catch (Exception e) {
                e.printStackTrace();
                printLine("Exception occurred during getFullCars(): " + e.toString(), bufferedWriter);
            }
            addDivider(bufferedWriter);
            
            
            
            // Close the BufferedWriter
            if (bufferedWriter != null) {
                bufferedWriter.close();
            }
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                // Close the BufferedWriter if it's still open
                if (bufferedWriter != null) {
                    bufferedWriter.close();
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
            // Disconnect from the database
            disconnect();
        }
    }
}

