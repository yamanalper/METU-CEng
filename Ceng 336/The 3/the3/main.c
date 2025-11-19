#include "pragmas.h"
#include <xc.h>


// ============================ //
//        DEFINITIONS           //
// ============================ //


typedef enum {
    INBUF = 0, OUTBUF = 1
} buf_t;

// structure for a single parking slot
typedef struct {
    int id;
    int subscribe;  // flag if it is subscribed slot.
    int entry_time; // time when car entered this slot
    int occupied;  // flag if it is occupied
} Slot;

// structure for a floor in the parking lot.
typedef struct {
    char level;
    Slot slots[10];
    int available_f; // Number of available slots on this floor
} Floor;

// Structure for the entire parking lot.
typedef struct {
    int available; // Total number of available slots in the entire parking lot
    Floor floors[4];
} Park;

// Structure for a queue of cars waiting to park.
typedef struct {
    int cars[16];
    int front;  // Index of the front of the queue
    int count;
} CarQueue;
// ============================ //
//          GLOBALS             //
// ============================ //

uint8_t chr_buf; // for received characters
int run_flag = 0; // Flag to control the main program loop
uint8_t inbuf[256]; // Input buffer for received UART data
uint8_t outbuf[256];  // Output buffer for data to be sent via UART
volatile uint8_t head[2] = {0, 0}; // Head pointers for INBUF and OUTBUF
volatile uint8_t tail[2] = {0, 0}; // Tail pointers for INBUF and OUTBUF
Park park = {0}; // Global instance of the parking lot structure
CarQueue q; // Global instance of the car queue structure
char buffer[20]; // Buffer to store incoming messages from UART
unsigned int level_counter = 0; // Counter for ADC readings
unsigned int time_counter = 5; // Global time counter, increments every 5ms
unsigned int message_counter = 0; // Counter for sending messages
int current_level; // Stores the current level determined by ADC
int tick_5 = 0; // Flag set by Timer0 interrupt approximately every 5ms
int temp_id = 0; // Temporary variable to hold car ID
int new_message_flag = 0; // Flag to indicate a new message
const unsigned char seg_list[10] = {
    0b00111111,
    0b00000110,
    0b01011011,
    0b01001111,
    0b01100110,
    0b01101101,
    0b01111101,
    0b00000111,
    0b01111111,
    0b01101111
};
unsigned int display_counter = 0; // Counter for 7-segment display digits
int display_mode = 0; // Display mode: 0 for total score, 1 for available slots
unsigned int total_score = 0; // parking fees and subscription fees
int message_flag = 0; // Flag for a complete message
int send_message_flag = 0; // Flag time to send a periodic message
int msg_len = 0; // Length of the current incoming message
int in_packet = 0; // Flag to indicate if currently receiving a packet
volatile uint8_t prev_rb4 = 0;  // Prev RB4 value
int switch_flag = 0; // Flag for RB4
uint8_t curr_rb4; // Current RB4 value


int find_park_available_slot(char *level, int *slot, int temp_id);
void find_parked(int id, char *level, int *slot);
void enqueue(int car_id);
int dequeue();
int buf_isempty(buf_t typ);
void buf_push(uint8_t v, buf_t buf);
uint8_t buf_pop(buf_t buf);
int is_same(const char* a, const char* b, int n);
void init_adc();
void init_ports();
void init_others();
void handle_char(uint8_t chr_buf);
void receive_message();
void eval_message();
void send_message();
void check_level();
void transmit_isr();

// ============================ //
//          FUNCTIONS           //
// ============================ //

// Disables UART receive and transmit interrupts.
void disable_rxtx( void ) { PIE1bits.RC1IE = 0;PIE1bits.TX1IE = 0;}
// Enables UART receive and transmit interrupts.
void enable_rxtx( void )  { PIE1bits.RC1IE = 1;PIE1bits.TX1IE = 1;}
// Disables Global and Peripheral Interrupts
void disable_i(){INTCONbits.GIE = 0;INTCONbits.PEIE = 0;}
// Enables Global and Peripheral Interrupts
void enable_i(){INTCONbits.GIE = 1;INTCONbits.PEIE = 1;}

