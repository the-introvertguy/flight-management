#include "header.h"

// struct flight structure definition
typedef struct flight
{
    bool isInc;
    int flightNum;
    char city[cityLen + 1];
    char schedTime[TimeStrLen];
    char status[satutsLen];
    int delayTime;
    char delayReason[reasonLen];
    int priority;
    int numOfSeat;
    int numOfPassenger;
    int terminal;
    char actualTime[TimeStrLen];
    struct flight *next;
    struct flight *prev;
} flight;


// Global variables
flight *outgoingFront = NULL;
flight *outgoingBack = NULL;
flight *incomingFront = NULL;
flight *incomingBack = NULL;
flight *historyFront = NULL;
flight *historyBack = NULL;


char lastTime[6] = "00:00";
int lastFlightNum = 1000;
int lastIncTerm = 1;
int lastOutTerm = 1;

// Function prototypes
void addFlight();
void processFlight();
void cancelFlight();
void displayAllFlights();
void displayFlightStatus(int flightNum);
void showStatistics();
void allocateSeats();
void delayFlight();
void saveFlightsToFile();
void loadFlightsFromFile();
void addToHistory(flight *f);
void saveHistoryToFile();
void loadHistoryFromFile();
void displayHistory();
void rebookPassengers(flight *cancelledFlight);
void gotoxy(int x, int y);
void drawBox(int x, int y, int width, int height);
void thinbox(int x, int y, int width, int height);
void mainbox();
void clearInputBuffer();
int getValidNumber(const char *prompt, int min, int max);
int getseatNumber(const char *prompt, int min, int max);
char *prio(int n);
char *generateTime();
char *getCurrentTime();
int getNextTerminal(bool isInc);
int getUrgencyLevel();
void freeAllFlights();
void loadingAnimation();
void exitAnimation();

int main()
{

    loadFlightsFromFile();
    loadHistoryFromFile();
    loadingAnimation();

    int choice;
    while (true)
    {
        system("cls");
        mainbox();
        gotoxy(40, 10 - 2);
        printf(BHWHT "1. Add Flight" reset);
        gotoxy(40, 11 - 2);
        printf("2. Display All Flights");
        gotoxy(40, 12 - 2);
        printf("3. Depart/Land Flight");
        gotoxy(40, 13 - 2);
        printf("4. Seat Allocation");
        gotoxy(40, 14 - 2);
        printf("5. Delay Flight");
        gotoxy(40, 15 - 2);
        printf("6. Cancel Flight");
        gotoxy(40, 16 - 2);
        printf("7. Check Flight Status");
        gotoxy(40, 17 - 2);
        printf("8. View Statistics");
        gotoxy(40, 18 - 2);
        printf("9. Flight History");
        gotoxy(40, 19 - 2);
        printf("10. Exit");
        gotoxy(51, 21);
        printf("Enter your choice: ");
        thinbox(40, 22, 40, 3);
        gotoxy(43, 23);

        if (scanf("%d", &choice) != 1)
        {
            clearInputBuffer();
            gotoxy(42, 25);
            printf(RED "Invalid input. Please enter a number.\n" reset);
            gotoxy(45, 26);
            printf(YEL "Press any key to continue..." reset);
            getch();
            continue;
        }

        switch (choice)
        {
        case 1:
            addFlight();
            break;
        case 2:
            displayAllFlights();
            break;
        case 3:
            processFlight();
            break;
        case 4:
            allocateSeats();
            break;
        case 5:
            delayFlight();
            break;
        case 6:
            cancelFlight();
            break;
        case 7:
        {
            system("cls");
            mainbox();
            int flightNum = getValidNumber("Enter Flight Number: ", 1001, lastFlightNum);
            displayFlightStatus(flightNum);
            break;
        }
        case 8:
            showStatistics();
            break;
        case 9:
            displayHistory();
            break;
        case 10:
            freeAllFlights();
            exitAnimation();
            exit(0);
        default:
            printf("Invalid choice. Please try again.\n");
            printf("Press any key to continue...");
            getch();
            break;
        }
    }
    return 0;
}


