#include "visualization.h"
#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

// Passengers for visual illustration
char passengers_at_a1[11] = "          ";  
char passengers_at_b1[11] = "          ";  
char passengers_at_a2[11] = "          ";  
char passengers_at_b2[11] = "          "; 
char road_l1[ROAD_LENGTH + BUS_LENGTH + 5 + 1]; 
char road_l2[ROAD_LENGTH + BUS_LENGTH + 5 + 1]; 

// station labels
const char stationA[] = "[Station A ]";
const char stationB[] = "[ Station B]";

int done = 0;
int position = 0;
int direction = 1;

// Initialize ncurses
void init_visualization() 
{
    initscr();            // Start curses mode
    cbreak();             // Disable line buffering
    noecho();             // Do not echo pressed keys
    curs_set(FALSE);      // Hide the cursor
    nodelay(stdscr, TRUE);// Non-blocking input if needed

}

// End ncurses
void close_visualization() 
{
    endwin();
}

void simulate_bus_movement(Passenger *passengers) 
{
    while (!done) 
	{
        position += direction;
		print_bus_position(passengers);
		print_finished_passengers(passengers);
		
		refresh();
		
        if (BUS_AT_A || BUS_AT_B) 
		{
			// Indicate the bus is stopped for boarding
						
			bus_arrived_in_station();
            
			usleep(STOP_TIME);
			
			// turn around
			
			direction = -direction;
            position += direction;
        }
		else
		{
			usleep(TRAVEL_TIME);
		}
    }
}

int passenger_sleep() 
{ 
	float sleeptime = rand() % 8 + 3;
	sleep(sleeptime); 
	return 0; 
}

int check_finish(Passenger *passengers)
{
    for (int i = 0; i < N; i++) 
	{
		if (passengers[i].status != DONE)
			return 0;
	}
	return 1;
}

void update_road_and_stations(Passenger *passengers) 
{
    // Clear previous road and station passenger data
    memset(road_l1, ' ', ROAD_LENGTH + BUS_LENGTH + 4);
    memset(road_l2, ' ', ROAD_LENGTH + BUS_LENGTH + 4);
    memset(passengers_at_a1, ' ', 10);
    memset(passengers_at_b1, ' ', 10);
    memset(passengers_at_a2, ' ', 10);
    memset(passengers_at_b2, ' ', 10);

    // Set road boundaries
    road_l1[position] = '|'; // Start of the bus
    road_l1[position + BUS_LENGTH + 4] = '|'; // End of the bus
    road_l2[position] = '|'; // Same for lower part of the bus
    road_l2[position + BUS_LENGTH + 4] = '|';
	
    // Fill in the passengers in the bus: seated on lower, standing on upper
    int free_lower = 0;
    int free_upper = 0;
    for (int i = 0; i < N; i++) 
	{
		if (passengers[i].status == BUS_SEATED) 
		{
			road_l2[position + 1 + free_lower++] = passengers[i].id; // Seated passengers on the lower level
		} 
		else if (passengers[i].status == BUS_STANDING) 
		{
			road_l1[position + 1 + free_upper++] = passengers[i].id; // Standing passengers on the upper level
		}
	}

    // Fill in the passengers at stations
    int a_count1 = 0, a_count2 = 0, b_count1 = 0, b_count2 = 0;
    for (int i = 0; i < N; i++) 
	{
        if (passengers[i].status == AT_A) 
		{
            if (a_count1 < STATION_CAPACITY/2)
                passengers_at_a1[a_count1++] = passengers[i].id;
            else if (a_count2 < STATION_CAPACITY/2)
                passengers_at_a2[a_count2++] = passengers[i].id;
        } 
		else if (passengers[i].status == AT_B) 
		{
            if (b_count1 < STATION_CAPACITY/2)
                passengers_at_b1[b_count1++] = passengers[i].id;
            else if (b_count2 < STATION_CAPACITY/2)
                passengers_at_b2[b_count2++] = passengers[i].id;
        }
    }
}

void print_finished_passengers(Passenger *passengers) 
{
    mvprintw(4, 0, "Finished passengers:");
    for (int i = 0; i < N; i++) 
	{
        if (passengers[i].status == DONE) 
		{
            printw("%c ", passengers[i].id);
        }
    }

}

// Clear the screen and print the bus and station information
void print_bus_position(Passenger *passengers) 
{
    clear();

    // Print the top line with station labels
    mvprintw(0, 0, "%s", stationA);

    // Draw the road with the bus at the current position
    for (int i = 0; i < ROAD_LENGTH; i++) 
	{
        if (i == position) {
            // Draw the bus 
            printw("[  Bus  ]");
        } else {
            printw("-");
        }
    }
    printw("%s\n", stationB);
    update_road_and_stations(passengers);
	
    // Print the passengers below with markers for bus and stations
    printw("|%s|%s|%s|\n", passengers_at_a1, road_l1, passengers_at_b1);  // Passenger line 1
    printw("|%s|%s|%s|\n", passengers_at_a2, road_l2, passengers_at_b2);  // Passenger line 2
	
}