/* Finds the first available (empty and unsubscribed) parking slot across all floors and park there.
 It iterates through each floor and then each slot to find a suitable space. Level pointer
 to a char where the level of the found slot will be stored. Slot pointer to an int where the
 0-indexed slot number will be stored. Return 1 if an available slot is found, 0 otherwise.
 */
int find_park_available_slot(char *level, int *slot, int temp_id) {
    for (int k = 0; k < 4; k++) {
        for (int j = 0; j < 10; j++) {
            if (park.floors[k].slots[j].entry_time == 0 && park.floors[k].slots[j].subscribe == 0 && park.floors[k].slots[j].occupied == 0) {
                park.floors[k].slots[j].id = temp_id;
                park.floors[k].slots[j].entry_time = time_counter;
                park.floors[k].slots[j].occupied = 1;
                park.floors[k].available_f--;
                park.available--;
                *level = park.floors[k].level;
                *slot = j;
                return 1;
            }
        }
    }
    return 0;
}

/* Finds a parking slot that is subscribed by a specific car ID. It iterates through all
 floors and slots to locate the subscribed slot. Level pointer to a char where the level of the
 found slot will be stored. Slot pointer to an int where the 0-indexed slot number will be stored.
 The ID of the car to search for in subscribed slots. return 1 if a subscribed slot for the given
 ID is found, 0 otherwise.
 */
int find_subscribed(char *level, int *slot, int id){
    for(int k = 0; k < 4; k++){
        for(int j = 0; j < 10; j++){
            if(park.floors[k].slots[j].id == id && park.floors[k].slots[j].subscribe == 1){
                *level = park.floors[k].level;
                *slot = j;
                return 1;
            }
        }
    }
    return 0;
}

/* Finds the parking slot where a specific car (identified by its id and occupied flag) is currently parked.It
 searches through all slots to locate the car. ID of the car to search for. level pointer to a char
 where the level of the parked slot will be stored. slot pointer to an int where the 0-indexed
 slot number will be stored.
 */
void find_parked(int id, char *level, int *slot) {
    for (int k = 0; k < 4; k++) {
        for (int j = 0; j < 10; j++) {
            if (park.floors[k].slots[j].id == id && park.floors[k].slots[j].occupied == 1) {
                *level = park.floors[k].level;
                *slot = j;
                return;
            }
        }
    }
}

// Updates the 7-segment display by multiplexing the digits.
// It alternates between displaying the total accumulated money and the number of
// empty spaces on the current level.
void update_display() {
    uint8_t bit3 = 0, bit2 = 0, bit1 = 0, bit0 = 0;

    if (display_mode == 0) {
        uint16_t val = total_score;
        bit3 = seg_list[(val / 1000) % 10];
        bit2 = seg_list[(val / 100) % 10];
        bit1 = seg_list[(val / 10) % 10];
        bit0 = seg_list[val % 10];
    } else {
        int empty = park.floors[current_level].available_f;
        bit3 = 0x00;
        bit2 = 0x00;
        bit1 = seg_list[(empty / 10) % 10];
        bit0 = seg_list[empty % 10];
    }

    display_counter++;
    if (display_counter == 1) {
        LATHbits.LATH3 = 0;
        LATHbits.LATH2 = 0;
        LATHbits.LATH1 = 0;
        LATHbits.LATH0 = 1;
        LATJ = bit3;
    } else if (display_counter == 2) {
        LATHbits.LATH3 = 0;
        LATHbits.LATH2 = 0;
        LATHbits.LATH1 = 1;
        LATHbits.LATH0 = 0;
        LATJ = bit2;
    } else if (display_counter == 3) {
        LATHbits.LATH3 = 0;
        LATHbits.LATH2 = 1;
        LATHbits.LATH1 = 0;
        LATHbits.LATH0 = 0;
        LATJ = bit1;
    } else if (display_counter == 4) {
        LATHbits.LATH3 = 1;
        LATHbits.LATH2 = 0;
        LATHbits.LATH1 = 0;
        LATHbits.LATH0 = 0;
        LATJ = bit0;
        display_counter = 0;
    }
}

