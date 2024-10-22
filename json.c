#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include<stdbool.h>

#define MAX_SEATS_PER_CLASS 10
#define MAX_NAME_LENGTH 30
#define MAX_EMAIL_LENGTH 50
#define MAX_CONTACT_LENGTH 15
#define MAX_LOCATION_LENGTH 50
#define MAX_DESTINATION_LENGTH 50

struct Reservation
{
    char passengerName[MAX_NAME_LENGTH];
    char email[MAX_EMAIL_LENGTH];
    char contactNumber[MAX_CONTACT_LENGTH];
    char source[MAX_LOCATION_LENGTH];
    char destination[MAX_DESTINATION_LENGTH];
    int seatNumber;
    int isBooked;
    float fare;
    int seatClass; 
};

struct Node
{
    struct Reservation data;
    struct Node *next;
};

struct Node *head = NULL;

void displayAvailableSeats();
void bookTicket();
void cancelReservation();
void saveDetailsToFile();
float calculateFare(int seatClass);
float refundFare(float originalFare);
int isAlphaString(const char *str);
int isNumericString(const char *str);
void saveDetailsToJSON();

int availableSeatsFirstClass[MAX_SEATS_PER_CLASS];
int availableSeatsEconomyClass[MAX_SEATS_PER_CLASS];
int availableSeatsBusinessClass[MAX_SEATS_PER_CLASS];

