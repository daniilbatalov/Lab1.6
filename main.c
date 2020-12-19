#define false 0
#define true 1

typedef int bool;
typedef struct flight flight;
typedef struct time time;
typedef struct date date;

#define ON_SCREEN 1
#define IN_FILE 0

#define LOWER_DATE 1
#define LOWER_TIME 0
#define LOWER_NO_VALID -32767
#define LOWER_MENU 1
#define LOWER_FLIGHTS 1

#define UPPER_DAY 31
#define UPPER_MONTH 12
#define UPPER_YEAR 32767
#define UPPER_HOUR 24
#define UPPER_MINUTE 60
#define UPPER_NO_VALID 32767
#define UPPER_MENU 6
#define UPPER_ENTER_MENU 3
#define UPPER_PRINT_MENU 4
#define UPPER_SORT_MENU 6

#define STRING_LIMIT 256

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <malloc.h>
#include <math.h>

struct date
{
    int day;
    int month;
    int year;
};
struct time
{
    int hour;
    int minute;
};
struct flight
{
    date departure_date;
    time dep_time;
    time travel_time;
    bool breakfast;
    char airport_dest[STRING_LIMIT];
};

void exept (const char* error_message, const int code);
flight* alloc_array_f (const size_t arr_size);
flight* append_array_f (flight* arr, const size_t by_size);
bool breakfast_check (const time tt);
flight flight_constructor (char* a, const time tt, const time dt, const date dd);
flight flight_constructor_p (const int d, const int m, const int y, const int dh, const int dm, const int th, const int tm, char* a);
int get_int(const char* message, const int lower, const int upper);
void change_order (int *order);
void change_way (int *way);
date get_date();
time get_time(const bool val);
void print_main_menu();
void print_enter_menu();
void print_print_menu();
void print_sort_menu(const int order);
bool validate (const int value, const int upper, const int lower);
void enter_flight(flight* fl, const int n);
int import_flight(FILE *file, flight* fl, const int n);
void print_line();
void print_str_line();
void print_flight(const flight fl, const int i, const int mode, FILE* out);
void print_flights (const flight* flights, const int amount, const int mode, FILE* out);
void print_pure_flight(const flight fl, FILE* out);
void print_pure_flights (const flight* flights, const int amount, FILE* out);
void print_header();
void print_empty_table();
void copy_date (date* to, const date* from);
void copy_time (time* to, const time* from);
void copy_flight (flight* to, const flight* from);
int signum(const int x);
int compare_date_time (const flight* first, const flight* second);
int compare_time (const flight* first, const flight* second);
int compare_by_airport (const flight* first, const flight* second);
void delete_flight (flight* fl, int* amount, const int to_delete);
void swap_flights (flight* fl, const int to_swap1, const int to_swap2);
void sort_b (flight* fl, const int amount, const int order, int (*cmp)(const flight*, const flight*));