// Function definitions
void addToHistory(flight *f)
{
    flight *newHistory = (flight *)malloc(sizeof(flight));
    if (!newHistory)
    {
        printf("Memory allocation error while adding to history.\n");
        return;
    }

    *newHistory = *f;
    newHistory->next = NULL;
    newHistory->prev = historyBack;

    if (historyBack)
    {
        historyBack->next = newHistory;
    }
    else
    {
        historyFront = newHistory;
    }
    historyBack = newHistory;

    saveHistoryToFile();
}
// Function to save flight history to a file
void saveHistoryToFile()
{
    FILE *file = fopen(historyFileDir, "wb");
    if (!file)
    {
        printf("Error opening history file for writing!\n");
        return;
    }

    flight *curr = historyFront;
    while (curr != NULL)
    {
        fwrite(curr, sizeof(flight), 1, file);
        curr = curr->next;
    }

    fclose(file);
}
// Function to load flight history from a file
void loadHistoryFromFile()
{
    FILE *file = fopen(historyFileDir, "rb");
    if (!file)
    {
        return;
    }

    flight *curr = historyFront;
    while (curr != NULL)
    {
        flight *temp = curr;
        curr = curr->next;
        free(temp);
    }
    historyFront = historyBack = NULL;

    flight temp;
    while (fread(&temp, sizeof(flight), 1, file) == 1)
    {
        flight *newFlight = (flight *)malloc(sizeof(flight));
        if (!newFlight)
        {
            printf("Memory allocation error while loading history.\n");
            fclose(file);
            return;
        }

        *newFlight = temp;
        newFlight->next = NULL;
        newFlight->prev = historyBack;

        if (historyBack)
        {
            historyBack->next = newFlight;
        }
        else
        {
            historyFront = newFlight;
        }
        historyBack = newFlight;
    }

    fclose(file);
}
// Function to display flight history
void displayHistory()
{
    system("cls");
    int y = 2;
    gotoxy(40, y);
    printf("=========== FLIGHT HISTORY ===========\n\n");

    flight *curr = historyFront;
    if (!curr)
    {
        gotoxy(40, y + 2);
        printf("\t\tNo flight history available.\n");
        y += 6;
    }
    else
    {
        gotoxy(10, y + 2);
        printf(BWHT "\t%-10s %-8s %-20s %-10s %-10s %-15s %-10s %-10s\n" reset,
               "Flight No", "Type", "City", "Scheduled", "Terminal", "Status", "Delay", "Actual Time");

        y += 4;
        while (curr)
        {
            gotoxy(10, y);
            printf("\t%9d  %-8s %-20s %-10s %-10d %-15s %-10d %-10s\n",
                   curr->flightNum,
                   curr->isInc ? "Incoming" : "Outgoing",
                   curr->city,
                   curr->schedTime,
                   curr->terminal,
                   curr->status,
                   curr->delayTime,
                   curr->actualTime);
            curr = curr->next;
            y++;
        }
        y += 2;
    }

    gotoxy(40, y);
    printf(YEL "Press any key to continue..." reset);
    getch();
}

// Function to rebook passengers in case of flight cancellation
void loadingAnimation()
{
    system("cls");

    mainbox();

    thinbox(40, 16, 40, 3);
    gotoxy(42, 15);
    printf(BHWHT "Password" reset);

    char passInput[32];
    char ch;
    int i = 0;
    gotoxy(43, 17);

    while (1)
    {
        ch = getch();
        if (ch == 13)
        {
            passInput[i] = '\0';
            break;
        }
        else if (ch == 8)
        {
            if (i > 0)
            {
                i--;
                printf("\b \b");
            }
        }
        else
        {
            passInput[i] = ch;
            i++;
            printf("*");
        }
    }

    while (strcmp(passInput, setProgPassword) != 0)
    {
        gotoxy(40, 20);
        printf(RED "Wrong password. Try again!" reset);
        gotoxy(43, 17);
        printf("                    ");
        gotoxy(43, 17);

        i = 0;
        while (1)
        {
            ch = getch();
            if (ch == 13)
            {
                passInput[i] = '\0';
                break;
            }
            else if (ch == 8)
            {
                if (i > 0)
                {
                    i--;
                    printf("\b \b");
                }
            }
            else
            {
                passInput[i] = ch;
                i++;
                printf("*");
            }
        }
    }

    system("cls");
    drawBox(30, 10, 60, 10);
    gotoxy(45, 12);
    printf(BHCYN "AIRPORT FLIGHT MANAGEMENT SYSTEM" reset);
    gotoxy(50, 14);
    printf("Initializing System...");

    for (int i = 1; i <= 55; i++)
    {
        gotoxy(32 + i, 16);

        if (i < 10)
            printf(WHTB " " reset);
        else if (i < 20)
            printf(WHTB " " reset);
        else if (i < 30)
            printf(WHTB " " reset);
        else if (i < 40)
            printf(WHTB " " reset);
        else if (i < 50)
            printf(WHTB " " reset);
        else
            printf(WHTB " " reset);

        gotoxy(60, 18);
        printf("%d%%", (i * 100) / 55);

        Sleep(30 + rand() % 50);
    }

    gotoxy(58, 20);
    printf(BGRN "Ready!" reset);
    Sleep(500);
}

// Function to exit the program with an animation
void exitAnimation()
{
    system("cls");

    system("cls");
    gotoxy(40, 12);
    printf(BHCYN "FLIGHT MANAGEMENT SYSTEM" reset);
    gotoxy(38, 14);
    printf("  Session ended successfully");
    gotoxy(37, 16);
    printf("All data saved. Program exiting...");

    for (int i = 3; i > -1; i--)
    {
        gotoxy(50, 18);
        printf(BWHT "%d" reset, i);
        Sleep(1000);
    }
    printf("\n\n");
}