// Enqueues a car ID into the CarQueue.
// Cars are added to the end of the queue, waiting for an available parking space.
// car_id The integer ID of the car to be added to the queue.
void enqueue(int car_id) {
    if (q.count < 16) {
        q.cars[q.count] = car_id;
        q.count++;
    }
}

// Dequeues a car ID from the front of the CarQueue with FIFO behavior.
int dequeue() {
    if (q.count > 0) {
        int car_id = q.cars[0];
        // Shift all subsequent elements forward by one position to fill the gap left by the dequeued car
        for (int i = 1; i < q.count; i++) {
            q.cars[i - 1] = q.cars[i];
        }
        q.count--;
        return car_id;
    }
    return 0;
}

// Check if buffer is empty. Empty buffer is indicated when its head and tail pointers are equal.
int buf_isempty(buf_t typ) {
    if (head[typ] == tail[typ]) return 1;
    else return 0;
}

// Pushes a byte v onto the buffer. The byte is added at the current head position, and
// the head pointer is then incremented, wrapping around if it reaches the end of the buffer.

void buf_push(uint8_t v, buf_t buf) {
    // Store the value v at the current head position of the buffer
    if (buf == INBUF) inbuf[head[buf]] = v;
    else outbuf[head[buf]] = v;
    head[buf]++;
    // If the head pointer reaches the end of the 256-byte buffer, wrap it around to 0.
    if (head[buf] == 256) head[buf] = 0;
}

// Pops a byte from the buffer. The byte is retrieved from the current tail position, and the tail
// pointer is then incremented, wrapping around if it reaches the end of the buffer.
uint8_t buf_pop(buf_t buf) {
    uint8_t v;
    if (buf_isempty(buf)) {
        return 0;
    } else {
        // Retrieve the value from the current tail position of the specified buffer
        if (buf == INBUF) v = inbuf[tail[buf]];
        else v = outbuf[tail[buf]];
        tail[buf]++;
        // If the tail pointer reaches the end of the 256-byte buffer, wrap it around to 0.
        if (tail[buf] == 256) tail[buf] = 0;
        return v;
    }
}

// Compare first n characters of two strings. Used for comparing commands with predefined strings.
int is_same(const char* a, const char* b, int n) {
    for (int k = 0; k < n; k++) {
        if (a[k] != b[k]) return 0;
    }
    return 1;
}

// Initializes the Analog-to-Digital Converter (ADC) module.

void init_adc() {
    ADCON0bits.CHS = 0b1100; // Select Analog Channel AN12
    ADCON0bits.ADON = 1; // Turn on ADC
    ADCON1 = 0x00; // Configure all AN pins as analog inputs
    // Set ADC result to be right-justified, acquisition time to 12 TAD, and ADC clock to Fosc/64
    ADCON2 = 0b10100110;
    ADRESH = 0x00; // Clear the ADC Result High Register
    ADRESL = 0x00; // Clear the ADC Result Low Register
}

void init_ports() {
    TRISB = 0x10; // RB4 for input.
    TRISH = 0x10; // RH4 for input
    TRISJ = 0x00; // PORTJ is output

}

void init_others() {
    INTCONbits.PEIE = 1; // Enable Peripheral Interrupts
    INTCONbits.GIE = 1; // Enable Global Interrupts
    INTCONbits.TMR0IE = 1; // Enable Timer0 Overflow Interrupt
    INTCONbits.RBIE = 1; // Enable PORTB Change Interrupt
    time_counter = 0; // Initialize the global time counter to 0
    T0CON = 0b10001000; // TMR0ON = 1, PSA = 1, others 0
    TMR0H = 0x3C; // High byte of Timer0
    TMR0L = 0xAF;  // Low byte of Timer0

    TXSTA1 = 0x04; // BRGH=1, others 0
    RCSTA1 = 0x90; // SPEN = 1, CREN = 1 others = 0
    BAUDCON1bits.BRG16 = 1; // BRG16 = 1 (16-bit Baud Rate Generator)
    SPBRGH1 = 0;
    SPBRG1 = 86; // For 115200 bps with 40MHz Fosc and BRG16=1.
    // Baud Rate = Fosc / (4 * (SPBRGH:SPBRG + 1))
    // 115200 = 40,000,000 / (4 * (0x0086 + 1)) = 40,000,000 / (4 * 87) = 40,000,000 / 348 = 114942.5
    
    // Initialize parking lot state
    for (int k = 0; k < 4; k++) {
        park.floors[k].level = 'A' + k;
        park.floors[k].available_f = 10;
    }
    park.available = 40;
    q.front = 0;
    q.count = 0;
    PIE1bits.TX1IE = 1; // Enable EUSART Transmit Interrupt
    PIE1bits.RC1IE = 1; // Enable EUSART Receive Interrupt
    PIR1 = 0; // Clear all peripheral interrupt flags

}

