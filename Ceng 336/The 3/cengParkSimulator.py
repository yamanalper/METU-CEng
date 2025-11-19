import os
os.environ["SDL_AUDIODRIVER"] = "dummy"
import pygame
import random
import string
import time
import threading
import serial
import sys
import queue

DEBUG = False
BOARD_SIMULATION = False

def debug_print(message):
    if DEBUG == True:
        print("DEBUG: " + message)

# Screen dimensions
SCREEN_WIDTH = 1500
SCREEN_HEIGHT = 600
DISPLAY_WIDTH = 200
SIMULATOR_CAPTION = 'Ceng Parking Lot Simulator'

# Parking lot configuration
FLOORS = 4
CARS_PER_FLOOR = 10

# Serial communication configuration
SERIAL_PORT = '/dev/ttyUSB0'
BAUDRATE = 115200
PARITY = serial.PARITY_NONE
RTSCTS = False
XONXOFF = False

car_colors = [
    (0, 0, 0),        # Black
    (210, 192, 210),  # Cream
    (128, 128, 128),  # Gray
    (192, 192, 192),  # Silver
    (0, 0, 255),      # Blue
    (0, 128, 255),    # Light Blue
    (255, 0, 0),      # Red
    (139, 0, 0),      # Dark Red
    (0, 128, 0),      # Green
    (255, 165, 0),    # Orange
    (128, 0, 128),    # Purple
    (210, 180, 140),  # Tan
    (255, 215, 0),    # Gold
    (105, 105, 105),  # Dark Gray
    (70, 130, 180),   # Steel Blue
    (0, 100, 0),      # Dark Green
    (220, 20, 60),    # Crimson
    (112, 128, 144),  # Slate Gray
    (47, 79, 79),     # Dark Slate Gray
    (169, 169, 169)   # Light Gray
]

def checkMessage(message):
    if message.startswith(b'EMP') and len(message) == 5:
        return True
    elif message.startswith(b'SPC') and len(message) == 9:
        return True
    elif message.startswith(b'FEE') and len(message) == 9:
        return True
    elif message.startswith(b'RES') and len(message) == 8:
        return True
    else:
        return False

class Car:
    def __init__(self, car_id, car_color, subscribed):
        self.car_id = car_id
        self.car_color = car_color
        self.subscribed = subscribed

    def __eq__(self, other):
        if isinstance(other, Car):
            return self.car_id == other.car_id
        return False

    def __hash__(self):
        return hash(self.car_id)

class CarQueue:
    def __init__(self, no_cars):
        self.no_cars = no_cars
        self.queue = []
        self.lock = threading.Lock()  

    def add_car(self, car):
        with self.lock:  
            if len(self.queue) < self.no_cars:
                self.queue.append(car)
                return True
            else:
                debug_print("Queue is full. Cannot add car.")
                return False

    def remove_car(self, car):
        with self.lock:  
            if self.queue and car in self.queue:
                self.queue.remove(car)

    def get_queue(self):
        with self.lock:  
            return list(self.queue)  

    def get_queue_size(self):
        with self.lock:  
            return len(self.queue)

    def is_full(self):
        with self.lock:  
            return len(self.queue) >= self.no_cars

    def is_empty(self):
        with self.lock:  
            return len(self.queue) == 0

class ParkingLot:
    def __init__(self, floors, places_per_floor):
        self.floors = floors
        self.places_per_floor = places_per_floor
        self.spots = [[None for _ in range(places_per_floor)] for _ in range(floors)]
        self.lock = threading.Lock() 

    def park_car_raw(self, floor, spot, car):
        real_floor = ord(floor) - ord('A')
        real_spot = spot - 1
        return self.park_car(real_floor, real_spot, car)

    def park_car(self, floor, spot, car):
        with self.lock:  
            if self.spots[floor][spot] is None:
                entry_time = time.time()  
                self.spots[floor][spot] = (car, entry_time)
                return True
            else:
                print(f"Spot {spot} on floor {floor} is already occupied.")
                return False

    def remove_car(self, floor, spot):
        with self.lock:  
            if self.spots[floor][spot] is not None:
                car, entry_time = self.spots[floor][spot]
                self.spots[floor][spot] = None
                return car, entry_time
            else:
                print(f"Spot {spot} on floor {floor} is already empty.")
                return None

    def remove_car_by_id(self, car_id):
        with self.lock:  
            for floor in range(self.floors):
                for spot in range(self.places_per_floor):
                    if self.spots[floor][spot] is not None and self.spots[floor][spot][0].car_id == car_id:
                        car, entry_time = self.spots[floor][spot]
                        self.spots[floor][spot] = None
                        return car, entry_time
            debug_print(f"Car {car_id} not found in the parking lot.")
            return None

    def get_number_of_cars(self, floor):
        with self.lock:  
            return sum(1 for spot in self.spots[floor] if spot is not None)

    def get_total_cars(self):
        return sum(self.get_number_of_cars(floor) for floor in range(self.floors))

    def read_spot(self, floor, spot):
        with self.lock: 
            return self.spots[floor][spot]
        
    def read_spot_raw(self, floor, spot):
        real_floor = ord(floor) - ord('A')
        real_spot = spot - 1
        return self.read_spot(real_floor, real_spot)
        
    def get_1D_spots(self):
        with self.lock:  
            return [spot for floor in self.spots for spot in floor]
        
    def get_all_cars(self):
        with self.lock:  
            return [car for floor in self.spots for spot in floor if spot is not None for car, _ in [spot]]