// Function to process flight (depart/land)
void processFlight()
{
    system("cls");
    mainbox();
    char type;
    do
    {
        gotoxy(40, 7);
        printf("Enter Flight Type 'I'/'O': ");
        thinbox(38, 8, 45, 3);
        gotoxy(41, 9);
        scanf(" %c", &type);
        type = toupper(type);
        clearInputBuffer();
        if (type != 'I' && type != 'O')
        {
            gotoxy(41, 11);
            printf("Invalid flight type. Please enter 'I' or 'O'.\n");
        }
    } while (type != 'I' && type != 'O');

    bool isInc = (type == 'I');
    flight *temp;

    if (isInc)
    {
        if (incomingFront)
        {
            temp = incomingFront;
            incomingFront = incomingFront->next;
            strcpy(temp->status, "Landed");
            strcpy(temp->actualTime, getCurrentTime());

            gotoxy(35, 11 + 5);
            printf("Flight %d Landed Successfully from %s\n",
                   temp->flightNum, temp->city);
            gotoxy(35, 12 + 5);
            printf("Terminal: %d\n", temp->terminal);
            gotoxy(35, 13 + 5);
            printf("Actual Arrival Time: %s\n", temp->actualTime);

            addToHistory(temp);

            if (incomingFront)
            {
                incomingFront->prev = NULL;
            }
            else
            {
                incomingBack = NULL;
            }
            free(temp);
        }
        else
        {
            gotoxy(35, 15);
            printf("No Flight To Land!\n");
        }
    }
    else
    {
        if (outgoingFront)
        {
            temp = outgoingFront;
            outgoingFront = outgoingFront->next;
            strcpy(temp->status, "Departed");
            strcpy(temp->actualTime, getCurrentTime());

            gotoxy(35, 11 + 5);
            printf("Flight No: %d Departed Successfully for %s", temp->flightNum,
                   temp->city);
            gotoxy(35, 12 + 5);
            printf("Terminal: %d", temp->terminal);
            gotoxy(35, 13 + 5);
            printf("Actual Departure Time: %s\n", temp->actualTime);

            addToHistory(temp);

            if (outgoingFront)
            {
                outgoingFront->prev = NULL;
            }
            else
            {
                outgoingBack = NULL;
            }
            free(temp);
        }
        else
        {
            gotoxy(35, 15);
            printf("No Flight To Take Off!\n");
        }
    }
    saveFlightsToFile();
    gotoxy(40, 24);
    printf(YEL "Press any key to continue..." reset);
    getch();
}

// Function to cancel a flight
void cancelFlight()
{
    system("cls");
    mainbox();
    int flightNum = getValidNumber("Enter Flight no. to cancel: ", 1001, lastFlightNum);

    flight *curr = outgoingFront;
    flight *prev = NULL;

    while (curr != NULL && curr->flightNum != flightNum)
    {
        prev = curr;
        curr = curr->next;
    }

    if (curr == NULL)
    {
        curr = incomingFront;
        prev = NULL;

        while (curr != NULL && curr->flightNum != flightNum)
        {
            prev = curr;
            curr = curr->next;
        }
    }

    if (curr != NULL)
    {
        gotoxy(52, 8);
        printf(BWHT "Flight %d Details" reset, curr->flightNum);
        gotoxy(48, 10);
        printf("Type: %s\n", curr->isInc ? "Incoming" : "Outgoing");
        gotoxy(48, 11);
        printf("%s: %s\n", curr->isInc ? "Origin" : "Destination", curr->city);
        gotoxy(48, 12);
        printf("Status: %s\n", curr->status);

        char confirm;
        gotoxy(33, 15);
        printf(BMAG "Are you sure you want to cancel this flight? (Y/N): " reset);
        scanf(" %c", &confirm);
        confirm = toupper(confirm);
        clearInputBuffer();

        if (confirm == 'Y')
        {
            strcpy(curr->status, "Cancelled");
            strcpy(curr->actualTime, getCurrentTime());

            addToHistory(curr);

            if (prev == NULL)
            {
                if (curr->isInc)
                {
                    incomingFront = curr->next;
                    if (incomingFront == NULL)
                        incomingBack = NULL;
                }
                else
                {
                    outgoingFront = curr->next;
                    if (outgoingFront == NULL)
                        outgoingBack = NULL;
                }
            }
            else
            {
                prev->next = curr->next;
                if (curr->next != NULL)
                {
                    curr->next->prev = prev;
                }
                else
                {
                    if (curr->isInc)
                        incomingBack = prev;
                    else
                        outgoingBack = prev;
                }
            }

            if (!curr->isInc && curr->numOfPassenger > 0)
            {
                rebookPassengers(curr);
            }

            free(curr);
            gotoxy(48, 17);
            printf("Flight %d has been cancelled.", flightNum);
        }
        else
        {
            gotoxy(48, 17);
            printf("Cancellation aborted.");
        }
    }
    else
    {
        gotoxy(48, 15);
        printf("Flight %d not found!", flightNum);
    }
    saveFlightsToFile();
    gotoxy(48, 18);
    printf(YEL "Press any key to continue..." reset);
    getch();
}