int main()
{
    int pick;
    int number, amount = 0;
    int to_del;
    int order = 1;
    int way = 1;
    flight* flights = NULL;
    FILE *input;
    FILE *output;
    do
    {
        system("cls");
        int print_pick, enter_pick, sort_pick;
        print_main_menu();
        pick = get_int("", LOWER_MENU, UPPER_MENU);
        switch (pick)
        {
        case 1:
            do
            {
                system("cls");
                print_enter_menu();
                enter_pick = get_int("", LOWER_MENU, UPPER_PRINT_MENU);
                switch (enter_pick)
                {
                case 1:
                    if (!amount)
                    {
                        number = get_int("How many flights are there?\n>", LOWER_FLIGHTS, UPPER_NO_VALID);
                    }
                    else
                    {
                        number = get_int("How many flights would you like to add?\n>", LOWER_FLIGHTS, UPPER_NO_VALID);
                    }
                    amount += number;
                    if (!flights)
                    {
                        flights = alloc_array_f(number);
                    }
                    else
                    {
                        flights = append_array_f(flights, number);
                    }
                    for(int i = amount - number; i < amount; i++)
                    {
                        enter_flight(flights, i);
                    }
                    enter_pick = UPPER_ENTER_MENU;
                    break;
                case 2:
                    if((input = fopen("input.txt", "r")) == NULL)
                    {
                        exept("Error: cannot open file.\n", 1);
                    }
                    ;
                    fscanf(input, "%i", &number);
                    amount += number;
                    if (!flights)
                    {
                        flights = alloc_array_f(number);
                    }
                    else
                    {
                        flights = append_array_f(flights, number);
                    }
                    for(int i = amount - number; i < amount; i++)
                    {
                        if (!import_flight(input, flights, i))
                        {
                            exept("Error: incorrect data!", 1);
                        }
                    }
                    fclose(input);
                    enter_pick = UPPER_ENTER_MENU;
                    break;
                }
            }
            while (enter_pick != UPPER_ENTER_MENU);
            break;
        case 2:
            if (!amount)
            {
                puts("Warning! Nothing to delete");
                getchar();
            }
            else
            {
                to_del = get_int("Which flight do you want to delete?\n>", 1, amount);
                delete_flight(flights, &amount, to_del - 1);
            }
            break;
        case 3:
            do
            {
                system("cls");
                print_sort_menu(order);
                sort_pick = get_int("", LOWER_MENU, UPPER_SORT_MENU);
                switch (sort_pick)
                {
                case 1:
                    sort_b(flights, amount, order, compare_date_time);
                    sort_pick = UPPER_SORT_MENU;
                    break;
                case 2:
                case 3:
                    sort_b(flights, amount, order, compare_time);
                    sort_pick = UPPER_SORT_MENU;
                    break;
                case 4:
                    sort_b(flights, amount, order, compare_by_airport);
                    sort_pick = UPPER_SORT_MENU;
                    break;
                case 5:
                    change_order(&order);
                    break;
                }
            }
            while (sort_pick != UPPER_SORT_MENU);
            break;
        case 4:
            do
            {
                system("cls");
                print_print_menu(way);
                print_pick = get_int("", LOWER_MENU, UPPER_PRINT_MENU);
                switch (print_pick)
                {
                case 1:
                    print_flights(flights, amount, ON_SCREEN, stdout);
                    getchar();
                    print_pick = UPPER_PRINT_MENU;
                    break;
                case 2:

                    switch (way)
                    {
                    case 1:
                        if((output = fopen("output.txt", "w")) == NULL)
                        {
                            exept("Error: cannot open file.\n", 1);
                        }
                        print_pure_flights(flights, amount, output);
                        fclose(output);
                        break;
                    case 0:
                        if((output = fopen("input.txt", "w")) == NULL)
                        {
                            exept("Error: cannot open file.\n", 1);
                        }
                        print_pure_flights(flights, amount, output);
                        fclose(output);
                        break;
                    case -1:
                        if((output = fopen("output.txt", "w")) == NULL)
                        {
                            exept("Error: cannot open file.\n", 1);
                        }
                        print_flights(flights, amount, IN_FILE, output);
                        fclose(output);
                        break;
                    }
                    print_pick = UPPER_PRINT_MENU;
                    break;
                case 3:
                    change_way(&way);
                    break;
                }
            }
            while (print_pick != UPPER_PRINT_MENU);
            break;
        case 5:
            if (!amount)
            {
                puts("Warning! Nothing to delete");
                getchar();
            }
            else
            {
                puts("Warning! This will delete every flight");
                getchar();
                while (amount != 0)
                {
                    delete_flight(flights, &amount, 0);
                }
                flights = NULL;
            }
            break;
        }

    }
    while (pick != UPPER_MENU);
    free(flights);
    flights = NULL;
    return 0;
}
void exept (const char* error_message, const int code)
{
    printf(error_message);
    exit(code);
}

flight* alloc_array_f (const size_t arr_size)
{
    return (flight*)malloc(arr_size * sizeof(flight));
}
flight* append_array_f (flight* arr, const size_t by_size)
{
    size_t prev_size = _msize(arr);
    size_t new_size = prev_size + (sizeof(flight) * by_size);
    return (flight*)realloc(arr, new_size);
}
bool breakfast_check (const time tt)
{
    return ((tt.hour > 4 || (tt.hour == 4 && tt.minute >= 1)) ? true : false);
}

