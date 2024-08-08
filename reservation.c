#include <ctype.h>
#include "reservation.h"

static char * s_gets(char * st, int n);
static void clear_input (void);
static void print_main_menu(void);
static void print_sub_menu(char flight_name[]);
static void sub_menu(struct Seat Plane [][SEAT_COUNT], int flight_index, char flight_name[]);
static void init_ptrs (struct Seat ** arr_ptr, struct Seat Plane[], int seats);
static int count_empty_seats (struct Seat Plane[][SEAT_COUNT], int num, int seats);
static void show_empty_seats_num (struct Seat Plane[][SEAT_COUNT], int num, int seats);
static void alpha_sort_seats (struct Seat Plane[][SEAT_COUNT], int num, int seats);
static void assign_seat (struct Seat Plane[][SEAT_COUNT], int num, int seats);
static void confirm_seat (struct Seat Plane[][SEAT_COUNT], int num, int seats);
static void unassign_seat (struct Seat Plane[][SEAT_COUNT], int num, int seats);

static struct Seat * arr_ptr[SEAT_COUNT];      //used for sorting the struct

const char * _status [3] = {"Unassigned", "Assigned", "Confirmed"};       //String form of seat status

static void show_empty_seats_num (struct Seat Plane[][SEAT_COUNT], int num, int seats)
{
    printf("\nCurrently there are %d empty seats.\n", count_empty_seats(Plane, num, seats));
}

//Shows the empty seats on the plane
static void show_empty_seats (struct Seat Plane[][SEAT_COUNT], int num, int seats)
{
    int count = 0;
    for (int i = 0; i < seats; i++)
    {
        if (Plane[num][i].seat_status == 0)
        {
            if (count == 0)
                printf("\nCurrently the available seats are:\n\n");

            count++;
            printf("Seat #%d: %s\n", count, Plane[num][i].id);
        }   
    }

    if (count == 0)
        printf("\nWe're sorry, we have no seats availabe currently.\n");
}

//Sorts seats alphabetically
static void alpha_sort_seats (struct Seat Plane[][SEAT_COUNT], int num, int seats)
{
    init_ptrs(arr_ptr, Plane[num], SEAT_COUNT);  //iniatilizes the pointers for the struct pointer array

    char temp[ID_LEN];

    //Sorting algorithm
    for (int top = 0; top < seats - 1; top++)
    {
        for (int seek = top + 1; seek < seats; seek++)
        {
            if (strcmp(arr_ptr[top]->id, arr_ptr[seek]->id) > 0)
            {
                strcpy(temp, arr_ptr[top]->id);
                strcpy(arr_ptr[top]->id, arr_ptr[seek]->id);
                strcpy(arr_ptr[seek]->id, temp);
            }
        }
    }
    
    printf("\nThe seats in alphabetical order:\n\n");
    for (int j = 0; j < seats; j++)
        printf("Seat #%2d: %s | %10s | %s %s\n", j + 1, arr_ptr[j]->id, _status[arr_ptr[j]->seat_status], arr_ptr[j]->fname, arr_ptr[j]->lname);
}

