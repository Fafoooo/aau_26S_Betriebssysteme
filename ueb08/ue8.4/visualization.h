#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#define ROAD_LENGTH 20
#define BUS_LENGTH 4
#define STATION_CAPACITY 20

#define M 9  				// Total bus capacity
#define N 26 				// Total passengers
#define X 4  				// Seats available

// for checking if the bus is here
#define BUS_AT_A (position == 0)
#define BUS_AT_B (position == ROAD_LENGTH-1)

// Simulation times
#define TRAVEL_TIME 200000
#define STOP_TIME 1000000

enum Status
{
	AT_A,
	AT_B,
	BUS_STANDING,
	BUS_SEATED,
	DONE
};

typedef struct
{
    char id; // ID is used to display passengers -> recommend using A-Z
    enum Status status;
} Passenger;


// Initializes the ncurses screen
void init_visualization();

// Closes the ncurses screen
void close_visualization();

// Displays a list of finished passengers
void print_finished_passengers(Passenger *passengers);

// Move the bus and update positions
void simulate_bus_movement(Passenger *passengers);

// Visual output of the bus, stops, and passengers
void print_bus_position(Passenger *passengers);

// check if all passengers arrived
int check_finish(Passenger *passengers);

// function headers for TODO function
void bus_arrived_in_station();
int passenger_sleep();

// Bus control
extern int position;
extern int direction;
extern int done;

#endif
