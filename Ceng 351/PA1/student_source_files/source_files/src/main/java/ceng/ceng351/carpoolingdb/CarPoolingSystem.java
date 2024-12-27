package ceng.ceng351.carpoolingdb;

import javax.print.DocFlavor;
import java.lang.invoke.SwitchPoint;
import java.sql.*;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class CarPoolingSystem implements ICarPoolingSystem {

    private static String url = "jdbc:h2:mem:carpoolingdb;DB_CLOSE_DELAY=-1"; // In-memory database
    private static String user = "sa";          // H2 default username
    private static String password = "";        // H2 default password

    private Connection connection;

    public void initialize(Connection connection) {
        this.connection = connection;
    }

    //Given: getAllDrivers()
    //Testing 5.16: All Drivers after Updating the Ratings
    @Override
    public Driver[] getAllDrivers() {
        List<Driver> drivers = new ArrayList<>();
        
        //uncomment following code slice
        String query = "SELECT PIN, rating FROM Drivers ORDER BY PIN ASC;";

        try {
            PreparedStatement ps = this.connection.prepareStatement(query);
            ResultSet rs = ps.executeQuery();
            while (rs.next()) {
                int PIN = rs.getInt("PIN");
                double rating = rs.getDouble("rating");

                // Create a Driver object with only PIN and rating
                Driver driver = new Driver(PIN, rating);
                drivers.add(driver);
            }
            rs.close();
            ps.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        
        return drivers.toArray(new Driver[0]); 
    }

    
    //5.1 Task 1 Create tables
    @Override
    public int createTables() {
        int tableCount = 0;
        String createParticipantsTable = "CREATE TABLE IF NOT EXISTS Participants ("+
                "PIN INT PRIMARY KEY,"+
                "p_name VARCHAR(50),"+
                "age INT"+
                ");";
        String createDriversTable = "CREATE TABLE IF NOT EXISTS Drivers ("+
                "PIN INT PRIMARY KEY,"+
                "rating DOUBLE,"+
                "FOREIGN KEY (PIN) REFERENCES Participants (PIN)"+
                ");";
        String createPassengersTable = "CREATE TABLE IF NOT EXISTS Passengers ("+
                "PIN INT PRIMARY KEY,"+
                "membership_status  VARCHAR(50),"+
                "FOREIGN KEY (PIN) REFERENCES Participants (PIN)"+
                ");";
        String createCarsTable = "CREATE TABLE IF NOT EXISTS Cars ("+
                "CarID INT PRIMARY KEY,"+
                "PIN INT,"+
                "color VARCHAR(50),"+
                "brand VARCHAR(50),"+
                "FOREIGN KEY (PIN) REFERENCES Drivers (PIN)"+
                ");";
        String createTripsTable = "CREATE TABLE IF NOT EXISTS Trips ("+
                "TripID INT PRIMARY KEY,"+
                "CarID INT,"+
                "date VARCHAR(50),"+
                "departure VARCHAR(50),"+
                "destination VARCHAR(50),"+
                "num_seats_available INT,"+
                "FOREIGN KEY (CarID) REFERENCES Cars (CarID)"+
                ");";
        String createBookingsTable = "CREATE TABLE IF NOT EXISTS Bookings ("+
                "TripID INT,"+
                "PIN INT,"+
                "booking_status VARCHAR(50),"+
                "PRIMARY KEY (TripID,PIN),"+
                "FOREIGN KEY (TripID) REFERENCES Trips (TripID),"+
                "FOREIGN KEY (PIN) REFERENCES Passengers (PIN)"+
                ");";
        try (Statement statement = this.connection.createStatement()){
            try{
                statement.executeUpdate(createParticipantsTable);
                tableCount++;
            } catch (SQLException e) {
                System.err.println("Failed to create Participants table: " + e.getMessage());
            }
            try{
                statement.executeUpdate(createPassengersTable);
                tableCount++;
            } catch (SQLException e){
                System.err.println("Failed to create Passengers table: " + e.getMessage());
            }
            try{
                statement.executeUpdate(createDriversTable);
                tableCount++;
            } catch (SQLException e){
                System.err.println("Failed to create Drivers table: " + e.getMessage());
            }
            try{
                statement.executeUpdate(createCarsTable);
                tableCount++;
            } catch (SQLException e){
                System.err.println("Failed to create Cars table: " + e.getMessage());
            }
            try{
                statement.executeUpdate(createTripsTable);
                tableCount++;
            } catch (SQLException e){
                System.err.println("Failed to create Trips table: " + e.getMessage());
            }
            try{
                statement.executeUpdate(createBookingsTable);
                tableCount++;
            } catch (SQLException e) {
                System.err.println("Failed to create Bookings table: " + e.getMessage());
            }
        } catch(SQLException e){
            System.err.println("Failed to create statement : "+e.getMessage());
        }

        return tableCount;
    }


    //5.17 Task 17 Drop tables
    @Override
    public int dropTables() {
        int tableCount = 0;
        
        String dropParticipantsTable = "DROP TABLE IF EXISTS Participants;";
        String dropDriversTable = "DROP TABLE IF EXISTS Drivers;";
        String dropPassengersTable = "DROP TABLE IF EXISTS Passengers;";
        String dropCarsTable = "DROP TABLE IF EXISTS Cars;";
        String dropTripsTable = "DROP TABLE IF EXISTS Trips;";
        String dropBookingsTable = "DROP TABLE IF EXISTS Bookings;";
        try(Statement statement = this.connection.createStatement()){
            try{
                statement.executeUpdate(dropParticipantsTable);
                tableCount++;
            } catch (SQLException e) {
                System.err.println("Failed to drop Participants table: " + e.getMessage());
            }
            try{
                statement.executeUpdate(dropBookingsTable);
                tableCount++;
            } catch (SQLException e) {
                System.err.println("Failed to drop Bookings table: " + e.getMessage());
            }
            try{
                statement.executeUpdate(dropTripsTable);
                tableCount++;
            } catch (SQLException e) {
                System.err.println("Failed to drop Trips table: " + e.getMessage());
            }
            try{
                statement.executeUpdate(dropCarsTable);
                tableCount++;
            } catch (SQLException e) {
                System.err.println("Failed to drop Cars table: " + e.getMessage());
            }
            try{
                statement.executeUpdate(dropPassengersTable);
                tableCount++;
            } catch (SQLException e) {
                System.err.println("Failed to drop Passengers table: " + e.getMessage());
            }
            try{
                statement.executeUpdate(dropDriversTable);
                tableCount++;
            } catch (SQLException e) {
                System.err.println("Failed to drop Drivers table: " + e.getMessage());
            }
        } catch (SQLException e) {
            System.err.println("Failed to drop statement : "+e.getMessage());
        }

        return tableCount;
    }
    
    
    //5.2 Task 2 Insert Participants
    @Override
    public int insertParticipants(Participant[] participants) {
        int rowsInserted = 0;

        for(int i = 0 ; i < participants.length ; i++){
            String addParticipant = "INSERT INTO Participants(PIN,p_name,age) VALUES (?,?,?)";
            try(PreparedStatement statement = this.connection.prepareStatement(addParticipant)){
                try {
                    statement.setInt(1, participants[i].getPIN());
                    statement.setString(2, participants[i].getP_name());
                    statement.setInt(3, participants[i].getAge());
                    statement.executeUpdate();
                    rowsInserted++;
                } catch (SQLException e) {
                    System.err.println("Failed to insert Participant: " + e.getMessage());
                }
            } catch (SQLException e) {
                System.err.println("Failed to insert Participant: " + e.getMessage());
            }
        }
        
        return rowsInserted;
    }

    
    //5.2 Task 2 Insert Passengers
    @Override
    public int insertPassengers(Passenger[] passengers) {
        int rowsInserted = 0;

        for(int i = 0 ; i < passengers.length ; i++){
            String addPassenger = "INSERT INTO Passengers(PIN,membership_status) VALUES (?,?)";
            try(PreparedStatement statement = this.connection.prepareStatement(addPassenger)){
                try {
                    statement.setInt(1, passengers[i].getPIN());
                    statement.setString(2, passengers[i].getMembership_status());
                    statement.executeUpdate();
                    rowsInserted++;
                } catch (SQLException e) {
                    System.err.println("Failed to insert Passenger: " + e.getMessage());
                }
            } catch (SQLException e) {
                System.err.println("Failed to insert Passenger: " + e.getMessage());
            }
        }
        
        return rowsInserted;
    }


    //5.2 Task 2 Insert Drivers
    @Override
    public int insertDrivers(Driver[] drivers) {
        int rowsInserted = 0;

        for(int i = 0 ; i < drivers.length ; i++){
            String addDriver = "INSERT INTO Drivers(PIN,rating) VALUES (?,?)";
            try (PreparedStatement statement = this.connection.prepareStatement(addDriver)){
                try {
                    statement.setInt(1, drivers[i].getPIN());
                    statement.setDouble(2,drivers[i].getRating());
                    statement.executeUpdate();
                    rowsInserted++;
                }catch (SQLException e) {
                    System.err.println("Failed to insert Driver: " + e.getMessage());
                }
            } catch (SQLException e) {
                System.err.println("Failed to insert Driver: " + e.getMessage());
            }
        }
        
        return rowsInserted;
    }

    
    //5.2 Task 2 Insert Cars
    @Override
    public int insertCars(Car[] cars) {
        int rowsInserted = 0;
        
        for(int i = 0 ; i < cars.length ; i++){
            String addCar = "INSERT INTO Cars(PIN,CarID,color,brand) VALUES (?,?,?,?)";
            try (PreparedStatement statement = this.connection.prepareStatement(addCar)){
                try{
                    statement.setInt(1,cars[i].getPIN());
                    statement.setInt(2,cars[i].getCarID());
                    statement.setString(3,cars[i].getColor());
                    statement.setString(4, cars[i].getBrand());
                    statement.executeUpdate();
                    rowsInserted++;
                } catch (SQLException e) {
                    System.err.println("Failed to insert Car: " + e.getMessage());
                }
            } catch (SQLException e){
                System.err.println("Failed to insert Car: " + e.getMessage());
            }
        }

        return rowsInserted;
    }


    //5.2 Task 2 Insert Trips
    @Override
    public int insertTrips(Trip[] trips) {
        int rowsInserted = 0;

        for(int i = 0 ; i < trips.length ; i++){
            String addTrip = "INSERT INTO Trips(TripID,CarID,date,departure,destination,num_seats_available) VALUES (?,?,?,?,?,?)";
            try(PreparedStatement statement = this.connection.prepareStatement(addTrip)){
                try {
                    statement.setInt(1, trips[i].getTripID());
                    statement.setInt(2, trips[i].getCarID());
                    statement.setString(3, trips[i].getDate());
                    statement.setString(4, trips[i].getDeparture());
                    statement.setString(5, trips[i].getDestination());
                    statement.setInt(6,trips[i].getNum_seats_available());
                    statement.executeUpdate();
                    rowsInserted++;
                } catch (SQLException e){
                    System.err.println("Failed to insert Trip: " + e.getMessage());
                }
            } catch (SQLException e){
                System.err.println("Failed to insert Trip: " + e.getMessage());
            }
        }
        
        return rowsInserted;
    }

    //5.2 Task 2 Insert Bookings
    @Override
    public int insertBookings(Booking[] bookings) {
        int rowsInserted = 0;
        
        for(int i = 0 ; i < bookings.length ; i++){
            String addBookings = "INSERT INTO Bookings(TripID,PIN,booking_status) VALUES (?,?,?)";
            try (PreparedStatement statement = this.connection.prepareStatement(addBookings)){
                try{
                    statement.setInt(1,bookings[i].getTripID());
                    statement.setInt(2,bookings[i].getPIN());
                    statement.setString(3, bookings[i].getBooking_status());
                    statement.executeUpdate();
                    rowsInserted++;
                } catch (SQLException e){
                    System.err.println("Failed to insert Bookings: " + e.getMessage());
                }
            } catch (SQLException e){
                System.err.println("Failed to insert Bookings: " + e.getMessage());
            }
        }

        return rowsInserted;
    }

    
    //5.3 Task 3 Find all participants who are recorded as both drivers and passengers
    @Override
    public Participant[] getBothPassengersAndDrivers() {
        List<Participant> participants = new ArrayList<>();

        String query = "SELECT p.PIN, p.p_name, p.age FROM Participants p, Drivers d, Passengers p2 WHERE p.PIN = d.PIN AND p.PIN = p2.PIN ORDER BY p.PIN ASC;";
        try(PreparedStatement ps = this.connection.prepareStatement(query)){
            ResultSet rs = ps.executeQuery();
            while(rs.next()){
                int PIN = rs.getInt("PIN");
                String pName = rs.getString("p_name");
                int age = rs.getInt("age");
                Participant par = new Participant(PIN,pName,age);
                participants.add(par);
            }


        } catch (SQLException e) {
            e.printStackTrace();
        }



    	return participants.toArray(new Participant[0]);
    }

 
    //5.4 Task 4 Find the PINs, names, ages, and ratings of drivers who do not own any cars
    @Override
    public QueryResult.DriverPINNameAgeRating[] getDriversWithNoCars() {
    	List<QueryResult.DriverPINNameAgeRating> list = new ArrayList<>();
        String query = "select p.PIN, p.p_name, p.age, d.rating "+
                "from Participants p, Drivers d "+
                "where d.PIN = p.PIN and d.PIN not in (select PIN from Cars) "+
                "order by p.PIN asc;";
        try(PreparedStatement ps = this.connection.prepareStatement(query)){
            ResultSet rs = ps.executeQuery();
            while(rs.next()){
                int pin = rs.getInt(1);
                String name = rs.getString(2);
                int age = rs.getInt(3);
                double rating = rs.getDouble(4);
                QueryResult.DriverPINNameAgeRating driver = new QueryResult.DriverPINNameAgeRating(pin,name,age,rating);
                list.add(driver);
            }

            rs.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

    	
    	return list.toArray(new QueryResult.DriverPINNameAgeRating[0]);
    }

    
    //5.5 Task 5 Delete Drivers who do not own any cars
    @Override
    public int deleteDriversWithNoCars() {
        int rowsDeleted = 0;
        String query1 = "select count(d.pin) "+
                "from Drivers d "+
                "where d.pin not in (select pin from cars); ";

        String query2 = "delete from Drivers d "+
                "where d.pin not in (select pin from cars); ";

        try (PreparedStatement ps1 = this.connection.prepareStatement(query1);
            PreparedStatement ps2 = this.connection.prepareStatement(query2);){

            try(ResultSet rs = ps1.executeQuery()) {
                if(rs.next()) {
                    rowsDeleted = rs.getInt(1);
                }
            }
            ps2.executeUpdate();

        } catch (SQLException e){
            e.printStackTrace();
        }
        return rowsDeleted;  
    }

    
    //5.6 Task 6 Find all cars that are not taken part in any trips
    @Override
    public Car[] getCarsWithNoTrips() {
        List<Car> cars = new ArrayList<>();
        String query = "select * "+
                "from cars c "+
                "where c.CarID not in (select t.CarID from trips t, cars c1 where t.CarID = c1.CarID) "+
                "order by CarID asc; ";
    	try (PreparedStatement ps = this.connection.prepareStatement(query)){
            ResultSet rs = ps.executeQuery();
            while(rs.next()){
                int carid = rs.getInt(1);
                int pin = rs.getInt(2);
                String color = rs.getString(3);
                String brand = rs.getString(4);
                Car car = new Car(carid,pin,color,brand);
                cars.add(car);
            }
        } catch (SQLException e){
            e.printStackTrace();
        }

        return cars.toArray(new Car[0]);
    }
    
    
    //5.7 Task 7 Find all passengers who didn't book any trips
    @Override
    public Passenger[] getPassengersWithNoBooks() {
        List<Passenger> noTrip = new ArrayList<>();
        String query = "Select * "+
                "from passengers p "+
                "where not exists ( "+
                    "select b.pin "+
                    "from bookings b "+
                    "where b.pin = p.pin) "+
                "order by p.pin asc; ";
        try(PreparedStatement ps = this.connection.prepareStatement(query)){
            ResultSet rs = ps.executeQuery();
            while(rs.next()){
                int pin = rs.getInt(1);
                String status = rs.getString(2);
                Passenger nobook = new Passenger(pin, status);
                noTrip.add(nobook);
            }

        } catch (SQLException e){
            e.printStackTrace();
        }

    	
        return noTrip.toArray(new Passenger[0]);
    }


    //5.8 Task 8 Find all trips that depart from the specified city to specified destination city on specific date
    @Override
    public Trip[] getTripsFromToCitiesOnSpecificDate(String departure, String destination, String date) {
        
    	List<Trip> spesific = new ArrayList<>();
        String query = "select * "+
                "from trips t "+
                "where t.date = ? and t.destination = ? and t.departure = ? "+
                "order by t.tripID asc; ";
    	try(PreparedStatement ps = this.connection.prepareStatement(query)) {
            ps.setString(1, date);
            ps.setString(2,destination);
            ps.setString(3,departure);
            ResultSet rs = ps.executeQuery();
            while(rs.next()){
                int id = rs.getInt(1);
                int carid = rs.getInt(2);
                String datee = rs.getString(3);
                String dep = rs.getString(4);
                String dest = rs.getString(5);
                int avail = rs.getInt(6);
                Trip t = new Trip(id,carid,datee,dep,dest,avail);
                spesific.add(t);
            }
        }catch(SQLException e){
            e.printStackTrace();
        }


        return spesific.toArray(new Trip[0]);
    }


    //5.9 Task 9 Find the PINs, names, ages, and membership_status of passengers who have bookings on all trips destined at a particular city
    @Override
    public QueryResult.PassengerPINNameAgeMembershipStatus[] getPassengersWithBookingsToAllTripsForCity(String city) {
    	List<QueryResult.PassengerPINNameAgeMembershipStatus> result = new ArrayList<>();
        String query = "select pas.pin, par.p_name, par.age , pas.membership_status "+
                "from Passengers pas, Participants par "+
                "where par.pin = pas.pin and not exists ( "+
                    "select * "+
                    "from trips t "+
                    "where t.destination = ? and not exists ( "+
                        "select * "+
                        "from bookings b "+
                        "where t.tripID = b.tripID and b.pin = pas.pin ))"+
                "order by pas.pin asc; ";
        try (PreparedStatement ps = this.connection.prepareStatement(query)){
            ps.setString(1,city);
            ResultSet rs = ps.executeQuery();
            while(rs.next()){
                int pin = rs.getInt(1);
                String name = rs.getString(2);
                int age = rs.getInt(3);
                String stat = rs.getString(4);
                QueryResult.PassengerPINNameAgeMembershipStatus item = new QueryResult.PassengerPINNameAgeMembershipStatus(pin, name, age, stat);
                result.add(item);
            }
        } catch (SQLException e){
            e.printStackTrace();
        }

    	
        return result.toArray(new QueryResult.PassengerPINNameAgeMembershipStatus[0] );
    }

    
    //5.10 Task 10 For a given driver PIN, find the CarIDs that the driver owns and were booked at most twice.    
    @Override
    public Integer[] getDriverCarsWithAtMost2Bookings(int driverPIN) {
        List<Integer> result = new ArrayList<>();
        String query = " Select c.carID "+
                "from cars c, bookings b, trips t "+
                "where c.pin = ? and c.carID = t.carID and t.tripID = b.tripID "+
                "group by c.carID "+
                "having count(b.tripID) <= 2 "+
                "order by c.carID asc; ";
        try (PreparedStatement ps = this.connection.prepareStatement(query)){
            ps.setInt(1, driverPIN);
            ResultSet rs = ps.executeQuery();
            while(rs.next()){
                int id = rs.getInt(1);
                result.add(id);
            }
        } catch(SQLException e){
            e.printStackTrace();
        }

        return result.toArray(new Integer[0]);  // Return the list as an array
    }


    //5.11 Task 11 Find the average age of passengers with "Confirmed" bookings (i.e., booking_status is ”Confirmed”) on trips departing from a given city and within a specified date range
    @Override
    public Double getAvgAgeOfPassengersDepartFromCityBetweenTwoDates(String city, String start_date, String end_date) {
        Double averageAge = null;

        String query = "select avg(p.age) "+
                "from participants p, bookings b, passengers pas "+
                "where p.pin = pas.pin and  pas.pin = b.pin and b.booking_status = 'Confirmed' and b.tripID in( "+
                    "select t.tripId "+
                    "from trips t "+
                    "where t.departure = ? and t.date between ? and ?); ";

        try(PreparedStatement ps = this.connection.prepareStatement(query)){
            ps.setString(1, city);
            ps.setString(2, start_date);
            ps.setString(3, end_date);
            ResultSet rs = ps.executeQuery();
            while(rs.next()){
                Double result = rs.getDouble(1);
                averageAge = result;
            }
        } catch (SQLException e){
            e.printStackTrace();
        }


        return averageAge;
    }


    //5.12 Task 12 Find Passengers in a Given Trip.
    @Override
    public QueryResult.PassengerPINNameAgeMembershipStatus[] getPassengerInGivenTrip(int TripID) {
        List<QueryResult.PassengerPINNameAgeMembershipStatus> result = new ArrayList<>();
        String query = "select par.pin, par.p_name, par.age, pas.membership_status "+
                "from passengers pas, participants par "+
                "where pas.pin = par.pin and pas.pin in( "+
                    "select b.pin "+
                    "from bookings b "+
                    "where b.tripID = ?) "+
                "order by par.pin asc; ";
        try(PreparedStatement ps = this.connection.prepareStatement(query)){
            ps.setInt(1, TripID);
            ResultSet rs = ps.executeQuery();
            while(rs.next()){
                int pin = rs.getInt(1);
                String name = rs.getString(2);
                int age = rs.getInt(3);
                String stat = rs.getString(4);
                QueryResult.PassengerPINNameAgeMembershipStatus aa = new QueryResult.PassengerPINNameAgeMembershipStatus(pin, name ,age ,stat);
                result.add(aa);
            }
        } catch (SQLException e){
            e.printStackTrace();
        }
    	
        return result.toArray(new QueryResult.PassengerPINNameAgeMembershipStatus[0]);
    }


    //5.13 Task 13 Find Drivers’ Scores
    @Override
    public QueryResult.DriverScoreRatingNumberOfBookingsPIN[] getDriversScores() {
        List<QueryResult.DriverScoreRatingNumberOfBookingsPIN> result = new ArrayList<>();
        String query = "select (d.rating * count(b.tripID) ) as driver_score , d.rating , count(b.tripID) as numberOfBookings, d.pin "+
                "from Drivers d, Cars c, Trips t, Bookings b "+
                "where d.pin = c.pin and c.carID = t.carID and t.tripID = b.tripID "+
                "group by d.pin "+
                "order by driver_score desc, d.pin asc; ";

        try(PreparedStatement ps = this.connection.prepareStatement(query)){
            ResultSet rs = ps.executeQuery();
            while(rs.next()){
                double score = rs.getDouble(1);
                double rating = rs.getDouble(2);
                int no = rs.getInt(3);
                int pin = rs.getInt(4);
                QueryResult.DriverScoreRatingNumberOfBookingsPIN temp = new QueryResult.DriverScoreRatingNumberOfBookingsPIN(score, rating, no , pin);
                result.add(temp);
            }

        } catch (SQLException e){
            e.printStackTrace();
        }

    	
        return result.toArray(new QueryResult.DriverScoreRatingNumberOfBookingsPIN[0]);
    }

    
    //5.14 Task 14 Find average ratings of drivers who have trips destined to each city
    @Override
    public QueryResult.CityAndAverageDriverRating[] getDriversAverageRatingsToEachDestinatedCity() {
        List<QueryResult.CityAndAverageDriverRating> result = new ArrayList<>();

        String query = "select t.destination,  avg(d.rating ) as avarage_rating_of_drivers "+
                "from trips t, drivers d, cars c "+
                "where t.carID = c.carID and c.pin = d.pin "+
                "group by t.destination "+
                "order by t.destination asc; ";

        try(PreparedStatement ps = this.connection.prepareStatement(query)){
            ResultSet rs = ps.executeQuery();
            while(rs.next()) {
                String dest = rs.getString(1);
                double avgRate = rs.getDouble(2);
                QueryResult.CityAndAverageDriverRating temp = new QueryResult.CityAndAverageDriverRating(dest, avgRate);
                result.add(temp);
            }

        } catch (SQLException e){
            e.printStackTrace();
        }

        return result.toArray(new QueryResult.CityAndAverageDriverRating[0]);
    }


    //5.15 Task 15 Find total number of bookings of passengers for each membership status
    @Override
    public QueryResult.MembershipStatusAndTotalBookings[] getTotalBookingsEachMembershipStatus() {
        List<QueryResult.MembershipStatusAndTotalBookings> result = new ArrayList<>();
        String query = "select p.membership_status , count(b.tripID) as number_of_total_bookings "+
                "from passengers p, bookings b "+
                "where p.pin = b.pin "+
                "group by p.membership_status "+
                "order by p.membership_status asc; ";
        try(PreparedStatement ps = this.connection.prepareStatement(query)){
            ResultSet rs = ps.executeQuery();
            while(rs.next()){
                String stat = rs.getString(1);
                int count = rs.getInt(2);
                QueryResult.MembershipStatusAndTotalBookings temp = new QueryResult.MembershipStatusAndTotalBookings(stat, count);
                result.add(temp);
            }
        }catch (SQLException e ){
            e.printStackTrace();
        }

    	
        return result.toArray(new QueryResult.MembershipStatusAndTotalBookings[0]);
    }

    
    //5.16 Task 16 For the drivers' ratings, if rating is smaller than 2.0 or equal to 2.0, update the rating by adding 0.5.
    @Override
    public int updateDriverRatings() {
        int rowsUpdated = 0;
        String query = "update drivers d "+
                "set d.rating = d.rating + 0.5 "+
                "where d.rating <= 2; ";
        try (PreparedStatement ps = this.connection.prepareStatement(query)){
            rowsUpdated = ps.executeUpdate();

        } catch (SQLException e ){
            e.printStackTrace();
        }
    	
        return rowsUpdated;
    }
    

    //6.1 (Optional) Task 18 Find trips departing from the given city
    @Override
    public Trip[] getTripsFromCity(String city) {
        
    	/*****************************************************/
        /*****************************************************/
        /*****************  TODO  (Optional)   ***************/
        /*****************************************************/
        /*****************************************************/
    	
        return new Trip[0];
    }
    
    
    //6.2 (Optional) Task 19 Find all trips that have never been booked
    @Override
    public Trip[] getTripsWithNoBooks() {
        
    	/*****************************************************/
        /*****************************************************/
        /*****************  TODO  (Optional)   ***************/
        /*****************************************************/
        /*****************************************************/
    	
        return new Trip[0];
    }
    
    
    //6.3 (Optional) Task 20 For each driver, find the trip(s) with the highest number of bookings
    @Override
    public QueryResult.DriverPINandTripIDandNumberOfBookings[] getTheMostBookedTripsPerDriver() {
        
    	/*****************************************************/
        /*****************************************************/
        /*****************  TODO  (Optional)   ***************/
        /*****************************************************/
        /*****************************************************/
    	
        return new QueryResult.DriverPINandTripIDandNumberOfBookings[0];
    }
    
    
    //6.4 (Optional) Task 21 Find Full Cars
    @Override
    public QueryResult.FullCars[] getFullCars() {
        
    	/*****************************************************/
        /*****************************************************/
        /*****************  TODO  (Optional)   ***************/
        /*****************************************************/
        /*****************************************************/
    	
        return new QueryResult.FullCars[0];
    }

}
