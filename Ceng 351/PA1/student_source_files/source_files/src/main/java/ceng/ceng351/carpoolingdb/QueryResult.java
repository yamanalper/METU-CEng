package ceng.ceng351.carpoolingdb;

import java.util.Locale;

public class QueryResult {

    public static class DriverPINNameAgeRating {
        private int PIN;
        private String p_name;
        private int age;
        private double rating;

        public DriverPINNameAgeRating(int PIN, String p_name, int age, double rating) {
            this.PIN = PIN;
            this.p_name = p_name;
            this.age=age;
            this.rating=rating;
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

		public double getRating() {
			return rating;
		}

		public void setRating(double rating) {
			this.rating = rating;
		}
		
		@Override
		public String toString() {
		    return "DriverPINNameAgeRating{" +
		            "PIN=" + PIN +
		            ", p_name='" + p_name + '\'' +
		            ", age=" + age +
		            ", rating=" + String.format(Locale.US, "%.1f", rating) +
		            '}';
		}

    }

    public static class PassengerPINNameAgeMembershipStatus {
        private int PIN;
        private String p_name;
        private int age;
        private String membership_status;

        public PassengerPINNameAgeMembershipStatus(int PIN, String p_name, int age, String membership_status) {
            this.PIN = PIN;
            this.p_name = p_name;
            this.age=age;
            this.membership_status=membership_status;
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

		public String getMembership_status() {
			return membership_status;
		}

		public void setMembership_status(String membership_status) {
			this.membership_status = membership_status;
		}
		
	    @Override
	    public String toString() {
	        return "PassengerPINNameAgeMembershipStatus{" +
	                "PIN=" + PIN +
	                ", p_name='" + p_name + '\'' +
	                ", age=" + age +
	                ", membership_status='" + membership_status + '\'' +
	                '}';
	    }
    }
    
    public static class DriverPINandTripIDandNumberOfBookings {
        private int TripID;
        private int numberOfBookings;
        private int DriverPIN;

        public DriverPINandTripIDandNumberOfBookings(int DriverPIN, int TripID, int numberOfBookings) {
        	this.DriverPIN=DriverPIN;
            this.TripID = TripID;
            this.numberOfBookings=numberOfBookings;
        }

        public int getTripID() {
            return TripID;
        }

        public void setTripID(int TripID) {
            this.TripID = TripID;
        }

		public int getNumberOfBookings() {
			return numberOfBookings;
		}

		public void setNumberOfBookings(int numberOfBookings) {
			this.numberOfBookings = numberOfBookings;
		}
		
		public int getDriverPIN() {
			return DriverPIN;
		}

		public void setDriverPIN(int DriverPIN) {
			this.DriverPIN = DriverPIN;
		}
		
	    @Override
	    public String toString() {
	        return "DriverPINandTripIDandNumberOfBookings{" +
	        		"DriverPIN=" + DriverPIN +
	                ", TripID=" + TripID +
	                ", numberOfBookings=" + numberOfBookings + 
	                '}';
	    }

    }
    
    
    public static class DriverScoreRatingNumberOfBookingsPIN {
        private double driver_score;
        private double rating;
        private int numberOfBookings;
        private int DriverPIN;

        public DriverScoreRatingNumberOfBookingsPIN(double driver_score, double rating, int numberOfBookings, int DriverPIN) {
            this.driver_score = driver_score;
            this.rating=rating;
            this.numberOfBookings=numberOfBookings;
        	this.DriverPIN=DriverPIN;
        }

        public double getDriverScore() {
            return driver_score;
        }

        public void setDriverScore(double driver_score) {
            this.driver_score = driver_score;
        }

		public double getDriverRating() {
			return rating;
		}

		public void setDriverRating(double rating) {
			this.rating = rating;
		}
		
		public int getNumberOfBookings() {
			return numberOfBookings;
		}

		public void setNumberOfBookings(int numberOfBookings) {
			this.numberOfBookings = numberOfBookings;
		}
		
		public int getDriverPIN() {
			return DriverPIN;
		}

		public void setDriverPIN(int DriverPIN) {
			this.DriverPIN = DriverPIN;
		}
		
		
		@Override
		public String toString() {
		    return "DriverScoreRatingNumberOfBookingsPIN{" +
		            "driver_score=" + String.format(Locale.US, "%.3f", driver_score) +
		            ", rating=" + String.format(Locale.US, "%.1f", rating) +
		            ", numberOfBookings=" + numberOfBookings+
		            ", DriverPIN=" + DriverPIN +
		            '}';
		}

    }
    
    
    public static class CityAndAverageDriverRating {
        private String destination_city;
        private double average_rating_of_drivers;

        public CityAndAverageDriverRating(String destination_city, double average_rating_of_drivers) {
            this.destination_city = destination_city;
            this.average_rating_of_drivers=average_rating_of_drivers;

        }

        public String getDestinationCity() {
            return destination_city;
        }

        public void setDestinationCity(String destination_city) {
            this.destination_city = destination_city;
        }

		public double getAverageRatingOfDrivers() {
			return average_rating_of_drivers;
		}

		public void setAverageRatingOfDrivers(double average_rating_of_drivers) {
			this.average_rating_of_drivers = average_rating_of_drivers;
		}
		
		
		
		@Override
		public String toString() {
		    return "CityAndAverageDriverRating{" +
		            "destination_city='" + destination_city + '\'' +
		            ", average_rating_of_drivers=" + String.format(Locale.US, "%.3f", average_rating_of_drivers) +
		            '}';
		}


    }
    
    
    public static class MembershipStatusAndTotalBookings {
        private String membership_status;
        private int total_number_of_bookings;

        public MembershipStatusAndTotalBookings(String membership_status, int total_number_of_bookings) {
            this.membership_status = membership_status;
            this.total_number_of_bookings=total_number_of_bookings;

        }

        public String getMemberhipStatus() {
            return membership_status;
        }

        public void setMemberhipStatus(String membership_status) {
            this.membership_status = membership_status;
        }

		public double getTotalNumberOfBookings() {
			return total_number_of_bookings;
		}

		public void setTotalNumberOfBookings(int total_number_of_bookings) {
			this.total_number_of_bookings = total_number_of_bookings;
		}
		
		

		@Override
		public String toString() {
		    return "MembershipStatusAndTotalBookings{" +
		            "membership_status='" + membership_status + '\'' +
		            ", total_number_of_bookings=" + total_number_of_bookings +
		            '}';
		}


    }

    
    public static class FullCars {
    	private int TripID;
        private int CarID;
        private String driver_name;
        private String color;
        private String brand;

        public FullCars(int TripID, int CarID, String driver_name, String color, String brand) {
            this.TripID=TripID;
        	this.CarID = CarID;
            this.driver_name=driver_name;
            this.color = color;
            this.brand=brand;
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

		public String getDriverName() {
			return driver_name;
		}

		public void setDriverName(String driver_name) {
			this.driver_name = driver_name;
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
		    return "FullCars{" +
		            "TripID=" + TripID +
		            ", CarID=" + CarID +
		            ", driver_name='" + driver_name + '\'' +
		            ", color='" + color + '\'' +
		            ", brand='" + brand + '\'' +
		            '}';
		}


    }
    
}
