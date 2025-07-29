#include <iostream>
#include <string>
using namespace std;

#define MAX_SEATS 3
#define WAITING_LIMIT 3

struct Passenger {
    string name;
    int age;
    Passenger* next;
};

struct Train {
    int trainNo;
    string trainName;
    string source;
    string destination;
    Passenger* bookedHead;
    Passenger* waitingHead;
    Train* next;
};

Train* trainHead = nullptr;

Train* findTrain(int trainNo) {
    Train* temp = trainHead;
    while (temp != nullptr) {
        if (temp->trainNo == trainNo)
            return temp;
        temp = temp->next;
    }
    return nullptr;
}

void addTrain() {
    Train* newTrain = new Train;
    cout << "Enter Train Number: ";
    cin >> newTrain->trainNo;
    cout << "Enter Train Name: ";
    cin.ignore();
    getline(cin, newTrain->trainName);
    cout << "Enter Source: ";
    getline(cin, newTrain->source);
    cout << "Enter Destination: ";
    getline(cin, newTrain->destination);
    newTrain->bookedHead = nullptr;
    newTrain->waitingHead = nullptr;
    newTrain->next = trainHead;
    trainHead = newTrain;
    cout << "Train added successfully!\n";
}

void viewTrains() {
    if (!trainHead) {
        cout << "No trains available.\n";
        return;
    }
    Train* temp = trainHead;
    while (temp) {
        cout << "\nTrain No: " << temp->trainNo;
        cout << "\nTrain Name: " << temp->trainName;
        cout << "\nRoute: " << temp->source << " -> " << temp->destination << "\n";
        temp = temp->next;
    }
}

int countPassengers(Passenger* head) {
    int count = 0;
    while (head) {
        count++;
        head = head->next;
    }
    return count;
}

void addPassenger(Passenger*& head, string name, int age) {
    Passenger* newP = new Passenger{name, age, nullptr};
    if (!head) {
        head = newP;
    } else {
        Passenger* temp = head;
        while (temp->next) temp = temp->next;
        temp->next = newP;
    }
}

void bookTicket() {
    int trainNo;
    cout << "Enter Train Number: ";
    cin >> trainNo;
    Train* train = findTrain(trainNo);
    if (!train) {
        cout << "Train not found!\n";
        return;
    }

    string name;
    int age;
    cout << "Enter Passenger Name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter Age: ";
    cin >> age;

    int bookedCount = countPassengers(train->bookedHead);
    int waitCount = countPassengers(train->waitingHead);

    if (bookedCount < MAX_SEATS) {
        addPassenger(train->bookedHead, name, age);
        cout << "Ticket Booked Successfully in Main List!\n";
    } else if (waitCount < WAITING_LIMIT) {
        addPassenger(train->waitingHead, name, age);
        cout << "Main list full. Added to Waiting List.\n";
    } else {
        cout << "Both Main and Waiting Lists are Full. Cannot Book.\n";
    }
}

void viewBookings() {
    int trainNo;
    cout << "Enter Train Number to View Bookings: ";
    cin >> trainNo;
    Train* train = findTrain(trainNo);
    if (!train) {
        cout << "Train not found!\n";
        return;
    }

    cout << "\n--- Confirmed Bookings ---\n";
    Passenger* temp = train->bookedHead;
    int i = 1;
    while (temp) {
        cout << i++ << ". " << temp->name << " (" << temp->age << " yrs)\n";
        temp = temp->next;
    }
    if (i == 1) cout << "No bookings.\n";

    cout << "\n--- Waiting List ---\n";
    temp = train->waitingHead;
    i = 1;
    while (temp) {
        cout << i++ << ". " << temp->name << " (" << temp->age << " yrs)\n";
        temp = temp->next;
    }
    if (i == 1) cout << "No one in waiting list.\n";
}

bool removePassenger(Passenger*& head, string name) {
    Passenger* temp = head;
    Passenger* prev = nullptr;
    while (temp) {
        if (temp->name == name) {
            if (!prev)
                head = temp->next;
            else
                prev->next = temp->next;
            delete temp;
            return true;
        }
        prev = temp;
        temp = temp->next;
    }
    return false;
}

void promoteFromWaiting(Train* train) {
    if (!train->waitingHead) return;

    Passenger* firstWait = train->waitingHead;
    train->waitingHead = train->waitingHead->next;
    firstWait->next = nullptr;

    Passenger* last = train->bookedHead;
    while (last && last->next) last = last->next;
    if (!last)
        train->bookedHead = firstWait;
    else
        last->next = firstWait;
}

void cancelTicket() {
    int trainNo;
    string name;
    cout << "Enter Train Number: ";
    cin >> trainNo;
    cout << "Enter Passenger Name to Cancel: ";
    cin.ignore();
    getline(cin, name);

    Train* train = findTrain(trainNo);
    if (!train) {
        cout << "Train not found!\n";
        return;
    }

    bool removed = removePassenger(train->bookedHead, name);
    if (removed) {
        cout << "Ticket cancelled from Confirmed List.\n";
        promoteFromWaiting(train);
    } else {
        removed = removePassenger(train->waitingHead, name);
        if (removed)
            cout << "Ticket cancelled from Waiting List.\n";
        else
            cout << "Passenger not found in any list.\n";
    }
}

void searchBooking() {
    string name;
    cout << "Enter Passenger Name to Search: ";
    cin.ignore();
    getline(cin, name);
    Train* t = trainHead;
    bool found = false;

    while (t) {
        Passenger* p = t->bookedHead;
        while (p) {
            if (p->name == name) {
                cout << "Passenger found in Train " << t->trainNo << " (Confirmed)\n";
                found = true;
            }
            p = p->next;
        }
        p = t->waitingHead;
        while (p) {
            if (p->name == name) {
                cout << "Passenger found in Train " << t->trainNo << " (Waiting)\n";
                found = true;
            }
            p = p->next;
        }
        t = t->next;
    }

    if (!found)
        cout << "Passenger not found in any train.\n";
}

int main() {
    int choice;
    do {
        cout << "\n--- Railway Reservation System ---\n";
        cout << "1. Add Train\n";
        cout << "2. View Trains\n";
        cout << "3. Book Ticket\n";
        cout << "4. View Bookings\n";
        cout << "5. Cancel Ticket\n";
        cout << "6. Search Booking\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
            case 1: addTrain(); break;
            case 2: viewTrains(); break;
            case 3: bookTicket(); break;
            case 4: viewBookings(); break;
            case 5: cancelTicket(); break;
            case 6: searchBooking(); break;
            case 0: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while (choice != 0);
    return 0;
}