flight flight_constructor (char* a, const time tt, const time dt, const date dd)
{
    flight result;
    result.departure_date = dd;
    result.breakfast = breakfast_check(tt);
    result.dep_time = dt;
    result.travel_time = tt;
    strcpy(result.airport_dest, a);
    return result;
};
flight flight_constructor_p (const int d, const int m, const int y, const int dh, const int dm, const int th, const int tm, char* a)
{
    date dd = {d, m, y};
    time dt = {dh, dm};
    time tt = {th, tm};
    return flight_constructor(a, tt, dt, dd);
};
int get_int(const char* message, const int lower, const int upper)
{
    char str[STRING_LIMIT];
    int x;
    printf(message);
    fgets(str, sizeof(str), stdin);
    while (sscanf (str, "%d", &x) != 1 || x < lower || x > upper)
    {
        printf("Enter correct value!\n>");
        fgets(str, sizeof(str), stdin);
    }
    return x;
}
void change_order (int *order)
{
     (*order == 1) ? (*order = -1) : (*order = 1);
}
void change_way (int *way)
{
     (*way == 1) ? (*way = 0) : ((*way == 0) ? (*way = -1) : (*way = 1));
}
date get_date()
{
    date dt;
    dt.day = get_int("Enter the day of the flight:\n>", LOWER_DATE, UPPER_DAY);
    dt.month = get_int("Enter the month of the flight:\n>", LOWER_DATE, UPPER_MONTH);
    dt.year = get_int("Enter the year of the flight:\n>", LOWER_DATE, UPPER_YEAR);
    return dt;
}
time get_time(const bool val)
{
    time t;
    if(val)
    {
        t.hour = get_int("Enter the hour of departure:\n>", LOWER_TIME, UPPER_HOUR) % UPPER_HOUR;
        t.minute = get_int("Enter the minute of departure:\n>", LOWER_TIME, UPPER_MINUTE) % UPPER_MINUTE;
    }
    else
    {
        t.hour = get_int("How many hours the flight will be?\n>", LOWER_TIME, UPPER_NO_VALID);
        t.minute = get_int("How many minutes the flight will be?\n>", LOWER_TIME, UPPER_MINUTE);
    }
    return t;
}

void print_main_menu()
{
    printf("What do you want to do?\n");
    printf("1. Enter the flights\n");
    printf("2. Delete the flight\n");
    printf("3. Sort the flights\n");
    printf("4. Print the flights\n");
    printf("5. Clear all the flights\n");
    printf("6. Exit\n");
    printf(">");
}
void print_enter_menu()
{
    printf("How do you want to enter the flights?\n");
    printf("1. Enter the flights manually\n");
    printf("2. Import the filghts from input.txt\n");
    printf("3. Back\n");
    printf(">");
}
void print_print_menu(const int way)
{
    printf("How do you want to print the flights?\n");
    printf("1. Print the flights on screen\n");
    printf("2. Export the filghts into %s.txt\n", ((way == 0) ? "input" : "output"));
    printf("3. Change the way of export: %s\n", ((way == 1) ? "pure" : (way == 0 ? "save" : "as table")));
    printf("4. Back\n");
    printf(">");
}
void print_sort_menu(const int order)
{
    printf("How do you want to sort the flights?\n");
    printf("1. Sort by date and time of departure\n");
    printf("2. Sort by travel time\n");
    printf("3. Sort by availability of breakfast\n");
    printf("4. Sort by airport of destination\n");
    printf("5. Change the sort order: %s\n", ((order == 1) ? "ascending" : "descending"));
    printf("6. Back\n");
    printf(">");
}
bool validate (const int value, const int upper, const int lower)
{
    return ((value <= upper) && (value >= lower)) ? true : false;
}
void enter_flight(flight* fl, const int n)
{

    print_str_line();
    printf("Flight #%d\n", n + 1);
    date dd = get_date();
    time dt = get_time(true);
    time tt = get_time(false);
    printf("What's the airport of destination?\n>");
    char s[STRING_LIMIT];
    gets(s);
    fl[n] = flight_constructor(s, tt, dt, dd);
}
int import_flight(FILE *file, flight* fl, const int n)
{
    int d, m, y, hd, md, th, tm = 0;
    char air [STRING_LIMIT];
    fscanf(file, "%2i.%2i.%4i %2i:%2i  %2i:%2i %[^\n]", &d, &m, &y, &hd, &md, &th, &tm, air);
    int result = 1;
    result = validate(d, UPPER_DAY, LOWER_DATE) * validate(m, UPPER_MONTH, LOWER_DATE) * validate(y, UPPER_YEAR, LOWER_DATE);
    result = result * validate(hd, UPPER_HOUR, LOWER_TIME) * validate(md, UPPER_MINUTE, LOWER_TIME) * validate(th, UPPER_NO_VALID, LOWER_TIME) * validate(tm, UPPER_MINUTE, LOWER_TIME);
    if(result)
    {
        fl[n] = flight_constructor_p(d, m, y, hd, md, th, tm, air);
    }
    return result;
}