// Processes characters from the input buffer to build full message.
// Message starts with $ and ends with #
void receive_message() {
    // Loop as long as there are characters in the input buffer
    while (!buf_isempty(INBUF)) {
        disable_rxtx();
        uint8_t c = buf_pop(INBUF); // Pop the next character from the input buffer
        enable_rxtx();
        if (c == '$') {
            in_packet = 1;  // Set flag to indicate we are inside a message packet
            msg_len = 0;    // Reset message length for the new packet
        } else if (c == '#' && in_packet) {
            buffer[msg_len] = '\0';
            eval_message();  // message done!
            in_packet = 0;  // Reset flag, we are not inside message packet
        } else if (in_packet && msg_len < sizeof(buffer) - 1) {
            buffer[msg_len++] = c;
        }
    }
}

// Evaluates and processes the received command stored in buffer.
void eval_message() {
    if (is_same(buffer, "GO", 2) ) {
        run_flag = 1;  // Set the run flag to enable parking logic
        T0CONbits.TMR0ON = 1; // Start Timer0 to begin time tracking and periodic tasks
    } else if (is_same(buffer, "END", 3) ) {
        run_flag = 0;    // Clear the run flag to halt simulation
    } else if (is_same(buffer, "PRK", 3) && run_flag) {
        // Ensure car ID digits are indeed numeric characters
        if(buffer[3] < '0' || buffer[3] > '9') return;
        if(buffer[4] < '0' || buffer[4] > '9') return;
        if(buffer[5] < '0' || buffer[5] > '9') return;
        // Parse the 3-digit car ID from buffer
        int id = (buffer[3] - '0') * 100 + (buffer[4] - '0') * 10 + (buffer[5] - '0');
        int tmp_slot;
        char tmp_lvl;
        // First, check if the car has a subscribed slot and if that slot is currently empty
        if(find_subscribed(&tmp_lvl, &tmp_slot, id)){
            park.floors[tmp_lvl - 'A'].slots[tmp_slot].entry_time = time_counter;
            park.floors[tmp_lvl - 'A'].slots[tmp_slot].occupied = 1;
            park.floors[tmp_lvl - 'A'].available_f--;
            park.available--;   
            tmp_slot++; // Increment slot, because it gives index. But we must give 1 more message(e.g. for 0 index we must give 01)
            buf_push('$', OUTBUF);
            buf_push('S', OUTBUF);
            buf_push('P', OUTBUF);
            buf_push('C', OUTBUF);
            buf_push((id / 100) % 10 + '0', OUTBUF);
            buf_push((id / 10) % 10 + '0', OUTBUF);
            buf_push((id % 10) + '0', OUTBUF);
            buf_push(tmp_lvl, OUTBUF);
            buf_push((tmp_slot / 10) + '0', OUTBUF);
            buf_push((tmp_slot % 10) + '0', OUTBUF);
            buf_push('#', OUTBUF);
            new_message_flag = 1;   // Set flag to indicate a response message needs to be sent
            return;
        }
        
        enqueue(id);     // If not a subscribed car, enqueue the car
        
        // If there are overall available slots and cars waiting in the queue
        if (park.available > 0 && q.count > 0) {       
            temp_id = dequeue();    // Get the next car from the queue
            if(find_park_available_slot(&tmp_lvl, &tmp_slot, temp_id) ){
                tmp_slot++; // Increment slot, because it gives index. But we must give 1 more message(e.g. for 0 index we must give 01)
                disable_rxtx();
                buf_push('$', OUTBUF);
                buf_push('S', OUTBUF);
                buf_push('P', OUTBUF);
                buf_push('C', OUTBUF);
                buf_push((temp_id / 100) % 10 + '0', OUTBUF);
                buf_push((temp_id / 10) % 10 + '0', OUTBUF);
                buf_push((temp_id % 10) + '0', OUTBUF);
                buf_push(tmp_lvl, OUTBUF);
                buf_push((tmp_slot / 10) + '0', OUTBUF);
                buf_push((tmp_slot % 10) + '0', OUTBUF);
                buf_push('#', OUTBUF);
                enable_rxtx();
                new_message_flag = 1;   // Set flag to indicate a response message needs to be sent
            }
            else enqueue(temp_id);
        }
        //A car requests to exit.
    } else if (is_same(buffer, "EXT", 3) && run_flag) {
        // Parse the 3-digit car ID from buffer
        int id = (buffer[3] - '0') * 100 + (buffer[4] - '0') * 10 + (buffer[5] - '0');
        // Ensure car ID digits are indeed numeric characters
        if(buffer[3] < '0' || buffer[3] > '9') return;
        if(buffer[4] < '0' || buffer[4] > '9') return;
        if(buffer[5] < '0' || buffer[5] > '9') return;
        int tmp_slot;
        char tmp_lvl;
        find_parked(id, &tmp_lvl, &tmp_slot);   // Find the slot where the car is parked
        // Check if the car was in a subscribed slot
        if (park.floors[tmp_lvl - 'A'].slots[tmp_slot].subscribe == 1) {
            park.floors[tmp_lvl - 'A'].slots[tmp_slot].entry_time = 0;
            park.floors[tmp_lvl - 'A'].slots[tmp_slot].occupied = 0;
            park.floors[tmp_lvl - 'A'].available_f++;
            park.available++;
            
            // Prepare and push "FEE" message with 0 TL for subscribed cars
            buf_push('$', OUTBUF);
            buf_push('F', OUTBUF);
            buf_push('E', OUTBUF);
            buf_push('E', OUTBUF);
            buf_push((id / 100) % 10 + '0', OUTBUF);
            buf_push((id / 10) % 10 + '0', OUTBUF);
            buf_push((id % 10) + '0', OUTBUF);
            buf_push('0', OUTBUF);
            buf_push('0', OUTBUF);
            buf_push('0', OUTBUF);
            buf_push('#', OUTBUF);
        } else {
            // Calculate parking fee for unsubscribed cars
            int fee = ( (time_counter - park.floors[tmp_lvl - 'A'].slots[tmp_slot].entry_time) / 250 ) + 1;
            total_score += fee;
            park.floors[tmp_lvl - 'A'].slots[tmp_slot].entry_time = 0;
            park.floors[tmp_lvl - 'A'].slots[tmp_slot].occupied = 0;
            park.floors[tmp_lvl - 'A'].available_f++;
            park.available++;
            // Prepare and push "FEE" message with the calculated fee
            buf_push('$', OUTBUF);
            buf_push('F', OUTBUF);
            buf_push('E', OUTBUF);
            buf_push('E', OUTBUF);
            buf_push((id / 100) % 10 + '0', OUTBUF);
            buf_push((id / 10) % 10 + '0', OUTBUF);
            buf_push((id % 10) + '0', OUTBUF);
            buf_push((fee / 100) % 10 + '0', OUTBUF);
            buf_push((fee / 10) % 10 + '0', OUTBUF);
            buf_push((fee % 10) + '0', OUTBUF);
            buf_push('#', OUTBUF);
            
            // If there are cars waiting in the queue, park the next one in the newly freed slot
            if(q.count > 0){
                temp_id = dequeue();
                park.floors[tmp_lvl - 'A'].slots[tmp_slot].id = temp_id;
                park.floors[tmp_lvl - 'A'].slots[tmp_slot].entry_time = time_counter;
                park.floors[tmp_lvl - 'A'].slots[tmp_slot].occupied = 1;
                park.floors[tmp_lvl - 'A'].available_f--;
                park.available--;
                tmp_slot++; // Increment slot, because it gives index. But we must give 1 more message(e.g. for 0 index we must give 01)
                buf_push('$', OUTBUF);
                buf_push('S', OUTBUF);
                buf_push('P', OUTBUF);
                buf_push('C', OUTBUF);
                buf_push((temp_id / 100) % 10 + '0', OUTBUF);
                buf_push((temp_id / 10) % 10 + '0', OUTBUF);
                buf_push((temp_id % 10) + '0', OUTBUF);
                buf_push(tmp_lvl, OUTBUF);
                buf_push((tmp_slot / 10) + '0', OUTBUF);
                buf_push((tmp_slot % 10) + '0', OUTBUF);
                buf_push('#', OUTBUF);
                new_message_flag = 1;   // Set flag to indicate a response message
            }
        }
        new_message_flag = 1;

    // A car requests to subscribe to a parking space.
    } else if (is_same(buffer, "SUB", 3) && run_flag) {
        // Parse car ID from the message
        int id = (buffer[3] - '0') * 100 + (buffer[4] - '0') * 10 + (buffer[5] - '0');
        if(buffer[3] < '0' || buffer[3] > '9') return;
        if(buffer[4] < '0' || buffer[4] > '9') return;
        if(buffer[5] < '0' || buffer[5] > '9') return;
        // Check for invalid level
        if(buffer[6] != 'A' && buffer[6] != 'B' && buffer[6] != 'C' && buffer[6] != 'D') return;
        // Check for invalid slot
        if(buffer[7] != '0' && buffer[7] != '1') return;
        if(buffer[8] < '0' || buffer[8] > '9') return;
        // Parse level(A,B..) from the message
        char lvl_no = buffer[6];
        // Parse number(01,02..) from the message
        int slot_no = (buffer[7] - '0') * 10 + (buffer[8] - '0');
        slot_no--;
        // Prepare RES message
        buf_push('$', OUTBUF);
        buf_push('R', OUTBUF);
        buf_push('E', OUTBUF);
        buf_push('S', OUTBUF);
        buf_push((id / 100) % 10 + '0', OUTBUF);
        buf_push((id / 10) % 10 + '0', OUTBUF);
        buf_push((id % 10) + '0', OUTBUF);
        // Check if requested slot is available for subscription
        if (park.floors[lvl_no - 'A'].slots[slot_no].subscribe == 0 && park.floors[lvl_no - 'A'].slots[slot_no].entry_time == 0 && park.floors[lvl_no - 'A'].slots[slot_no].occupied == 0) {
            park.floors[lvl_no - 'A'].slots[slot_no].subscribe = 1;
            park.floors[lvl_no - 'A'].slots[slot_no].id = id;
            buf_push('5', OUTBUF);
            buf_push('0', OUTBUF);
            total_score += 50;
        } else {
            buf_push('0', OUTBUF);
            buf_push('0', OUTBUF);
        }
        buf_push('#', OUTBUF);
        new_message_flag = 1;   // Set flag to indicate a response message
    }
}