/// display all flights in the system
void displayAllFlights()
{
    system("cls");
    int y = 2;
    gotoxy(40, y);
    printf("=========== INCOMING FLIGHTS ===========\n\n");
    flight *curr = incomingFront;
    if (!curr)
    {
        gotoxy(40, y + 2);
        printf("\t\tNo incoming flights scheduled.\n");
        y += 6;
    }
    else
    {
        gotoxy(10, y + 2);
        printf(BWHT "\t%-10s %-20s %-10s %-10s %-15s %-10s\n" reset, "Flight No", "Origin",
               "Time", "Terminal", "Status", "Delay");
        y += 4;
        while (curr)
        {
            gotoxy(10, y);
            printf("\t%9d  %-20s %-10s %-10d %-15s %-10d\n", curr->flightNum,
                   curr->city, curr->schedTime, curr->terminal, curr->status,
                   curr->delayTime);
            curr = curr->next;
            y++;
        }
        y += 2;
    }
    gotoxy(40, y);

    printf("=========== OUTGOING FLIGHTS ===========\n\n");
    curr = outgoingFront;
    if (!curr)
    {
        gotoxy(40, y + 2);
        printf("\t\tNo outgoing flights scheduled.\n");
        y += 6;
    }
    else
    {
        gotoxy(10, y + 2);
        printf(BWHT "\t%-10s %-20s %-10s %-10s %-15s %-10s %-10s %-10s\n" reset, "Flight No",
               "Destination", "Time", "Terminal", "Status", "Priority", "Delay",
               "Seats");
        y += 4;
        while (curr)
        {
            gotoxy(10, y);
            printf("\t%9d  %-20s %-10s %-10d %-15s %-10s %-10d %d/%d\n",
                   curr->flightNum, curr->city, curr->schedTime, curr->terminal,
                   curr->status, prio(curr->priority), curr->delayTime,
                   curr->numOfPassenger, curr->numOfSeat);
            curr = curr->next;
            y++;
        }
    }

    gotoxy(40, y + 2);
    printf(YEL "Press any key to continue..." reset);
    getch();
}
// Function to save flight data to a file
void saveFlightsToFile()
{
    FILE *file = fopen(dataFileDir, "wb");
    if (!file)
    {
        printf("Error opening file for writing!\n");
        return;
    }

    fwrite(&lastFlightNum, sizeof(int), 1, file);
    fwrite(&lastIncTerm, sizeof(int), 1, file);
    fwrite(&lastOutTerm, sizeof(int), 1, file);
    fwrite(lastTime, sizeof(char), 6, file);

    flight *curr = incomingFront;
    while (curr != NULL)
    {
        fwrite(curr, sizeof(flight), 1, file);
        curr = curr->next;
    }

    curr = outgoingFront;
    while (curr != NULL)
    {
        fwrite(curr, sizeof(flight), 1, file);
        curr = curr->next;
    }

    fclose(file);
}

// Function to load flight data from a file
void loadFlightsFromFile()
{
    FILE *file = fopen(dataFileDir, "rb");
    if (!file)
    {
        printf("No existing flight data found. Starting fresh.\n");
        return;
    }

    freeAllFlights();
    incomingFront = incomingBack = NULL;
    outgoingFront = outgoingBack = NULL;

    fread(&lastFlightNum, sizeof(int), 1, file);
    fread(&lastIncTerm, sizeof(int), 1, file);
    fread(&lastOutTerm, sizeof(int), 1, file);
    fread(lastTime, sizeof(char), 6, file);

    flight temp;
    while (fread(&temp, sizeof(flight), 1, file) == 1)
    {
        flight *newFlight = (flight *)malloc(sizeof(flight));
        if (!newFlight)
        {
            printf("Memory allocation error while loading data.\n");
            fclose(file);
            return;
        }

        *newFlight = temp;
        newFlight->next = NULL;
        newFlight->prev = NULL;

        if (newFlight->isInc)
        {
            newFlight->prev = incomingBack;
            if (incomingBack)
            {
                incomingBack->next = newFlight;
            }
            else
            {
                incomingFront = newFlight;
            }
            incomingBack = newFlight;
        }
        else
        {

            flight *curr = outgoingFront;
            flight *prev = NULL;

            while (curr && curr->priority >= newFlight->priority)
            {
                prev = curr;
                curr = curr->next;
            }

            if (!prev)
            {
                newFlight->next = outgoingFront;
                if (outgoingFront)
                {
                    outgoingFront->prev = newFlight;
                }
                else
                {
                    outgoingBack = newFlight;
                }
                outgoingFront = newFlight;
            }
            else
            {
                newFlight->next = curr;
                newFlight->prev = prev;
                prev->next = newFlight;
                if (curr)
                {
                    curr->prev = newFlight;
                }
                else
                {
                    outgoingBack = newFlight;
                }
            }
        }
    }

    fclose(file);
}