//Assigns a client to a seat
static void assign_seat (struct Seat Plane[][SEAT_COUNT], int num, int seats)
{
    int count = 0;
    
    for (int i = 0; i < seats; i++)
    {
        if (Plane[num][i].seat_status == 0)
        {
            if (count == 0)
                printf("\nAvailable seats:\n");

            count++;
            printf("Seat #%d: %s\n", count, Plane[num][i].id);
        }
    }
    
    if (count > 0)
    {
        int choice = 0;
        printf("\nPlease enter the number to choose the seat you want:\n");
        while (scanf("%d", &choice) != 1 || choice < 1 || choice > count)
        {
            clear_input();
            printf("Please enter a valid seat number:\n");
        }
        clear_input();

        count = 0;
        int index;
        for (int k = 0; k < seats; k++)
        {
            if (Plane[num][k].seat_status == 0)
                count++;
                
            if (count == choice)
            {
                Plane[num][k].seat_status = 1;
                index = k;
                break;
            }
        }

        printf("\nYou will be assigned seat %s\n", Plane[num][index].id);

        printf("\nPlease enter your first name:\n");
        s_gets(Plane[num][index].fname, NAME_LEN);
        while (Plane[num][index].fname[0] == '\0')
        {
            printf("Please enter a valid name:\n");
            s_gets(Plane[num][index].fname, NAME_LEN);
        }

        printf("\nPlease enter your last name:\n");
        s_gets(Plane[num][index].lname, NAME_LEN);
        while (Plane[num][index].lname[0] == '\0')
        {
            printf("Please enter a valid name:\n");
            s_gets(Plane[num][index].lname, NAME_LEN);
        }

        printf("\n%s %s you have been assigned seat %s\n", Plane[num][index].fname, Plane[num][index].lname, Plane[num][index].id);
    }

    else
    {
        printf("\nWe're sorry, we have no seats available right now.\n");
    }    
}

static void confirm_seat (struct Seat Plane[][SEAT_COUNT], int num, int seats)
{
    int count = 0;

    for (int i = 0; i < seats; i++)
    {
        if (Plane[num][i].seat_status == 1)
        {
            if (count == 0)
                printf("\nCurrent assigned seats:\n");

            count++;
            printf("Seat #%d: %s | %s %s\n", count, Plane[num][i].id, Plane[num][i].fname, Plane[num][i].lname);
        }
    }

    if (count > 0)
    {
        int choice = 0;

        printf("\nPlease enter the number to choose the seat you want to confirm:\n");
        while (scanf(" %d", &choice) != 1 || choice < 1 || choice > count)
        {
            clear_input();
            printf("Please enter a valid seat number:\n");
        }
        clear_input();

        count = 0;
        int index;
        for (int k = 0; k < seats; k++)
        {
            if (Plane[num][k].seat_status == 1)
                count++;
                
            if (count == choice)
            {
                Plane[num][k].seat_status = 2;
                index = k;
                break;
            }
        }

        printf("\n%s %s has been confirmed for seat %s\n", Plane[num][index].fname, Plane[num][index].lname, Plane[num][index].id);
    }
    
    else
    {
        printf("\nNo seats have been assigned yet.\n");
    }
}

//Removes seat_status status from a seat
static void unassign_seat (struct Seat Plane[][SEAT_COUNT], int num, int seats)
{
    int count = 0;

    for (int i = 0; i < seats; i++)
    {
        if (Plane[num][i].seat_status == 1 || Plane[num][i].seat_status == 2)
        {
            if (count == 0)
            {
                printf("\nAssigned seats:\n");
            }

            count++;
            printf("Seat #%2d: %s | %10s | %s %s\n", count, Plane[num][i].id, _status[Plane[num][i].seat_status], Plane[num][i].fname, Plane[num][i].lname);
        }
    }

    if (count > 0)
    {
        int choice = 0;

        printf("\nPlease enter the number to choose the seat you want to unassign:\n");
        while (scanf(" %d", &choice) != 1 || choice < 1 || choice > count)
        {
            clear_input();
            printf("Please enter a valid seat number:\n");
        }
        clear_input();

        count = 0;
        int index;
        for (int k = 0; k < seats; k++)
        {
            if (Plane[num][k].seat_status == 1 || Plane[num][k].seat_status == 2)
                count++;
                
            if (count == choice)
            {
                Plane[num][k].seat_status = 0;
                index = k;
                break;
            }
        }

        printf("\n%s %s has been unassigned from seat %s\n", Plane[num][index].fname, Plane[num][index].lname, Plane[num][index].id);
        Plane[num][index].fname[0] = '\0';
        Plane[num][index].lname[0] = '\0';
    }
    
    else
    {
        printf("\nNo seats have been assigned yet.\n");
    }
}

// initializes the pointers into the struct pointers array
static void init_ptrs (struct Seat ** arr_ptr, struct Seat Plane[], int seats)
{
    for (int i = 0; i < seats; i++)
        arr_ptr[i] = &Plane[i];
}