// Manages sending messages from the output buffer.
// If `new_message_flag` is set, it enables transmitter. Otherwise it sends an EMP message.
void send_message() {
    // If a new message is ready to be sent
    if (new_message_flag) {
        TXSTA1bits.TXEN = 1;
        new_message_flag = 0;
    } else{
        // If no specific command response is pending, send EMP message.
        buf_push('$', OUTBUF);
        buf_push('E', OUTBUF);
        buf_push('M', OUTBUF);
        buf_push('P', OUTBUF);
        // Append the total available parking spaces as two ASCII digits
        buf_push( (park.available / 10) % 10 + '0' , OUTBUF);
        buf_push( (park.available % 10) + '0', OUTBUF);
        buf_push('#', OUTBUF);
        TXSTA1bits.TXEN = 1; // Enable UART Transmit to send this message
    }
}

// Reads the Analog-to-Digital Converter (ADC) value and determines the current parking level (A, B, C, or D).
void check_level() {
    ADCON0bits.GO = 1;
    while(ADCON0bits.GO);
    // Combine the 8-bit ADRESL and 2-bit ADRESH
    unsigned int adc_val = (ADRESH << 8) + ADRESL;
    // Map the ADC value to the corresponding parking level
    if (adc_val < 256) current_level = 0;
    else if (adc_val < 512) current_level = 1;
    else if (adc_val < 768) current_level = 2;
    else current_level = 3;
}



