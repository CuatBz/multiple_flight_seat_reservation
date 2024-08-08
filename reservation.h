#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ID_LEN 4
#define NAME_LEN 20
#define SEAT_COUNT 12
#define PLANES 4

struct Seat {
    char id[ID_LEN];
    __uint8_t seat_status;
    char fname [NAME_LEN];
    char lname [NAME_LEN];
};

void main_menu (struct Seat Plane[][SEAT_COUNT]);