//Returns the amount of empty seats in the flight
static int count_empty_seats (struct Seat Plane[][SEAT_COUNT], int num, int seats)
{
    int count = 0;

    for (int i = 0; i < seats; i++)
    {
        if (Plane[num][i].seat_status == 0)
            count++;
    }
    
    return count;
}

//Handles all the main menu functions
void main_menu (struct Seat Plane[][SEAT_COUNT])
{
    char opt;

    print_main_menu();
    scanf(" %c", &opt);
    opt = tolower(opt);
    while (opt != 'e')
    {
        switch (opt)
        {
        case 'a':
            sub_menu(Plane, 0, "102");
            break;
        
        case 'b':
            sub_menu(Plane, 1, "311");
            break;

        case 'c':
            sub_menu(Plane, 2, "444");
            break;

        case 'd':
            sub_menu(Plane, 3, "519");
            break;

        case 'e':
            printf("\nThank you for using our program. Have a good day!\n");
            break;

        default:
            printf("\nInvalid option. Please try again.\n");
            break;
        }

        printf("\n");
        print_main_menu();
        scanf(" %c", &opt);
        opt = tolower(opt);
    }
}

//Handles sub-menu
static void sub_menu(struct Seat Plane [][SEAT_COUNT], int flight_index, char flight_name[])
{
    char opt;

    printf("\n");
    print_sub_menu(flight_name);
    scanf(" %c", &opt);
    opt = tolower(opt);
    while (opt != 'g')
    {
        switch (opt)
        {
        case 'a':
            show_empty_seats_num(Plane, flight_index, SEAT_COUNT);            //shows number of empty seats
            break;
        
        case 'b':
            show_empty_seats(Plane, flight_index, SEAT_COUNT);                //shows id of empty seats
            break;

        case 'c':
            alpha_sort_seats(Plane, flight_index, SEAT_COUNT);                //sorts seats alphabetically
            break;

        case 'd':
            assign_seat(Plane, flight_index, SEAT_COUNT);                     //assigns a person to a seat
            break;

        case 'e':
            confirm_seat(Plane, flight_index, SEAT_COUNT);                   //unassigns a person from a seat
            break;

        case 'f':
            unassign_seat(Plane, flight_index, SEAT_COUNT);                   //unassigns a person from a seat
            break;

        case 'g':
            printf("\nGoing back to main menu.\n");
            break;

        default:
            printf("\nInvalid option. Please try again.\n");
            break;
        }

        printf("\n");
        print_sub_menu(flight_name);
        scanf(" %c", &opt);
        opt = tolower(opt);
    }   
}

//Main menu
static void print_main_menu(void)
{
    printf("Colossus Airlines | Main Menu\n");
    printf("a) Flight 102\n");
    printf("b) Flight 311\n");
    printf("c) Flight 444\n");
    printf("d) Flight 519\n");
    printf("e) Exit\n");
    printf("Option: ");
}

//Sub-menu
static void print_sub_menu(char flight_name[])
{
    printf("Colossus Airlines | Flight %s\n", flight_name);
    printf("a) Show number of empty seats\n");
    printf("b) Show list of empty seats\n");
    printf("c) Show alphabetical list of seats\n");
    printf("d) Assign a customer to a seat\n");
    printf("e) Confirm an assigned seat\n");
    printf("f) Remove a seat assignment\n");
    printf("g) Exit to main menu\n");
    printf("Option: ");
}

//Clears input buffer
static void clear_input (void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        continue;
}

//Replaces f_gets. Does not store '\n'
static char * s_gets(char * st, int n)
{
    char * ret_val;
    char * find;

    ret_val = fgets(st, n, stdin);

    if (ret_val)
    {
        find = strchr(st, '\n'); // look for newline

        if (find)       // if the address is not NULL,
            *find = '\0';       // place a null character there

        else
            while (getchar() != '\n')
                continue;               // dispose of rest of line
    }
    
    return ret_val;
}