// ============================ //
//   INTERRUPT SERVICE ROUTINE  //
// ============================ //

__interrupt(high_priority)
void HandleInterrupt() {
    // Timer0 Overflow Interrupt Handler
    if (INTCONbits.T0IF) {
        INTCONbits.T0IF = 0; // Clear the Timer0 overflow interrupt flag
        TMR0H = 0x3C; // Reload the high byte of Timer0
        TMR0L = 0xAF; // Reload the low byte of Timer0
        if (run_flag) tick_5 = 1;
    }
    
    // PORTB Change Interrupt Handler
    // This interrupt fires when there's a change on any of the enabled PORTB pins.
    // Specifically used here for the RB4 button press/release to toggle display mode.
    if(INTCONbits.RBIF && INTCONbits.RBIE){ // Check if the PORTB change interrupt flag
        INTCONbits.RBIF = 0;    // Clear the PORTB change interrupt flag
        curr_rb4 = PORTBbits.RB4;
        switch_flag = 1;
    }
    
    // UART Receive Interrupt Handler
    // This interrupt fires when a new byte is received by the EUSART.
    if (PIR1bits.RC1IF) {   // Check if the EUSART Receive Interrupt Flag is set
        disable_i();
        uint8_t c = RCREG1;  // Read the received character from the RCREG1 buffer.
        buf_push(c, INBUF);  // no parsing here! Push the received character into the input buffer.
        enable_i();
        if(c == '#') message_flag = 1;  // If the received character is '#', it indicates the end of a message packet
    }
    
    // UART Transmit Interrupt Handler
    // This interrupt fires when the EUSART Transmit Shift Register is empty.
    if(PIR1bits.TX1IF){
        if (buf_isempty(OUTBUF)) {  // If the output buffer is empty, there are no more bytes to send

            if (TXSTA1bits.TRMT) {  // Check if the Transmit Shift Register (TSR) is also empty
                TXSTA1bits.TXEN = 0;    // If everything is sent, disable the UART transmitter
            }
        } else {
            // If there are bytes in the output buffer, load the next byte into the Transmit Register
            disable_i();
            TXREG1 = buf_pop(OUTBUF);
            enable_i();
        }
        
    }
}