FILE* choose_stream (const int mode, FILE* out)
{
    return ((mode == 1) ? stdout : out);
}
void print_line(const int mode, FILE* out)
{
    fprintf(choose_stream(mode, out) ,"+----+------------+--------+-------------+-----------+---------------+\n");
}
void print_str_line(const int mode, FILE* out)
{
    fprintf(choose_stream(mode, out), "----------------------------\n");
}
void print_empty_line(const int mode, FILE* out)
{
    fprintf(choose_stream(mode, out), "+--------------------------------------------------------------------+\n");
}
void print_header(const int mode, FILE* out)
{
    print_line(mode, out);
    fprintf(choose_stream(mode, out), "| No |    Date    |  Time  | Travel time | Breakfast |    Airport    |\n");
}
void print_empty_table(const int mode, FILE* out)
{
    print_empty_line(mode, out);
    fprintf(choose_stream(mode, out), "|                     There's no flights yet...                      |\n");
    print_empty_line(mode, out);
}
void print_flight(const flight fl, const int i, const int mode, FILE* out)
{
    char* b = fl.breakfast ? "+" : "-";
    print_line(mode, out);
    fprintf(choose_stream(mode, out), "| %2i | %02d.%02d.%04d | %02d:%02d  |    %02d:%02d    |     %s     | %13s |\n", i, fl.departure_date.day, fl.departure_date.month, fl.departure_date.year, fl.dep_time.hour, fl.dep_time.minute, fl.travel_time.hour, fl.travel_time.minute, b, fl.airport_dest);
}
void print_flights (const flight* flights, const int amount, const int mode, FILE* out)
{
    print_header(mode, out);
    if (amount == 0)
    {
        print_empty_table(mode, out);
    }
    else
    {
        for(int i = 0; i < amount; i++)
        {
            print_flight(flights[i], i + 1, mode, out);
        }
        print_line(mode, out);
    }
}
void print_pure_flight(const flight fl, FILE* out)
{
    fprintf(out, "%02d.%02d.%04d %02d:%02d %02d:%02d %s\n", fl.departure_date.day, fl.departure_date.month, fl.departure_date.year, fl.dep_time.hour, fl.dep_time.minute, fl.travel_time.hour, fl.travel_time.minute, fl.airport_dest);
}
void print_pure_flights (const flight* flights, const int amount, FILE* out)
{
        fprintf(out, "%i\n", amount);
        for(int i = 0; i < amount; i++)
        {
            print_pure_flight(flights[i], out);
        }
}
void copy_date (date* to, const date* from)
{
    to->day = from->day;
    to->month = from->month;
    to->year = from->year;
}
void copy_time (time* to, const time* from)
{
    to->hour = from->hour;
    to->minute = from->minute;
}
void copy_flight (flight* to, const flight* from)
{
    strcpy(to->airport_dest, from->airport_dest);
    to->breakfast = from->breakfast;
    copy_date(&to->departure_date, &from->departure_date);
    copy_time(&to->dep_time, &from->dep_time);
    copy_time(&to->travel_time, &from->travel_time);
}
int signum(const int x)
{
    return (!x) ? 0 : (x / abs(x));
}
int compare_date_time (const flight* first, const flight* second)
{
    int result = 0;
    int arr[4];
    arr[0] = signum(first->departure_date.year - second->departure_date.year);
    arr[1] = signum(first->departure_date.month - second->departure_date.month);
    arr[2] = signum(first->departure_date.day - second->departure_date.day);
    arr[3] = compare_time(first, second);
    result = signum(1000 * arr[0] + 100 * arr[1] + 10 * arr[2] + arr[3]);
    return result;
}
int compare_time (const flight* first, const flight* second)
{
    int result = 0;
    int arr[2];
    arr[0] = signum(first->travel_time.hour - second->travel_time.hour);
    arr[1] = signum(first->travel_time.minute - second->travel_time.minute);
    result = signum(10 * arr[0] + arr[1]);
    return result;
}

int compare_by_airport (const flight* first, const flight* second)
{
    return strcmp(first->airport_dest, second->airport_dest);
}
void delete_flight (flight* fl, int* amount, const int to_delete)
{
    for(int i = to_delete; i < *amount - 1; i++)
    {
        copy_flight(&fl[i], &fl[i + 1]);
    }
    append_array_f(fl, -1);
    (*amount)--;
}
void swap_flights (flight* fl, const int to_swap1, const int to_swap2)
{
    flight temp;
    copy_flight(&temp, &fl[to_swap1]);
    copy_flight(&fl[to_swap1], &fl[to_swap2]);
    copy_flight(&fl[to_swap2], &temp);
}
void sort_b (flight* fl, const int amount, const int order, int (*cmp)(const flight*, const flight*))
{
    for(int i = 0 ; i < amount - 1; i++)
    {
        for(int j = 0 ; j < amount - i - 1 ; j++)
        {
            if(cmp(&fl[j], &fl[j + 1]) == order)
            {
                //swap_flights(fl, j, j + 1);
                flight tmp = fl[j];
                fl[j] = fl[j+1];
                fl[j+1] = tmp;
            }
        }
    }
}