int main()
{
    for (int i = 0; i < MAX_SEATS_PER_CLASS; i++)
    {
        availableSeatsFirstClass[i] = 1;
        availableSeatsEconomyClass[i] = 1;
        availableSeatsBusinessClass[i] = 1;
    }

    int choice;

    do
    {
        printf("\n1. Display Available Seats\n");
        printf("2. Book Ticket\n");
        printf("3. Cancel Reservation\n");
        printf("4. Print Passenger Details\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            displayAvailableSeats();
            break;
        case 2:
            bookTicket();
            break;
        case 3:
            cancelReservation();
            break;
        case 4:
            // Print Passenger Details
            printf("\nPassenger Details:\n");
            struct Node *current = head;
            while (current != NULL)
            {
                printf("Name: %s\n", current->data.passengerName);
                printf("Email: %s\n", current->data.email);
                printf("Contact Number: %s\n", current->data.contactNumber);
                printf("Source: %s\n", current->data.source);
                printf("Destination: %s\n", current->data.destination);
                printf("Seat Number: %d\n", current->data.seatNumber);
                printf("Booking Status: %s\n", (current->data.isBooked == 1) ? "Booked" : "Cancelled");
                printf("Fare: $%.2f\n", current->data.fare);
                printf("Seat Class: %s\n", (current->data.seatClass == 1) ? "First Class" : ((current->data.seatClass == 2) ? "Economy Class" : "Business Class"));
                printf("\n");
                current = current->next;
            }
            break;
        case 5:
            printf("Exiting the program.\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }

    } while (choice != 5);

    saveDetailsToFile();

    struct Node *current = head;
    while (current != NULL)
    {
        struct Node *temp = current;
        current = current->next;
        free(temp);
    }

    return 0;
}

void displayAvailableSeats()
{
    printf("\nAvailable Seats:\n");

    printf("\nFirst Class:\n");
    for (int i = 0; i < MAX_SEATS_PER_CLASS; i++)
    {
        printf("Seat %02d [%c]   ", i + 1, (availableSeatsFirstClass[i] ? 'O' : 'X'));
        if ((i + 1) % 5 == 0)
        {
            printf("\n");
        }
        else if ((i + 1) % 2 == 0)
        {
            printf("   ");
        }
    }

    printf("\nEconomy Class:\n");
    for (int i = 0; i < MAX_SEATS_PER_CLASS; i++)
    {
        printf("Seat %02d [%c]   ", i + 1, (availableSeatsEconomyClass[i] ? 'O' : 'X'));
        if ((i + 1) % 5 == 0)
        {
            printf("\n");
        }
        else if ((i + 1) % 2 == 0)
        {
            printf("   ");
        }
    }

    printf("\nBusiness Class:\n");
    for (int i = 0; i < MAX_SEATS_PER_CLASS; i++)
    {
        printf("Seat %02d [%c]   ", i + 1, (availableSeatsBusinessClass[i] ? 'O' : 'X'));
        if ((i + 1) % 5 == 0)
        {
            printf("\n");
        }
        else if ((i + 1) % 2 == 0)
        {
            printf("   ");
        }
    }

    printf("\n");
}

void bookTicket()
{
    int numSeats, seatNumber, seatClass;
    char passengerName[MAX_NAME_LENGTH];
    char email[MAX_EMAIL_LENGTH];
    char contactNumber[MAX_CONTACT_LENGTH];
    char source[MAX_LOCATION_LENGTH];
    char destination[MAX_DESTINATION_LENGTH];

    printf("\nEnter number of seats to book: ");
    scanf("%d", &numSeats);

    if (numSeats < 1)
    {
        printf("Invalid number of seats.\n");
        return;
    }

    for (int s = 0; s < numSeats; s++)
    {
        printf("\nEnter seat number to book: ");
        scanf("%d", &seatNumber);

        if (seatNumber < 1 || seatNumber > MAX_SEATS_PER_CLASS)
        {
            printf("Invalid seat number.\n");
            return;
        }

        printf("Select seat class:\n");
        printf("1. First Class\n");
        printf("2. Economy Class\n");
        printf("3. Business Class\n");
        printf("Enter choice: ");
        scanf("%d", &seatClass);

        if (seatClass < 1 || seatClass > 3)
        {
            printf("Invalid seat class.\n");
            return;
        }

        int *availableSeats;
        switch (seatClass)
        {
        case 1:
            availableSeats = availableSeatsFirstClass;
            break;
        case 2:
            availableSeats = availableSeatsEconomyClass;
            break;
        case 3:
            availableSeats = availableSeatsBusinessClass;
            break;
        default:
            return;
        }

        if (!availableSeats[seatNumber - 1])
        {
            printf("Seat %d is already booked.\n", seatNumber);
            return;
        }

        printf("Enter passenger name: ");
        scanf("%s", passengerName);

        if (!isAlphaString(passengerName))
        {
            printf("Invalid name. Name should contain only alphabets.\n");
            return;
        }

        printf("Enter passenger email (gmail only): ");
        scanf("%s", email);

        if (strstr(email, "@gmail.com") == NULL)
        {
            printf("Invalid email. Please enter a Gmail address.\n");
            return;
        }

        printf("Enter passenger contact number: ");
        scanf("%s", contactNumber);

        if (!isNumericString(contactNumber))
        {
            printf("Invalid contact number. Contact number should contain only numbers.\n");
            return;
        }

        printf("Enter source location: ");
        scanf("%s", source);

        printf("Enter destination: ");
        scanf("%s", destination);

        float fare = calculateFare(seatClass);

        struct Reservation newReservation;
        strcpy(newReservation.passengerName, passengerName);
        strcpy(newReservation.email, email);
        strcpy(newReservation.contactNumber, contactNumber);
        strcpy(newReservation.source, source);
        strcpy(newReservation.destination, destination);
        newReservation.seatNumber = seatNumber;
        newReservation.isBooked = 1;
        newReservation.fare = fare;
        newReservation.seatClass = seatClass;

        struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
        newNode->data = newReservation;
        newNode->next = NULL;

        if (head == NULL)
        {
            head = newNode;
        }
        else
        {
            struct Node *current = head;
            while (current->next != NULL)
            {
                current = current->next;
            }
            current->next = newNode;
        }

        availableSeats[seatNumber - 1] = 0;

        printf("Ticket booked successfully for seat %d. Fare: $%.2f\n", seatNumber, fare);
    }
}

void cancelReservation()
{
    int seatNumber, seatClass;

    printf("\nEnter seat number to cancel reservation: ");
    scanf("%d", &seatNumber);

    if (seatNumber < 1 || seatNumber > MAX_SEATS_PER_CLASS)
    {
        printf("Invalid seat number.\n");
        return;
    }

    printf("Select seat class:\n");
    printf("1. First Class\n");
    printf("2. Economy Class\n");
    printf("3. Business Class\n");
    printf("Enter choice: ");
    scanf("%d", &seatClass);

    if (seatClass < 1 || seatClass > 3)
    {
        printf("Invalid seat class.\n");
        return;
    }

    int *availableSeats;
    switch (seatClass)
    {
    case 1:
        availableSeats = availableSeatsFirstClass;
        break;
    case 2:
        availableSeats = availableSeatsEconomyClass;
        break;
    case 3:
        availableSeats = availableSeatsBusinessClass;
        break;
    default:
        return;
    }

    if (!availableSeats[seatNumber - 1])
    {
        struct Node *current = head;
        while (current != NULL)
        {
            if (current->data.seatNumber == seatNumber && current->data.seatClass == seatClass && current->data.isBooked)
            {
                availableSeats[seatNumber - 1] = 1;
                current->data.isBooked = 0;
                printf("Reservation canceled successfully for seat %d in %s class. Refund: $%.2f\n", seatNumber, (seatClass == 1) ? "First" : (seatClass == 2) ? "Economy" : "Business", refundFare(current->data.fare));
                return;
            }
            current = current->next;
        }
        printf("No reservation found for seat %d in %s class.\n", seatNumber, (seatClass == 1) ? "First" : (seatClass == 2) ? "Economy" : "Business");
    }
    else
    {
        printf("Seat %d is not booked in %s class.\n", seatNumber, (seatClass == 1) ? "First" : (seatClass == 2) ? "Economy" : "Business");
    }
}


void saveDetailsToFile()
{
    saveDetailsToJSON(); // Call the function to save data to JSON
}

void saveDetailsToJSON()
{
    FILE *fpointer = fopen("passenger_records.json", "w");
    if (!fpointer)
    {
        printf("\nError in opening file!\n");
        return;
    }

    fprintf(fpointer, "{\"records\": [");

    struct Node *current = head;
    bool isFirstRecord = true;
    while (current != NULL)
    {
        if (!isFirstRecord)
        {
            fprintf(fpointer, ",");
        }
        fprintf(fpointer, "{\"passengerName\": \"%s\", \"email\": \"%s\", \"contactNumber\": \"%s\", \"source\": \"%s\", \"destination\": \"%s\", \"seatNumber\": %d, \"isBooked\": %d, \"fare\": %.2f, \"seatClass\": %d}",
                current->data.passengerName, current->data.email, current->data.contactNumber, current->data.source, current->data.destination, current->data.seatNumber, current->data.isBooked, current->data.fare, current->data.seatClass);
        isFirstRecord = false;
        current = current->next;
    }

    fprintf(fpointer, "]}\n");

    printf("\nDetails have been saved to a JSON file (passenger_records.json).\n");
    fclose(fpointer);
}

float calculateFare(int seatClass)
{
    float fare = 0;
    switch (seatClass)
    {
    case 1:
        fare = 200.0; 
        break;
    case 2:
        fare = 100.0; 
        break;
    case 3:
        fare = 150.0; 
        break;
    default:
        printf("Invalid seat class. Setting fare to default value.\n");
        fare = 100.0; 
    }
    return fare;
}

float refundFare(float originalFare)
{
    return originalFare * 0.5;
}

int isAlphaString(const char *str)
{
    while (*str)
    {
        if (!isalpha(*str))
            return 0;
        str++;
    }
    return 1;
}

int isNumericString(const char *str)
{
    while (*str)
    {
        if (!isdigit(*str))
            return 0;
        str++;
    }
    return 1;
}
