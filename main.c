#include <sys/stat.h>               //helps me check if file is created for the first time
#include <sys/types.h>              //helps store file size with off_t
#include "reservation.h"

//Function to find out file size
off_t fsize(const char *filename) {
    struct stat st; 

    if (stat(filename, &st) == 0)
        return st.st_size;

    return -1; 
}


//A reservation program for plane seats
int main (void)
{
    FILE * save_file;                           //save file pointer
    int size = sizeof(struct Seat);             //size of a Seat struct
    const char save_name[] = "flight.bin";      //save file name

    //Initialize struct array in case file does not exist yet
    const struct Seat Template [SEAT_COUNT] = {
        {"1LA", 0, "", ""},
        {"1LW", 0, "", ""},
        {"1RA", 0, "", ""},
        {"1RW", 0, "", ""},
        {"2LA", 0, "", ""},
        {"2LW", 0, "", ""},
        {"2RA", 0, "", ""},
        {"2RW", 0, "", ""},
        {"3LA", 0, "", ""},
        {"3LW", 0, "", ""},
        {"3RA", 0, "", ""},
        {"3RW", 0, "", ""}
    };

    struct Seat Plane [PLANES][SEAT_COUNT];

    //Load data from save file
    if ((save_file = fopen(save_name, "a+b")) == NULL)
    {
        printf("Can't open %s. Data cannot be read from file.\n", save_name);
    }

    else
    {
        off_t is_new_file = fsize(save_name);       //will affect behaviour if file is 0

        int structs_read = 0;                       //makes sure all structs are read
        if (is_new_file > 0)                        //do this if file already had data
        {
            rewind(save_file);

            //Load saved file contents
            for (int i = 0; i < PLANES; i++)
            {
                for (int j = 0; j < SEAT_COUNT; j++)
                {
                    if (fread(&Plane[i][j], size, 1, save_file) == 1)
                        structs_read++;
                }
            }
            
            if (structs_read == 48)
                printf("Data loaded successfully!\n\n");
        }

        if (is_new_file <= 0 || structs_read < 48)                                        //do this if the file is new
        {
            //Fills in struct arrays with default data
            for (int i = 0; i < PLANES; i++)
            {
                for (int j = 0; j < SEAT_COUNT; j++)
                {
                    Plane[i][j] = Template[j];
                }
                
            }
        }

        fclose(save_file);
    }

    //All the functions run from this point
    main_menu(Plane);

    FILE * temp_file;
    const char temp_name[] = "temp.bin";

    //Write data to temporary file
    if ((temp_file = fopen(temp_name, "wb")) == NULL)
    {
        printf("Can't open %s. Data cannot be stored to file.\n", temp_name);
    }

    else
    {
        int structs_read = 0;
        for (int i = 0; i < PLANES; i++)
        {
            for (int j = 0; j < SEAT_COUNT; j++)
            {
                if (fwrite(&Plane[i][j], size, 1, temp_file) == 1)
                    structs_read++;
            }
        }
        
        if (structs_read == 48)
            printf("\nData saved successfully!\n");

        fclose(temp_file);
    }

    //Replace save file with temp file
    remove(save_name);
    rename(temp_name, save_name);

    return 0;
}