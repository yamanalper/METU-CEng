package ceng.ceng351.carpoolingdb;

import java.sql.Connection;

public interface ICarPoolingSystem {

    /**
     * Place your initialization code inside if required.
     *
     * <p>
     * This function will be called before all other operations. If your implementation
     * needs initialization, necessary operations should be done inside this function. For
     * example, you can set up your connection to the database server inside this function.
     */
    public void initialize(Connection connection);

    /**
     * Should get all drivers
     *
     * @return Driver[]
     */
    public Driver[] getAllDrivers();
    
    /**
     * Should create the necessary tables when called.
     *
     * @return the number of tables that are created successfully.
     */
    public int createTables();

    /**
     * Should drop the database tables if they exist.
     *
     * @return The number of tables that are dropped successfully.
     */
    public int dropTables();
    
    /**
     * Should insert an array of Participant into the database.
     *
     * @return Number of rows inserted successfully.
     */
    public int insertParticipants(Participant[] participants);

    /**
     * Should insert an array of Passengers into the database.
     *
     * @return Number of rows inserted successfully.
     */
    public int insertPassengers(Passenger[] passengers);

    /**
     * Should insert an array of Drivers into the database.
     *
     * @return Number of rows inserted successfully.
     */
    public int insertDrivers(Driver[] drivers);

    /**
     * Should insert an array of Cars into the database.
     *
     * @return Number of rows inserted successfully.
     */
    public int insertCars(Car[] cars);

    /**
     * Should insert an array of Trips into the database.
     *
     * @return Number of rows inserted successfully.
     */
    public int insertTrips(Trip[] trips);

    /**
     * Should insert an array of Bookings into the database.
     *
     * @return Number of rows inserted successfully.
     */
    public int insertBookings(Booking[] bookings);

    /**
     * The Car Pooling system allows participants to be recorded as both passengers and drivers.
     * Should return all participants who are recorded as both drivers and passengers.
     * 
     * @return Participant[]
     */
    public Participant[] getBothPassengersAndDrivers();
    
    /**
     * Should return the PINs, names, ages, and ratings of drivers who do not own any cars.
     *
     * @return QueryResult.DriverPINNameAgeRating[]
     */
    public QueryResult.DriverPINNameAgeRating[] getDriversWithNoCars();
    
    /**
     * Should delete drivers who do not own any cars in the system from Drivers table.
     * Note that you should not delete these drivers from Participants table.
     *
     * @return Number of rows deleted successfully.
     */
    public int deleteDriversWithNoCars();
    
    /**
     * Should return all cars that are not taken part in any trips.
     *
     * @return Car[]
     */
    public Car[] getCarsWithNoTrips();

    /**
     * Should return all passengers who didn't book any trips
     *
     * @return Passenger[]
     */
    public Passenger[] getPassengersWithNoBooks();
    
    /**
     * Should return all trips that depart from the specified city to specified destination city on specific date.
     *
     * @param departure The departure city.
     * @param destination The destination city.
     * @param date The specific date.
     * @return Trip[]
     */
    public Trip[] getTripsFromToCitiesOnSpecificDate(String departure, String destination, String date);
    
    /**
     * Should return the PINs, names, ages, and membership_status of passengers who have bookings on all trips destined at a particular city.
     *
     * @param city The destination city.
     * @return QueryResult.PassengerPINNameAgeMembershipStatus[]
     */
    public QueryResult.PassengerPINNameAgeMembershipStatus[] getPassengersWithBookingsToAllTripsForCity(String city);

    /**
     * For a given driver PIN, find the CarIDs that the driver owns and were booked at most twice.
     *
     * @param driverPIN The PIN of the driver.
     * @return Integer[] Array of CarIDs.
     */
    public Integer[] getDriverCarsWithAtMost2Bookings(int driverPIN);

    /**
     * For a given date range and departure city, find the average age of passengers in confirmed trips.
     *
     * @param city     The departure city.
     * @param start_date The start date of the specified date range for trips.
     * @param end_date   The end date of the specified date range for trips.
     * @return The average age of passengers.
     */
    public Double getAvgAgeOfPassengersDepartFromCityBetweenTwoDates(String city, String start_date, String end_date );

    /**
     * Find Passengers in a Given Trip.
     *
     * @param TripID The ID of the trip
     * @return QueryResult.PassengerPINNameAgeMembershipStatus[]
     */
    public QueryResult.PassengerPINNameAgeMembershipStatus[] getPassengerInGivenTrip(int TripID);

    /**
     * Find Drivers' Scores
     *
     * @return QueryResult.DriverScoreRatingNumberOfBookingsPIN[]
     */
    public QueryResult.DriverScoreRatingNumberOfBookingsPIN[] getDriversScores();

    /**
     * Find the average rating of drivers associated with trips to each destination city
     *
     * @return QueryResult.CityAndAverageDriverRating[]
     */
    public  QueryResult.CityAndAverageDriverRating[] getDriversAverageRatingsToEachDestinatedCity();
    
    /**
     * Find Total Number of Bookings of Passengers for Each Membership Status
     *
     * @return QueryResult.MembershipStatusAndTotalBookings[] 
     */
    public QueryResult.MembershipStatusAndTotalBookings[] getTotalBookingsEachMembershipStatus ();
     
    /**
     * For the drivers' ratings, if rating is smaller than 2.0 or equal to 2.0, update the rating by adding 0.5. 
     *
     * @return The number of rows updated.
     */
    public int updateDriverRatings();
    
    /**
     * Should return all trips that depart from the given city.
     *
     * @param city The departure city.
     * @return Trip[]
     */
    public Trip[] getTripsFromCity(String city);
    
    /**
     * Should return all trips that have never been booked.
     *
     * @return Trip[]
     */
    public Trip[] getTripsWithNoBooks();
    
    /**
     * For each driver, find the trip(s) with the highest number of bookings.
     *
     * @return QueryResult.DriverPINandTripIDandNumberOfBookings[]
     */
    public QueryResult.DriverPINandTripIDandNumberOfBookings[] getTheMostBookedTripsPerDriver();
    
    /**
     * Find Full Cars
     *
     * @return QueryResult.FullCars[]
     */
    public QueryResult.FullCars[] getFullCars ();
    
    
}