// Function to free all flight nodes
void freeAllFlights()
{
    flight *curr = incomingFront;
    while (curr)
    {
        flight *temp = curr;
        curr = curr->next;
        free(temp);
    }

    curr = outgoingFront;
    while (curr)
    {
        flight *temp = curr;
        curr = curr->next;
        free(temp);
    }

    curr = historyFront;
    while (curr)
    {
        flight *temp = curr;
        curr = curr->next;
        free(temp);
    }
}
// Function to set the cursor position in the console
void gotoxy(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void drawBox(int x, int y, int width, int height)
{
    gotoxy(x, y);
    printf("%c", 201);

    for (int i = 0; i < width - 2; i++)
        printf("%c", 205);

    printf("%c", 187);

    for (int i = 0; i < height - 2; i++)
    {
        gotoxy(x, y + i + 1);
        printf("%c", 186);
        gotoxy(x + width - 1, y + i + 1);
        printf("%c", 186);
    }

    gotoxy(x, y + height - 1);
    printf("%c", 200);

    for (int i = 0; i < width - 2; i++)
        printf("%c", 205);

    printf("%c", 188);
}

// Function to draw a thin box
void thinbox(int x, int y, int width, int height)
{
    gotoxy(x, y);
    printf("%c", 218);

    for (int i = 0; i < width - 2; i++)
        printf("%c", 196);

    printf("%c", 191);

    for (int i = 0; i < height - 2; i++)
    {
        gotoxy(x, y + i + 1);
        printf("%c", 179);
        gotoxy(x + width - 1, y + i + 1);
        printf("%c", 179);
    }

    gotoxy(x, y + height - 1);
    printf("%c", 192);

    for (int i = 0; i < width - 2; i++)
        printf("%c", 196);

    printf("%c", 217);
}

// Function to draw the main box
void mainbox()
{

    drawBox(10, 2, 100, 26);
    gotoxy(45, 3);
    printf(BHCYN "AIRPORT FLIGHT MANAGEMENT SYSTEM" reset);
    gotoxy(45, 4);
    printf("================================");
}
// Function to clear the input buffer
void clearInputBuffer()
{
    while (getchar() != '\n')
        ;
}
// Function to get valid seat number from user
int getseatNumber(const char *prompt, int min, int max)
{
    int value;
    while (1)
    {
        gotoxy(43, 16);
        printf("%s", prompt);
        gotoxy(43, 18);
        if (scanf("%d", &value) == 1 && value >= min && value <= max)
        {
            clearInputBuffer();
            return value;
        }
        gotoxy(33, 23);
        printf("Invalid input. Please enter a number between %d and %d.", min, max);
        clearInputBuffer();
    }
}

// function to get priority level for outgoing flights
char *prio(int n)
{
    switch (n)
    {
    case 7:
        return "MED";
    case 6:
        return "HUM";
    case 5:
        return "MILI";
    case 4:
        return "DIP";
    case 3:
        return "VIP";
    case 2:
        return "WEA";
    case 1:
        return "REG";
    }
}

// function to get valid number from user
int getValidNumber(const char *prompt, int min, int max)
{
    int value;
    while (1)
    {
        gotoxy(47, 21);
        printf("%s", prompt);
        thinbox(40, 22, 40, 3);
        gotoxy(47, 23);
        if (scanf("%d", &value) == 1 && value >= min && value <= max)
        {
            clearInputBuffer();
            return value;
        }
        gotoxy(33, 25);
        printf(RED "Invalid input. Please enter a number between %d and %d." reset, min, max);
        clearInputBuffer();
    }
}

// function to add a flight
void addFlight()
{
    system("cls");

    char type;
    do
    {
        mainbox();
        gotoxy(40, 11);
        printf("Enter Flight Type: ");
        gotoxy(52, 14);
        printf(BHWHT "'I'" BLK " for Incoming " reset);
        gotoxy(52, 15);
        printf(BHWHT "'O'" BLK " for Outgoing " reset);

        gotoxy(51, 20);
        printf(WHT "Enter your choice: " reset);
        thinbox(40, 21, 40, 3);
        gotoxy(44, 22);
        scanf(" %c", &type);
        type = toupper(type);
        clearInputBuffer();

        if (type != 'I' && type != 'O')
        {
            system("cls");
            mainbox();
            gotoxy(38, 15);
            printf(RED "Invalid flight type. Please enter 'I' or 'O'.\n" reset);
            getch();
            system("cls");
        }
    } while (type != 'I' && type != 'O');

    bool isInc = (type == 'I');

    flight *newFlight = (flight *)malloc(sizeof(flight));
    if (!newFlight)
    {
        system("cls");
        mainbox();
        gotoxy(43, 15);
        printf("Memory allocation failed.\n");
        getch();
        system("cls");
        return;
    }

    newFlight->next = NULL;
    newFlight->prev = NULL;
    newFlight->flightNum = ++lastFlightNum;
    strcpy(newFlight->schedTime, generateTime());
    newFlight->delayTime = 0;
    newFlight->isInc = isInc;
    strcpy(newFlight->status, "Scheduled");
    strcpy(newFlight->delayReason, "None");
    newFlight->terminal = getNextTerminal(isInc);

    system("cls");
    mainbox();
    thinbox(40, 10, 40, 3);
    gotoxy(43, 9);
    printf("Enter %s City: ", isInc ? "Origin" : "Destination");
    gotoxy(44, 11);
    fgets(newFlight->city, sizeof(newFlight->city), stdin);
    newFlight->city[strcspn(newFlight->city, "\n")] = '\0';

    if (!isInc)
    {
        thinbox(40, 17, 40, 3);
        newFlight->numOfSeat = getseatNumber("Enter Number Of Seats: ", 1, 500);
        newFlight->priority = 8 - getUrgencyLevel();
        newFlight->numOfPassenger = 0;
    }
    else
    {
        newFlight->priority = 0;
        newFlight->numOfSeat = 0;
        newFlight->numOfPassenger = 0;
    }

    if (isInc)
    {

        newFlight->prev = incomingBack;
        if (incomingBack)
        {
            incomingBack->next = newFlight;
        }
        else
        {
            incomingFront = newFlight;
        }
        incomingBack = newFlight;
    }
    else
    {

        flight *curr = outgoingFront;
        flight *prev = NULL;

        while (curr && curr->priority >= newFlight->priority)
        {
            prev = curr;
            curr = curr->next;
        }

        if (!prev)
        {
            newFlight->next = outgoingFront;
            if (outgoingFront)
            {
                outgoingFront->prev = newFlight;
            }
            else
            {
                outgoingBack = newFlight;
            }
            outgoingFront = newFlight;
        }
        else
        {
            newFlight->next = curr;
            newFlight->prev = prev;
            prev->next = newFlight;
            if (curr)
            {
                curr->prev = newFlight;
            }
            else
            {
                outgoingBack = newFlight;
            }
        }
        curr = newFlight->next;
        while (curr)
        {
            curr->delayTime += 30;
            curr = curr->next;
        }
    }

    system("cls");
    mainbox();
    gotoxy(45, 10);
    printf("Flight %d added successfully.", newFlight->flightNum);
    gotoxy(45, 11);
    printf("Status: %s", newFlight->status);
    gotoxy(45, 12);
    printf("Terminal: %d", newFlight->terminal);
    gotoxy(45, 13);
    printf("Scheduled Time: %s", newFlight->schedTime);
    saveFlightsToFile();

    gotoxy(40, 20);
    printf(YEL "Press any key to continue..." reset);
    getch();
}

// Function to allocate seats for outgoing flights
void allocateSeats()
{
    system("cls");
    mainbox();
    int flightNum = getValidNumber("Enter Flight Number: ", 1001, lastFlightNum);
    flight *temp = outgoingFront;
    while (temp != NULL && temp->flightNum != flightNum)
    {
        temp = temp->next;
    }

    if (temp != NULL)
    {
        system("cls");
        mainbox();
        gotoxy(35, 10);
        printf("Current Status: %s\n", temp->status);
        gotoxy(35, 11);
        printf("Seats Booked: %d/%d\n", temp->numOfPassenger, temp->numOfSeat);

        int seatNum = getValidNumber("Enter Number of Passengers: ", 0, temp->numOfSeat - temp->numOfPassenger);

        temp->numOfPassenger += seatNum;
        gotoxy(35, 13);
        printf("Allocated seats successfully for %d passengers!", seatNum);

        if (temp->numOfPassenger == temp->numOfSeat)
        {
            strcpy(temp->status, "Fully Booked");
            gotoxy(35, 15);
            printf("Flight is now fully booked.");
        }
    }
    else
    {
        gotoxy(38, 12);
        printf("Invalid Flight Number or not an outgoing flight!\n");
    }
    saveFlightsToFile();
    gotoxy(45, 17);
    printf(YEL "Press any key to continue..." reset);
    getch();
}

// Function to delay a flight
void delayFlight()
{
    system("cls");
    mainbox();
    int flightNum = getValidNumber("Enter Flight Number to delay: ", 1001, lastFlightNum);

    flight *curr = outgoingFront;
    while (curr != NULL && curr->flightNum != flightNum)
    {
        curr = curr->next;
    }

    if (curr == NULL)
    {
        curr = incomingFront;
        while (curr != NULL && curr->flightNum != flightNum)
        {
            curr = curr->next;
        }
    }

    if (curr != NULL)
    {

        system("cls");
        mainbox();
        int delayMinutes = getValidNumber("Enter delay time in minutes: ", 0, 240);
        gotoxy(50, 7);
        printf("Enter reason for delay: ");
        thinbox(40, 8, 40, 3);
        gotoxy(43, 9);
        fgets(curr->delayReason, sizeof(curr->delayReason), stdin);
        curr->delayReason[strcspn(curr->delayReason, "\n")] = '\0';

        curr->delayTime += delayMinutes;
        strcpy(curr->status, "Delayed");

        if (!curr->isInc)
        {
            flight *nextFlight = curr->next;
            while (nextFlight != NULL)
            {
                nextFlight->delayTime += delayMinutes;
                if (strcmp(nextFlight->status, "Scheduled") == 0)
                {
                    strcpy(nextFlight->status, "Delayed");
                }
                nextFlight = nextFlight->next;
            }
        }
        gotoxy(40, 14);
        printf("Flight %d delayed by %d minutes.", flightNum, delayMinutes);
        gotoxy(40, 15);
        printf("Reason of Delaying: %s", curr->delayReason);
    }
    else
    {
        gotoxy(40, 15);
        printf("Flight %d not found!\n", flightNum);
    }
    saveFlightsToFile();
    gotoxy(40, 17);
    printf(YEL "Press any key to continue..." reset);
    getch();
}

// Function to display flight status
void displayFlightStatus(int flightNum)
{
    flight *curr = outgoingFront;
    while (curr != NULL && curr->flightNum != flightNum)
    {
        curr = curr->next;
    }

    if (curr == NULL)
    {
        curr = incomingFront;
        while (curr != NULL && curr->flightNum != flightNum)
        {
            curr = curr->next;
        }
    }

    system("cls");
    if (curr != NULL)
    {
        int y = 6;
        gotoxy(40, 3);
        printf("====== Flight " BWHT "%d" reset " Status ======", flightNum);
        gotoxy(20, y++);
        printf("Type\t\t: \t%s\n", curr->isInc ? "Incoming" : "Outgoing");
        gotoxy(20, y++);

        printf("%s \t" BWHT "%s\n" reset, curr->isInc ? "Origin\t\t:" : "Destination\t\t:", curr->city);
        gotoxy(20, y++);

        printf("Scheduled Time\t: \t%s\n", curr->schedTime);
        gotoxy(20, y++);

        printf("Status\t\t: \t%s\n", curr->status);
        gotoxy(20, y++);

        printf("Terminal\t\t: \t%d\n", curr->terminal);

        if (strcmp(curr->status, "Delayed") == 0)
        {

            gotoxy(20, y++);
            printf("Delay\t\t: \t%d minutes\n", curr->delayTime);
            gotoxy(20, y++);

            printf("Reason\t\t: \t%s\n", curr->delayReason);
        }

        if (!curr->isInc)
        {
            gotoxy(20, y++);
            printf("Seats\t\t: \t%d/%d\n", curr->numOfPassenger, curr->numOfSeat);
            gotoxy(20, y++);
            printf("Priority\t\t: \t%d\n", curr->priority);
        }
    }
    else
    {
        gotoxy(20, 6);
        printf("Flight %d not found!\n", flightNum);
    }
    gotoxy(18, 22);
    printf(YEL "Press any key to continue...\t" reset);
    getch();
}

// Function to show statistics of flights
void showStatistics()
{
    system("cls");
    int totalFlights = 0;
    int delayedFlights = 0;
    int totalPassengers = 0;
    int totalSeats = 0;
    int totalDelayMinutes = 0;

    flight *curr = incomingFront;
    while (curr != NULL)
    {
        totalFlights++;
        if (strcmp(curr->status, "Delayed") == 0)
        {
            delayedFlights++;
            totalDelayMinutes += curr->delayTime;
        }
        curr = curr->next;
    }

    curr = outgoingFront;
    while (curr != NULL)
    {
        totalFlights++;
        if (strcmp(curr->status, "Delayed") == 0)
        {
            delayedFlights++;
            totalDelayMinutes += curr->delayTime;
        }
        totalPassengers += curr->numOfPassenger;
        totalSeats += curr->numOfSeat;
        curr = curr->next;
    }
    int y = 5;
    gotoxy(50, y);
    printf("====== Airport Statistics ======\n");
    y += 3;
    gotoxy(20, y++);
    printf("Total Flights\t\t:\t %d\n", totalFlights);
    gotoxy(20, y++);

    printf("Incoming\t\t\t:\t %d\n", (incomingFront ? 1 : 0));
    gotoxy(20, y++);
    printf("Outgoing\t\t\t:\t %d\n", (outgoingFront ? 1 : 0));
    gotoxy(20, y++);
    printf("Delayed Flights\t\t:\t %d (%.1f%%)\n", delayedFlights,
           totalFlights > 0 ? (float)delayedFlights / totalFlights * 100 : 0);
    gotoxy(20, y++);
    printf("Average Delay\t\t:\t %.1f minutes\n",
           delayedFlights > 0 ? (float)totalDelayMinutes / delayedFlights : 0);
    gotoxy(20, y++);

    printf("Outgoing Seat Occupancy\t:\t %d/%d (%.1f%%)\n", totalPassengers,
           totalSeats,
           totalSeats > 0 ? (float)totalPassengers / totalSeats * 100 : 0);
    gotoxy(20, y++);
    y += 4;
    gotoxy(18, y);
    printf(YEL "Press any key to continue..." reset);
    getch();
}

// Function to rebook passengers from a cancelled flight
void rebookPassengers(flight *cancelledFlight)
{
    if (cancelledFlight->numOfPassenger == 0)
    {
        printf("No passengers to rebook for flight %d.\n",
               cancelledFlight->flightNum);
        return;
    }

    printf("\nRebooking %d passengers from cancelled flight %d...\n",
           cancelledFlight->numOfPassenger, cancelledFlight->flightNum);

    printf("\nAvailable outgoing flights:\n");
    flight *curr = outgoingFront;
    int count = 0;

    while (curr != NULL)
    {
        if (curr != cancelledFlight && strcmp(curr->status, "Cancelled") != 0)
        {
            printf("%d. Flight %d to %s (%d seats available)\n", ++count,
                   curr->flightNum, curr->city,
                   curr->numOfSeat - curr->numOfPassenger);
        }
        curr = curr->next;
    }

    if (count == 0)
    {
        printf("No available flights for rebooking!\n");
        return;
    }

    int choice =
        getValidNumber("\nSelect flight to rebook passengers to: ", 1, count);

    curr = outgoingFront;
    count = 0;
    flight *selectedFlight = NULL;

    while (curr != NULL && selectedFlight == NULL)
    {
        if (curr != cancelledFlight && strcmp(curr->status, "Cancelled") != 0)
        {
            count++;
            if (count == choice)
            {
                selectedFlight = curr;
            }
        }
        curr = curr->next;
    }

    if (selectedFlight != NULL)
    {
        int availableSeats =
            selectedFlight->numOfSeat - selectedFlight->numOfPassenger;
        if (availableSeats >= cancelledFlight->numOfPassenger)
        {
            selectedFlight->numOfPassenger += cancelledFlight->numOfPassenger;
            printf("\nSuccessfully rebooked %d passengers to flight %d\n",
                   cancelledFlight->numOfPassenger, selectedFlight->flightNum);

            if (selectedFlight->numOfPassenger == selectedFlight->numOfSeat)
            {
                strcpy(selectedFlight->status, "Fully Booked");
            }
        }
        else
        {
            printf("\nOnly %d seats available on flight %d. Cannot rebook all "
                   "passengers.\n",
                   availableSeats, selectedFlight->flightNum);
        }
    }
}

// Function to get the current time as a string
char *getCurrentTime()
{
    time_t now;
    struct tm *timeinfo;
    static char timeStr[TimeStrLen];

    time(&now);
    timeinfo = localtime(&now);

    strftime(timeStr, sizeof(timeStr), "%H:%M:%S", timeinfo);
    return timeStr;
}

char *generateTime()
{
    int hours = (lastTime[0] - '0') * 10 + (lastTime[1] - '0');
    int minutes = (lastTime[3] - '0') * 10 + (lastTime[4] - '0');

    minutes += timeIncreCount;
    if (minutes >= 60)
    {
        hours += minutes / 60;
        minutes %= 60;
    }
    if (hours >= 24)
        hours %= 24;

    lastTime[0] = (hours / 10) + '0';
    lastTime[1] = (hours % 10) + '0';
    lastTime[3] = (minutes / 10) + '0';
    lastTime[4] = (minutes % 10) + '0';

    return lastTime;
}

// Function to get the next terminal number
int getNextTerminal(bool isInc)
{
    if (isInc)
    {
        if (lastIncTerm == terminalCount)
            lastIncTerm = 1;
        else
            lastIncTerm++;
        return lastIncTerm;
    }
    else
    {
        if (lastOutTerm == terminalCount)
            lastOutTerm = 1;
        else
            lastOutTerm++;
        return lastOutTerm;
    }
}
// Function to get the urgency level for outgoing flights
int getUrgencyLevel()
{
    system("cls");
    mainbox();
    gotoxy(52, 7);
    printf("Priority Levels");
    gotoxy(40, 10);
    printf("1. Medical Emergency");
    gotoxy(40, 11);
    printf("2. Humanitarian/Disaster Relief");
    gotoxy(40, 12);
    printf("3. Military Operation");
    gotoxy(40, 13);
    printf("4. Diplomatic/Political");
    gotoxy(40, 14);
    printf("5. VIP/Business");
    gotoxy(40, 15);
    printf("6. Weather Avoidance");
    gotoxy(40, 16);
    printf("7. Regular flight (Lowest)");

    return getValidNumber("Enter priority level (1-7): ", 1, 7);
}