// ============================ //
//            MAIN              //
// ============================ //

// Main function of the parking lot management system. Initializes all necessary
// peripherals and enters an infinite loop
// It continuously monitor and manage the parking lot based on commands from the simulator
void main() {
    init_ports();
    init_adc();
    init_others();
    while (1) {
        if(message_flag){ // If message received, It call receive_message()
            message_flag = 0;
            receive_message();
        }
        if(send_message_flag){ // If send_message_flag is set, it calls send_message()
            send_message_flag = 0;
            send_message();
        }
        if(tick_5){
            tick_5 = 0;
            level_counter++;    // It increases in every 5ms.
            time_counter += 5; // Global time calculator. It increases in every 5ms.
            message_counter++; // It increases in every 5ms.
            if (level_counter == 100) { // In every 500ms it calls check_level()
                level_counter = 0;
                check_level();
            }
            if (message_counter == 20) { // In every 100ms it sets send_message_flag.
                message_counter = 0;
                send_message_flag = 1;
            }
            if(switch_flag){ // If switch_flag is set, it toggles display_mode.
                switch_flag = 0;
                if(prev_rb4 == 1 && curr_rb4 == 0){display_mode = !display_mode;} // Check the state of RB4.
                // Toggle the `display_mode` between 0 (total score) and 1 (available slots)
                prev_rb4 = curr_rb4;
            }
            update_display(); // In every 5ms it calls update_display()
        }
    }
}