class Subscriptions:
    def __init__(self):
        self.subscriptions = {}
        self.lock = threading.Lock()

    def add_subscription(self, car_id, floor, spot):
        with self.lock:
            if 10 * floor + spot in self.subscriptions:
                debug_print(f"Error: Subscription already exists for car {Subscriptions[10 * floor + spot]} at floor {floor}, spot {spot}.")
                return
            self.subscriptions[10 * floor + spot] = car_id

    def add_subscription_raw(self, car_id, floor, spot):
        real_floor = ord(floor) - ord('A')
        real_spot = spot - 1
        return self.add_subscription(car_id, real_floor, real_spot)

    def remove_subscription(self, floor, spot):
        with self.lock:
            if 10 * floor + spot in self.subscriptions:
                del self.subscriptions[10 * floor + spot]

    def get_subscription_raw(self, floor, spot):
        real_floor = ord(floor) - ord('A')
        real_spot = spot - 1
        return self.get_subscription(real_floor, real_spot)

    def get_subscription(self, floor, spot):
        with self.lock:
            if 10 * floor + spot in self.subscriptions:
                return self.subscriptions[10 * floor + spot]
            return None

class Drawer:
    def __init__(self, screen_width, screen_height, display_width, floors, cars_per_floor, simulator_caption, 
                 car_queue: CarQueue, parking_lot: ParkingLot, subscriptions: Subscriptions, serial_manager):

        self.car_queue = car_queue
        self.parking_lot = parking_lot
        self.subscriptions = subscriptions
        self.serial_manager = serial_manager

        self.screen_width = screen_width
        self.screen_height = screen_height
        self.display_width = display_width
        self.game_area_width = screen_width - display_width
        self.simulator_caption = simulator_caption

        self.floors = floors
        self.cars_per_floor = cars_per_floor
        self.places_per_column = cars_per_floor // 2
        self.queue_places_per_column = 2

        self.parking_height = self.places_per_column * screen_height // (self.places_per_column + self.queue_places_per_column + 1)
        self.queue_height = self.queue_places_per_column * screen_height // (self.places_per_column + self.queue_places_per_column + 1)
        self.empty_height = screen_height - self.parking_height - self.queue_height
        self.floor_width = self.game_area_width // floors
        self.car_width, self.car_height = self.floor_width // 3, self.parking_height // (self.places_per_column * 1.2)

        self.floor_font_size = 50
        self.display_font = pygame.font.SysFont(None, 24)
        self.floor_font = pygame.font.SysFont(None, self.floor_font_size)
        self.gray_color = (128, 128, 148)
        self.white_color = (255, 255, 255)
        self.red_color = (255, 0, 0)
        self.yellow_color = (204, 204, 0)
        self.black_color = (0, 0, 0)
        self.text_color = (255, 255, 255)
        self.green_color = (0, 255, 0)

        self.parking_spots = self.__init_parking_spots()
        self.queue_spots = self.__init_queue_spots()
        self.floor_character_spots = self.__init_floor_character_spots()
        
        # Game statistics
        self.game_status = 0
        self.calculated_fee = 0
        self.simulator_fee = 0
        self.received_empty_spaces = 0

        self.running = True
        self.drawer_thread = threading.Thread(target=self.__drawing_loop, daemon=True)
        self.drawer_thread.start()

    def stop(self):
        if self.running:
            self.running = False
            self.drawer_thread.join()

    def __init_parking_spots(self):
        parking_spots = []
        for floor in range(self.floors):
            for col in range(2):
                for spot in range(self.places_per_column):
                    x = floor * self.floor_width + col * (self.floor_width // 2) + self.floor_width // 4 - self.car_width // 2 + ((2 * (col % 2) - 1) * self.car_width) // 6.5
                    y = spot * (self.parking_height // self.places_per_column) + (self.parking_height // (self.places_per_column * 2) - self.car_height // 2)
                    parking_spots.append(pygame.Rect(x, y, self.car_width, self.car_height))
        return parking_spots    

    def __init_queue_spots(self):
        queue_spots = []
        for queue in range(self.queue_places_per_column):
            for floor in range(self.floors):
                for col in range(2):
                    x = floor * self.floor_width + col * (self.floor_width // 2) + (self.floor_width // 4 - self.car_width // 2)
                    y = queue * (self.parking_height // self.places_per_column) + (self.parking_height // (self.places_per_column * 2) - self.car_height // 2) + self.empty_height + self.parking_height
                    queue_spots.append(pygame.Rect(x, y, self.car_width, self.car_height))
        return queue_spots
    
    def __init_floor_character_spots(self):
        floor_character_spots = []
        for floor in range(self.floors):
            x = floor * self.floor_width + self.floor_width // 2 - 3 * self.floor_font_size // 10
            y = self.parking_height // 2 - 3 * self.floor_font_size // 10
            floor_character_spots.append((string.ascii_uppercase[floor], (x, y)))
        return floor_character_spots
    
    def __drawing_loop(self):
        # TODO : Implement fps for drawing
        self.screen = pygame.display.set_mode((self.screen_width, self.screen_height))
        pygame.display.set_caption(self.simulator_caption)
        while self.running:
            self.__draw()
    
    def __draw(self):
        # Clear screen
        self.screen.fill(self.black_color)

        # Game area background
        pygame.draw.rect(self.screen, self.gray_color, pygame.Rect(0, 0, self.game_area_width, self.screen_height))

        # Draw vertical lines to divide floors
        for i in range(1, self.floors):
            pygame.draw.line(self.screen, self.yellow_color, (i * self.floor_width, 0), (i * self.floor_width, self.parking_height), 5)

        # Draw horizontal line to separate parking and queue areas
        pygame.draw.line(self.screen, (60, 30, 20), (3 * self.car_width // 2, self.parking_height + self.empty_height - 10), (self.game_area_width, self.parking_height + self.empty_height - 10), 10)

        # Draw parking spot lines
        for park_ind in range(self.cars_per_floor):
            for floor in range(self.floors):
                spot = self.parking_spots[floor * self.cars_per_floor + park_ind]
                color = self.white_color
                if self.subscriptions.get_subscription(floor, park_ind) is not None:
                    color = self.red_color
                pygame.draw.rect(self.screen, color, spot, 2)
                id_text = self.display_font.render(str(park_ind + 1), True, color)
                text_rect = id_text.get_rect(center=spot.center)
                self.screen.blit(id_text, text_rect)

        # Draw queue spot lines
        for queue in self.queue_spots:
            pygame.draw.rect(self.screen, self.white_color, queue, 2)

        # Draw Floor Characters
        for char, position in self.floor_character_spots:
            text = self.floor_font.render(char, True, self.white_color)
            self.screen.blit(text, position)

        # Draw parked cars
        for car, spot in zip(self.parking_lot.get_1D_spots(), self.parking_spots):
            if car is not None:
                pygame.draw.rect(self.screen, car[0].car_color, spot)
                text = f"{car[0].car_id}"
                if car[0].subscribed:
                    text += "*"
                id_text = self.display_font.render(text, True, self.white_color)
                text_rect = id_text.get_rect(center=spot.center)
                self.screen.blit(id_text, text_rect)

        # Display queue cars
        for car, queue_spot in zip(self.car_queue.get_queue(), self.queue_spots):
            pygame.draw.rect(self.screen, car.car_color, queue_spot)
            text = f"{car.car_id}"
            if car.subscribed:
                text += "*"
            id_text = self.display_font.render(text, True, self.white_color)
            text_rect = id_text.get_rect(center=queue_spot.center)
            self.screen.blit(id_text, text_rect)

        # Display cars per floor information
        for floor in range(self.floors):
            floor_cars = self.parking_lot.get_number_of_cars(floor)
            floor_text = self.display_font.render(f'Floor {floor + 1}: {floor_cars} cars', True, self.text_color)
            self.screen.blit(floor_text, (self.game_area_width + 20, 20 + floor * 30))

        (avg_time, min_time, max_time) = self.serial_manager.get_statistics()

        # Display statistics
        self.screen.blit(self.display_font.render(f"Time statistics:" , True, self.text_color),
                          (self.game_area_width + 20, 40 + (self.floors + 0) * 30))
        self.screen.blit(self.display_font.render(f"Average: {avg_time:05.2f}" , True, self.text_color),
                          (self.game_area_width + 20, 40 + (self.floors + 1) * 30))
        self.screen.blit(self.display_font.render(f"Minimum: {min_time:05.2f}" , True, self.text_color),
                          (self.game_area_width + 20, 40 + (self.floors + 2) * 30))
        self.screen.blit(self.display_font.render(f"Maximum: {max_time:05.2f}" , True, self.text_color),
                          (self.game_area_width + 20, 40 + (self.floors + 3) * 30))
        
        self.screen.blit(self.display_font.render(f"Total Earnings:" , True, self.text_color),
                          (self.game_area_width + 20, 60 + (self.floors + 4) * 30))
        self.screen.blit(self.display_font.render(f"Simulator: {self.simulator_fee:03}" , True, self.text_color),
                            (self.game_area_width + 20, 60 + (self.floors + 5) * 30))
        self.screen.blit(self.display_font.render(f"Received: {self.calculated_fee:03}" , True, self.text_color),
                            (self.game_area_width + 20, 60 + (self.floors + 6) * 30))
        
        self.screen.blit(self.display_font.render(f"Empty Places:" , True, self.text_color),
                          (self.game_area_width + 20, 80 + (self.floors + 7) * 30))
        self.screen.blit(self.display_font.render(f"Simulator: {((self.floors * self.cars_per_floor) -self.parking_lot.get_total_cars()):02}" , True, self.text_color),
                            (self.game_area_width + 20, 80 + (self.floors + 8) * 30))
        self.screen.blit(self.display_font.render(f"Received: : {self.received_empty_spaces:02}" , True, self.text_color),
                            (self.game_area_width + 20, 80 + (self.floors + 9) * 30))

        
        
        self.screen.blit(self.display_font.render(f"Status:" , True, self.text_color), (self.game_area_width + 20, self.screen_height - 40))
        if self.game_status == 0:
            self.screen.blit(self.display_font.render(f"WAITING" , True, self.text_color), (self.game_area_width + 100, self.screen_height - 40))
        elif self.game_status == 1:
            self.screen.blit(self.display_font.render(f"RUNNING" , True, self.green_color), (self.game_area_width + 100, self.screen_height - 40))
        elif self.game_status == 2:
            self.screen.blit(self.display_font.render(f"FINISHED" , True, self.red_color), (self.game_area_width + 100, self.screen_height - 40))

        # Update display
        pygame.display.flip()

class SerialManager:

    WAITING = 0
    GETTING = 1

    def __init__(self, port, baudrate, parity, rtscts, xonxoff):
        try:
            if BOARD_SIMULATION == False:
                self.serial = serial.Serial(port, baudrate, parity=parity,
                                    rtscts=rtscts, xonxoff=xonxoff, timeout=None)
        except serial.SerialException as e:
            raise
        self.state = self.WAITING
        self.data = b''
        self.time = 0
        self.avg_time = -1
        self.max_time = float('-inf')
        self.min_time = float('inf')
        self.prev_time = -1
        self.startTime = -1
        self.endTime = -1
        self.cmd_count = 0
        self.messages = queue.Queue()
        self.writer_lock = threading.Lock()
        self.statistics_lock = threading.Lock()
        self.running = False

        if BOARD_SIMULATION == False:
            self.receiver_thread = threading.Thread(target=self.read, daemon=True)

    def start(self):
        self.startTime = time.time()
        self.cmd_count = 0
        self.state = self.WAITING
        if BOARD_SIMULATION == False:
            self.running = True
            self.receiver_thread.start()

    def stop(self):
        if self.running:
            self.running = False
            self.receiver_thread.join()
            self.serial.close()

    def __update_statistics(self):
        with self.statistics_lock:
            self.time = time.time()
            total_time_passed = (self.time - self.startTime) * 1000.0
            if self.cmd_count > 0:
                self.avg_time = (total_time_passed) / self.cmd_count
            else:
                self.avg_time = 0

            if self.prev_time != -1:
                time_passed = (self.time - self.prev_time) * 1000.0
                if time_passed < self.min_time or self.min_time == -1:
                    self.min_time = time_passed
                if time_passed > self.max_time or self.max_time == -1:
                    self.max_time = time_passed
            self.prev_time = self.time

    def get_statistics(self):
        with self.statistics_lock:
            if self.cmd_count == 0:
                return 0, 0, 0
            else:
                return self.avg_time, self.min_time, self.max_time

    def read(self):
        while self.running:
            byte = self.serial.read(1)
            if self.state == self.WAITING:
                if byte == b'$':
                    self.state = self.GETTING
                    if (self.data != b''):
                        print("Error: Uncomplete message received.")
                    self.data = b''
            elif self.state == self.GETTING:
                if byte == b'#':
                    if (checkMessage(self.data)):
                        self.messages.put(self.data)
                        self.cmd_count += 1
                        self.data = b''
                        self.__update_statistics()
                    else:
                        print("Error: Invalid message received.")
                    self.data = b''
                    self.state = self.WAITING
                else:
                    self.data = b''.join([self.data, byte])                      

    def write(self, data):
        with self.writer_lock:
            if BOARD_SIMULATION == False:
                self.serial.write(data)

class BoardSimulator:
    def __init__(self, debug_messages, debug_commands):
        self.parking_lot = [[None for _ in range(10)] for _ in range(4)]
        self.subscribed_cars = {}
        self.subscribed_places = {}
        self.car_queue = queue.Queue()
        self.running = True
        self.simulation_started = False
        self.debug_messages = debug_messages
        self.debug_commands = debug_commands
        self.simulator_thread = threading.Thread(target=self.__simulate_board, daemon=True)
        self.simulator_thread.start()

    def __get__empty_spaces(self):
        empty_spaces = 0
        for floor in self.parking_lot:
            for spot in floor:
                if spot is None:
                    empty_spaces += 1
        return empty_spaces
    
    def stop(self):
        self.running = False
        self.simulator_thread.join()

    def __process_park_message(self, car_id):
        if car_id in self.subscribed_cars:
            floor, spot = self.subscribed_cars[car_id]
            if self.parking_lot[floor][spot] is not None:
                self.car_queue.put(car_id)
            else:
                current_time = time.time()
                self.parking_lot[floor][spot] = (car_id, current_time)
                self.debug_messages.put(b'SPC' + f"{car_id:03}".encode('ascii') 
                                        + chr(floor + 65).encode('ascii') + f"{spot + 1:02}".encode('ascii'))
        else:
            car_found = False
            for floor in range(4):
                for spot in range(10):
                    if self.parking_lot[floor][spot] is None and (floor, spot) not in self.subscribed_places:
                        current_time = time.time()
                        self.parking_lot[floor][spot] = (car_id, current_time)
                        self.debug_messages.put(b'SPC' + f"{car_id:03}".encode('ascii') 
                                                + chr(floor + 65).encode('ascii') + f"{spot + 1:02}".encode('ascii'))
                        car_found = True
                        break
                else:
                    continue
                break
            if not car_found:
                self.car_queue.put(car_id)
                self.debug_messages.put(b'EMP' + f"{self.__get__empty_spaces():02}".encode('ascii'))

    def __simulate_board(self):
        while self.running:
            time.sleep(0.1)
            if self.debug_commands.empty():
                if self.simulation_started:
                    if not self.car_queue.empty():
                        car_id = self.car_queue.get()
                        self.__process_park_message(car_id)
                    else:
                        self.debug_messages.put(b'EMP' + f"{self.__get__empty_spaces():02}".encode('ascii'))
            else:
                command = self.debug_commands.get()
                if command.startswith(b'GO'):
                    self.simulation_started = True
                if self.simulation_started:
                    if command.startswith(b'EXT'):
                        car_id = int(command[3:6].decode('ascii'))
                        if car_id in self.subscribed_cars:
                            self.debug_messages.put(b'FEE' + f"{car_id:03}".encode('ascii') + f"{000:03}".encode('ascii'))
                            floor, spot = self.subscribed_cars[car_id]
                            self.parking_lot[floor][spot] = None
                        else:
                            for floor in range(4):
                                for spot in range(10):
                                    if self.parking_lot[floor][spot] is not None:
                                        if self.parking_lot[floor][spot][0] == car_id:
                                            current_time = time.time()
                                            parkedTime = self.parking_lot[floor][spot][1]
                                            fee = int(4 * (current_time - parkedTime))
                                            self.debug_messages.put(b'FEE' + f"{car_id:03}".encode('ascii') + f"{fee:03}".encode('ascii'))
                                            self.parking_lot[floor][spot] = None
                                            break
                                else:
                                    continue
                                break
                    elif command.startswith(b'PRK'):
                        car_id = int(command[3:6].decode('ascii'))
                        self.__process_park_message(car_id)
                    elif command.startswith(b'SUB'):
                        car_id = int(command[3:6].decode('ascii'))
                        floor = ord(command[6:7].decode('ascii')) - ord('A')
                        spot = int(command[7:9].decode('ascii')) - 1
                        
                        if (floor, spot) in self.subscribed_places:
                            fee = 0
                        elif car_id in self.subscribed_cars:
                            fee = 0
                        elif self.parking_lot[floor][spot] is not None:
                            fee = 0
                        else:
                            fee = 50
                            self.subscribed_cars[car_id] = (floor, spot)
                            self.subscribed_places[(floor, spot)] = car_id

                        self.debug_messages.put(b'RES' + f"{car_id:03}".encode('ascii') + f"{fee:02}".encode('ascii'))
                    elif command.startswith(b'END'):
                        self.simulation_started = False
                        self.running = False

class GameEngine:
    def __init__(self, screen_width, screen_height, display_width, simulator_caption, floors, cars_per_floor
                 , serial_port='/dev/ttyUSB0', baudrate=115200, parity=serial.PARITY_NONE, rtscts=False, xonxoff=False):
        try:
            self.serial_manager = SerialManager(serial_port, baudrate, parity, rtscts, xonxoff)
        except serial.SerialException as e:
            print(f"Error opening serial port: {e}")
            print("Please check the serial port and try again.")
            sys.exit()
        pygame.init()
        self.parking_lot = ParkingLot(floors, cars_per_floor)
        self.car_queue = CarQueue(4 * floors)
        self.nonparking_cars = []
        self.status = 0
        for car_id in range(100):
            car_color = random.choice(car_colors)
            new_car = Car(car_id, car_color, False)
            self.nonparking_cars.append(new_car)
        self.subscribed_cars = {}
        self.nonparking_subscribed_cars = []
        self.subsriptions = Subscriptions()
        self.cars_waiting_to_exit = []
        self.cars_waiting_to_subscribe = {}
        self.no_cars_in_game = 0
        self.lock = threading.Lock()
        self.generate = False
        self.event_generator_thread = threading.Thread(target=self.__event_generator_loop, daemon=True)
        self.drawer = Drawer(screen_width, screen_height, display_width, floors, cars_per_floor, 
                             simulator_caption, self.car_queue, self.parking_lot, self.subsriptions, self.serial_manager)
        self.drawer.game_status = 0
        self.automatic_mode = True
        if BOARD_SIMULATION == True:
            self.debug_messages = queue.Queue()
            self.debug_commands = queue.Queue()
            self.board_simulator = BoardSimulator(self.debug_messages, self.debug_commands)

    def __stop(self):
        if self.generate:
            self.generate = False
            self.event_generator_thread.join()
        if BOARD_SIMULATION == True:
            self.board_simulator.stop()
        self.drawer.stop()
        self.serial_manager.stop()
        pygame.quit()
        sys.exit()

    def __send_command(self, code, XXX: int = -1, Y: str = '?', ZZ: int = -1):
        command = b''
        if code == "GO":
            command = b'GO'
        elif code == "END":
            command = b'END'
        elif code == "EXT":
            if (XXX < 0 or XXX > 999):
                debug_print("Error: Invalid XXX for EXT command.")
                return
            command = b'EXT' + f"{XXX:03}".encode('ascii')
        elif code == "PRK":
            if (XXX < 0 or XXX > 999):
                debug_print("Error: Invalid XXX for PRK command.")
                return
            command = b'PRK' + f"{XXX:03}".encode('ascii')
        elif code == "SUB":
            if (XXX < 0 or XXX > 999):
                debug_print("Error: Invalid XXX for SUB command.")
                return
            if (Y not in ['A', 'B', 'C', 'D']):
                debug_print("Error: Invalid Y for SUB command.")
                return
            if (ZZ < 0 or ZZ > 10):
                debug_print("Error: Invalid ZZ for SUB command.")
                return
            command = b'SUB' + f"{XXX:03}".encode('ascii') + Y.encode('ascii') + f"{ZZ:02}".encode('ascii')
        else:
            debug_print("Error: Invalid command.")
            return
        
        if BOARD_SIMULATION == True:
            self.debug_commands.put(command)
        else:
            self.serial_manager.write(b'$' + command + b'#')

    def __add_car(self, car):
        with self.lock:
            if car not in self.nonparking_cars:
                debug_print(f": Car{car.car_id} is not in the non-parking list.")
                return False

            if car.car_id in self.cars_waiting_to_subscribe:
                debug_print(f"Error: Car{car.car_id} is waiting to be subscribe cannot be added.")
                return False
            
            if self.car_queue.is_full():
                # TODO: Point reduction
                print("Car queue is full. New cars cannot enter.")
                return False
            
            self.nonparking_cars.remove(car)
            if car in self.nonparking_subscribed_cars:
                self.nonparking_subscribed_cars.remove(car)
            
            self.car_queue.add_car(car)
            self.no_cars_in_game += 1

        self.__send_command("PRK", car.car_id)  

        return True

    def __add_random_car(self):
        with self.lock:
            random_car = random.choice(self.nonparking_cars)
        self.__add_car(random_car)

    def __exit_car(self, car):
        with self.lock:
            if car in self.nonparking_cars:
                debug_print(f"Error: Car{car.car_id} is already in the non-parking list.")
                return False

            if car in self.cars_waiting_to_exit:
                debug_print(f"Error: Car{car.car_id} is already in the waiting list.")
                return False
        
        self.cars_waiting_to_exit.append(car)
        self.__send_command("EXT", car.car_id)
        return True
    
    def __exit_random_car(self):
        if self.parking_lot.get_total_cars() == 0:
            debug_print("No cars in the parking lot.")
            return False
        with self.lock:
                random_car = random.choice(self.parking_lot.get_all_cars())
        self.__exit_car(random_car)        
        return True
    
    def __subscribe_car(self, car, letter, spot):
        with self.lock:
            if car not in self.nonparking_cars:
                debug_print(f"Error: Car{car.car_id} is not in the non-parking list.")
                return False
            
            if car.car_id in self.subscribed_cars:
                debug_print(f"Error: Car{car.car_id} is already subscribed.")
                return False

            if car.car_id in self.cars_waiting_to_subscribe:
                debug_print(f"Error: Car{car.car_id} is already waiting to be subscribe.")
                return False
            
            self.cars_waiting_to_subscribe[car.car_id] = {
                "floor": letter,
                "spot": spot
            }

            self.nonparking_subscribed_cars.append(car)

        self.__send_command("SUB", car.car_id, letter, spot)  
        return True
    
    def __subscribe_random_car(self):
        with self.lock:
            random_car = random.choice(self.nonparking_cars)
            random_letter = random.choice("ABCD")
            random_spot = random.randint(1, 10)
        self.__subscribe_car(random_car, random_letter, random_spot)

    def __add_random_subscribed_car(self):
        if len(self.subscribed_cars) == 0:
            debug_print("There is no subscribed car.")
            return False
        
        if len(self.nonparking_subscribed_cars) == 0:
            debug_print("There is no non-parking subscribed car.")
            return False
        
        with self.lock:
            random_car = random.choice(self.nonparking_subscribed_cars)
        self.__add_car(random_car)

    def __handle_empty_space_message(self, empty_spaces):
        with self.lock:
            self.drawer.received_empty_spaces = empty_spaces

    def __handle_parking_space_message(self, car_id, floor, spot):
        car_in_queue = None
        for car in self.car_queue.get_queue():
            if car.car_id == car_id:
                car_in_queue = car
                break
        if car_in_queue is None:
            print(f"Error: Trying to park Car{car_id} which is not in the queue.")
            return
        
        if self.parking_lot.read_spot_raw(floor, spot) is not None:
            print(f"Error: Spot {spot} on floor {floor} is already occupied.")
            return
        
        with self.lock:
            if car_id in self.subscribed_cars:
                if self.subscribed_cars[car_id]["floor"] != floor or self.subscribed_cars[car_id]["spot"] != spot:
                    print(f"Error: Car{car_id} is subscribed to a different spot.")
                    print(f"Car{car_id} cannot be parked at this spot.")
                    return

        subscribed_car_at_spot = self.subsriptions.get_subscription_raw(floor, spot)
        if subscribed_car_at_spot is not None:
            if subscribed_car_at_spot != car_id:
                print(f"Error: Floor {floor} and spot {spot} is already occupied by Car{subscribed_car_at_spot}.")
                print(f"Car{car_id} cannot be parked at this spot.")
                return
        
        self.car_queue.remove_car(car_in_queue)
        self.parking_lot.park_car_raw(floor, spot, car_in_queue)

    def __calculate_fee(self, time_passed):
        return int(time_passed / 250) + 1
    
    def __get_subscription_fee(self):
        return 50
    
    def __handle_fee_message(self, car_id, fee):
        exiting_car = None
        with self.lock:
            for car in self.cars_waiting_to_exit:
                if car.car_id == car_id:
                    exiting_car = car
                    break
        if exiting_car is None:
            print(f"Error: Car{car_id} is not in the waiting list.")
            return
        
        if car not in self.parking_lot.get_all_cars():
            print(f"Error: Car{car_id} is not in the parking lot, cannot exit.")
            return
        
        simulator_fee = 0
        subscribed = False
        car, entry_time = self.parking_lot.remove_car_by_id(car_id)
        time_passed = time.time() - entry_time
        with self.lock:
            if car_id in self.subscribed_cars:
                simulator_fee = 0
                subscribed = True
            else:
                simulator_fee = self.__calculate_fee(time_passed * 1000.0)
                
            self.drawer.calculated_fee += fee
            self.drawer.simulator_fee += simulator_fee
           
            self.cars_waiting_to_exit.remove(exiting_car)
            self.nonparking_cars.append(car)
            self.no_cars_in_game -= 1
            if subscribed:
                self.nonparking_subscribed_cars.append(car)
            
    def __handle_res_message(self, car_id, fee):
        already_subscribed = False
        with self.lock:
            subcribing_car = None
            for car in self.nonparking_cars:
                if car.car_id == car_id:
                    subcribing_car = car
                    break
            if subcribing_car is None:
                print(f"Error: Car{car_id} is not in the non-parking list.")
                return

            if car_id not in self.cars_waiting_to_subscribe:
                print(f"Error: Car{car_id} is not in the waiting subcriber list.")
                return False

            if fee != 0 and fee != 50:
                print(f"Wrong fee {fee} for Car{car_id}.")
                if subcribing_car in self.nonparking_subscribed_cars:
                    self.nonparking_subscribed_cars.remove(subcribing_car)
                    return False

            floor = self.cars_waiting_to_subscribe[car_id]["floor"]
            spot = self.cars_waiting_to_subscribe[car_id]["spot"]

            if self.parking_lot.read_spot_raw(floor, spot) is not None:
                if fee != 0:
                    print(f"Error: Spot {spot} on floor {floor} is already occupied. Cannot subscribe.")
                if subcribing_car in self.nonparking_subscribed_cars:
                    self.nonparking_subscribed_cars.remove(subcribing_car)
                    return False
            
            if car_id in self.subscribed_cars:
                if fee == 0:
                    already_subscribed = True
                else:
                    print(f"Error: Car{car_id} is already subscribed.")
                    return False

        if self.subsriptions.get_subscription_raw(floor, spot) is not None:
            if fee == 0:
                already_subscribed = True
            else:
                print(f"Error: Spot {spot} on floor {floor} is already subscribed.")
                return False
        
        with self.lock:
            if not already_subscribed:
                if fee == 50:
                    for car in self.nonparking_subscribed_cars:
                        if car.car_id == car_id:
                            car.subscribed = True
                            break
                elif fee == 0:
                    print(f"Error: Car{car_id} could be subscribed but rejected.")
                    if subcribing_car in self.nonparking_subscribed_cars:
                        self.nonparking_subscribed_cars.remove(subcribing_car)
                        return False
            else:
                if subcribing_car in self.nonparking_subscribed_cars:
                    self.nonparking_subscribed_cars.remove(subcribing_car)
            
            del self.cars_waiting_to_subscribe[car_id]

        if not already_subscribed:
            self.subscribed_cars[car_id] = {
                "floor": floor,
                "spot": spot
            }
            self.subsriptions.add_subscription_raw(car_id, floor, spot)
            
            with self.lock:
                simulated_fee = self.__get_subscription_fee()
                self.drawer.calculated_fee += fee
                self.drawer.simulator_fee += simulated_fee
        
    def __process_message(self, message):
        if message.startswith(b'EMP'):
            empty_spaces = int(message[3:5].decode('ascii'))
            if empty_spaces < 0 or empty_spaces > 40:
                print(f"Error: Invalid number of empty spaces {empty_spaces}.")
                return
            self.__handle_empty_space_message(empty_spaces)

        elif message.startswith(b'SPC'):
            car_id = int(message[3:6].decode('ascii'))
            floor = message[6:7].decode('ascii')
            spot = int(message[7:9].decode('ascii'))
            if floor not in ['A', 'B', 'C', 'D']:
                print(f"Error: Invalid floor {floor} in SPC command.")
                return
            if spot < 1 or spot > 10:
                print(f"Error: Invalid spot {spot} in SPC command.")
                return
            self.__handle_parking_space_message(car_id, floor, spot)

        elif message.startswith(b'FEE'):
            car_id = int(message[3:6].decode('ascii'))
            fee = int(message[6:9].decode('ascii'))
            self.__handle_fee_message(car_id, fee)

        elif message.startswith(b'RES'):
            car_id = int(message[3:6].decode('ascii'))
            fee = int(message[6:8].decode('ascii'))
            self.__handle_res_message(car_id, fee)

        else:
            print("Unknown message received.")

    def __receive_messages(self):
        while self.serial_manager.running and self.running:
            if not self.serial_manager.messages.empty():
                message = self.serial_manager.messages.get()
                self.__process_message(message)
            else:
                break

    def __debug_receive_messages(self):
        while self.running:
            if not self.debug_messages.empty():
                message = self.debug_messages.get()
                self.__process_message(message)
            else:
                break

    def __event_generator_loop(self):
        while self.generate:
            random_number = random.randint(0, 1000)
            with self.lock:
                no_cars = self.no_cars_in_game
                coefficient = int(200 * no_cars / 35)

            if random_number < 600 - coefficient and no_cars < 45:
                # Randomly add a car
                self.__add_random_car()
            elif random_number < 800 and self.parking_lot.get_total_cars() > 0:
                # Randomly request a car to exit
                self.__exit_random_car()
            elif random_number < 850 and len(self.subscribed_cars) < 10:
                # Randomly request subscribe a car
                self.__subscribe_random_car()
            elif random_number < 900 and len(self.nonparking_subscribed_cars) > 0:
                # Randomly add a subscribed car
                self.__add_random_subscribed_car()
            elif random_number <= 1000:
                # DO NOTHING
                pass

            time.sleep(0.11)             

    def run(self):
        # Waiting loop
        while True:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    self.__stop()
                elif event.type == pygame.KEYDOWN:
                    if event.key == pygame.K_ESCAPE:
                        self.__stop()
                    elif event.key == pygame.K_a:
                        self.automatic_mode =True
                        break
                    elif event.key == pygame.K_m:
                        self.automatic_mode = False
                        break
            else:
                continue
            break

        # Start the game
        self.serial_manager.start()
        self.running = True
        start_time = time.time()
        self.__send_command("GO")
        self.status = 1
        self.drawer.game_status = 1
        if self.automatic_mode:
            self.generate = True
            self.event_generator_thread.start()

        # Main running loop
        while self.running:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    self.__stop()
                elif event.type == pygame.KEYDOWN:
                    if event.key == pygame.K_ESCAPE:
                        self.__stop()
                    if not self.automatic_mode:
                        if event.key == pygame.K_r:
                            self.__add_random_car()
                        elif event.key == pygame.K_t:
                            self.__exit_random_car()
                        elif event.key == pygame.K_y:
                            self.__subscribe_random_car()
                        elif event.key == pygame.K_u:
                            self.__add_random_subscribed_car()                    

            if BOARD_SIMULATION == True: 
                self.__debug_receive_messages()
            else:
                self.__receive_messages()            

            current_time = time.time()
            
            if (current_time - start_time) >= 60:
                self.__send_command("END")
                self.running = False

        self.status = 2
        self.drawer.game_status = 2
        self.generate = False
        self.serial_manager.stop()

        # Loop to wait for the user to close the window
        while True:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    self.__stop()
                elif event.type == pygame.KEYDOWN:
                    if event.key == pygame.K_ESCAPE:
                        self.__stop()

if __name__ == "__main__":
    game_engine = GameEngine(SCREEN_WIDTH, SCREEN_HEIGHT, DISPLAY_WIDTH, SIMULATOR_CAPTION, FLOORS, CARS_PER_FLOOR
                             , SERIAL_PORT, BAUDRATE, PARITY, RTSCTS, XONXOFF)
    game_engine.run()
