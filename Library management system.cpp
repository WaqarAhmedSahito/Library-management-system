#include <iostream>
#include <string>
#include <cstdlib>
#include <conio.h>
#include <limits>

using namespace std;

// Global variables
int quantity;
string studname;
string idnum;
string studentpassword;
string adminpass = "1234";
bool foundacc = false;

// Book node class
class booknode {
public:
    string username; // Student who borrowed the book
    string bookname; // Name of the book
    int quantity;    // Quantity of the book
    booknode* next;
    booknode* prev;

    // Constructor
    booknode(string username, string bookname, int quantity = 1) {
        this->username = username;
        this->bookname = bookname;
        this->quantity = quantity;
        this->next = nullptr;
        this->prev = nullptr;
    }
};

// User node class
class node {
public:
    string username;
    string password;
    int balance;
    node* next;
    node* prev;

    // Constructor
    node(string username, string password, int balance = 100) {
        this->username = username;
        this->password = password;
        this->balance = balance;
        this->next = nullptr;
        this->prev = nullptr;
    }
};

// Global book list pointers
booknode* bookhead = nullptr;
booknode* booktail = nullptr;

// Function prototypes
void login_admin(node*& head);
void admin(node*& head);
void loginstudentupdated(node*& head, node*& tail);
void depositmoney(node*& studentnode, node*& tail);
void insertend(node*& tail, string username, string password);
void traverse(node*& head);
void genidnum();
void adminaccount(node*& head, node*& tail);
void registerstudent(node*& head, node*& tail);
void student(node*& studentnode, node*& tail);
void borrowBooks(node*& studentnode, node*& tail);
void returnBooks(node*& studentnode, node*& tail);
void viewBorrowedBooks(node*& studentnode, node*& tail);
void initializeBooks(node*& head);
void deleteBook(booknode*& head, string k);

// Utility functions for input validation
int getMenuChoice(int minChoice, int maxChoice) {
    int choice;
    while (true) {
        cout << "Enter choice: ";
        cin >> choice;
        if (cin.fail() || choice < minChoice || choice > maxChoice) {
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "Invalid choice. Please enter a number between " << minChoice << " and " << maxChoice << ".\n";
        }
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard any extra input
            return choice;
        }
    }
}

string getPasswordInput(const string& prompt) {
    cout << prompt;
    string password;
    char ch;

    while ((ch = _getch()) != '\r') { // '\r' is the Enter key
        if (ch == '\b') { // Backspace key pressed
            if (!password.empty()) {
                password.pop_back();
                cout << "\b \b"; // Move cursor back and erase the character
            }
        }
        else {
            password += ch;
            cout << '*'; // Display asterisk instead of the actual character
        }
    }
    cout << endl;
    return password;
}

string getNonEmptyString(const string& prompt) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        if (!input.empty()) {
            return input;
        }
        cout << "Input cannot be empty. Please try again.\n";
    }
}

int getPositiveNumber(const string& prompt) {
    int number;
    while (true) {
        cout << prompt;
        cin >> number;
        if (cin.fail() || number <= 0) {
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "Invalid input. Please enter a positive number.\n";
        }
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard any extra input
            return number;
        }
    }
}

// Delete a book from the book list
void deleteBook(booknode*& head, string k) {
    if (head == nullptr) {
        cout << "List is empty. Nothing to delete." << endl;
        return;
    }

    booknode* temp = head;
    while (temp != nullptr && temp->bookname != k) {
        temp = temp->next;
    }

    if (temp == nullptr) {
        cout << "Book with name '" << k << "' not found." << endl;
        return;
    }

    if (temp == head) {
        head = head->next;
        if (head != nullptr) {
            head->prev = nullptr;
        }
    }
    else {
        if (temp->prev != nullptr) {
            temp->prev->next = temp->next;
        }
        if (temp->next != nullptr) {
            temp->next->prev = temp->prev;
        }
    }
    delete temp;
    cout << "Book '" << k << "' deleted." << endl;
}

// Insert a new user at the end of the user list
void insertend(node*& tail, string username, string password) {
    node* temp = new node(username, password);
    tail->next = temp;
    temp->prev = tail;
    tail = temp;
}

// Traverse and display all users
void traverse(node*& head) {
    node* temp = head;

    while (temp != nullptr) {
        cout << endl;
        cout << "Username: " << temp->username<<", Password: "<<temp->password << ", Balance: " << temp->balance << endl;
        temp = temp->next;
    }
    admin(head);
    cout << endl;
}

// Search for a user in the user list
void search(node*& head, string data, bool isfound) {
    node* temp = head;
    while (temp != nullptr) {
        if (temp->username == data) {
            isfound = true;
            foundacc = isfound;
            studentpassword = temp->password;
            return;
        }
        temp = temp->next;
    }
    isfound = false;
}

// Generate a random ID number
void genidnum() {
    idnum = "";
    char idnumarr[10] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

    for (int i = 0; i <= 4; i++) {
        int randindex = rand() % 10;
        idnum += idnumarr[randindex];
    }
    cout << "ID Number: " << idnum << endl;
}

// Initialize the admin account
void adminaccount(node*& head, node*& tail) {
    insertend(tail, "admin", adminpass);
}

// Register or login as a student
void registerstudent(node*& head, node*& tail) {
    char ch ;
    bool cont =true;
    while (cont ==true) {
        cout << "1) Register Account\n2) Login Existing Account\n3) Login as Admin\n4) Exit\n";
        int loginchoice = getMenuChoice(1, 4);

        switch (loginchoice) {
        case 1: {
            bool nameexist = false;
            int attempts = 0;
            int maxattempts = 5;

            while (attempts < maxattempts) {
                studname = getNonEmptyString("Assign username: ");

                node* temp = head;
                while (temp != nullptr) {
                    if (temp->username == studname) {
                        nameexist = true;
                        break;
                    }
                    temp = temp->next;
                }

                if (nameexist) {
                    cout << "This username already exists!\n";
                    cout << "Press \"Enter\" to try again or \"Esc\" to proceed to main menu.\n";

                    char ch;
                    while (true) {
                        if (_kbhit()) {
                            ch = _getch();
                            break;
                        }
                    }

                    if (ch == 13) {
                        cout << "Enter pressed. Trying again.\n";
                        nameexist = false;
                        attempts++;
                        continue;
                    }
                    else if (ch == 27) {
                        cout << "Esc pressed. Returning to main menu.\n";
                        registerstudent(head, tail);
                        return;
                    }
                }
                else {
                    studentpassword = getNonEmptyString("Assign password: ");

                    genidnum();
                    insertend(tail, studname, studentpassword);
                    cout << "Account created successfully!\n";
                    break;
                }
            }

            if (attempts >= maxattempts) {
                cout << "Maximum attempts reached. Returning to main menu.\n";
            }
            break;
        }
        case 2: {
            loginstudentupdated(head, tail);
            break;
        }
        case 3: {
            login_admin(head);
            break;
        }
        case 4: {
            cout << "Exiting program.\n";
            return;
        }
        default:
            cout << "Invalid choice.\n";
            break;
        }

        cout << "Press Esc for Main Menu OR Press Enter to Login Student Account " << endl;
        cout << "Shift+0 to Exit" << endl;
        cout << endl;

        while (true)
        {
            if (_kbhit()) {
                ch = _getch();
                break;
            }
        }
        if (ch == 13) {
            loginstudentupdated(head, tail);
            
        }
        else if (ch == 27) {
            registerstudent(head, tail);
        }
        //Shift + `
        else if (ch == 41) {
            break;
        }
        cout << endl;
    }
}

// Admin login
void login_admin(node*& head) {
    string username = getNonEmptyString("Enter admin username: ");
    string password = getPasswordInput("Enter password: ");

    if (username == "admin" && password == adminpass) {
        cout << endl;

        cout << "Admin logged in successfully.\n";
        admin(head);
    }
    else {
        cout << "Invalid admin credentials.\n";
    }
}

// Student menu
void student(node*& studentnode, node*& tail) {
    cout << endl;

    cout << "\nStudent Options:\n1. View Books\n2. Borrow Book\n3. Return Book\n4. Deposit Money\n5. Logout User\n6. Exit\n";
    int option = getMenuChoice(1, 6);

    switch (option) {
    case 1: {
        viewBorrowedBooks(studentnode, tail);
        break;
    }
    case 2: {
        borrowBooks(studentnode, tail);
        break;
    }
    case 3: {
        returnBooks(studentnode, tail);
        break;
    }
    case 4: {
        depositmoney(studentnode, tail);
        break;
    }
    case 5: {
        cout << "User logged out successfully!\n";
        registerstudent(studentnode, tail);
        break;
    }
    case 6: {
        cout << "Exiting student menu.\n";
        break;
    }
    default: {
        cout << "Invalid option. Returning to main menu.\n";
        break;
    }
    }
}

// Student login


void loginstudentupdated(node*& head, node*& tail) {
    cout << endl;

    string inputid = getNonEmptyString("Enter username: ");

    node* temp = head;
    while (temp != nullptr) {
        if (temp->username == inputid) {
            int trycnt = 0; // Counter for password attempts
            const int maxattempts = 3;

            while (trycnt < maxattempts) {
                string inputpass = getPasswordInput("Enter password: ");

                if (inputpass == temp->password) {
                    cout << "Welcome " << inputid << ", you are now logged in as a student.\n";
                    student(temp, tail); // Pass the node* to the student function
                    return;
                }
                else {
                    trycnt++;
                    cout << "Invalid password. Attempt " << trycnt << " of " << maxattempts << ".\n";

                    if (trycnt >= maxattempts) {
                        cout << "Maximum login attempts reached. Returning to the main menu.\n";
                        return;
                    }
                }
            }
        }
        temp = temp->next;
    }
    cout << "User not found.\n";
}

// Admin menu
void admin(node*& head) {
    cout << endl;

    cout << "\nAdmin Options:\n1. Add Book\n2. Edit Book\n3. View Books\n4. Generate Users List\n5. Exit\n";
    int option = getMenuChoice(1, 5);

    switch (option) {
    case 1: {
        cout << "Add book selected.\n";
        initializeBooks(head);
        break;
    }
    case 2: {
        cout << "Edit book selected.\n";
        break;
    }
    case 3: {
        cout << "View books selected.\n";
        break;
    }
    case 4: {
        cout << "Generating list of users...\n";
        traverse(head);
        break;
    }
    case 5: {
        cout << "Exiting admin menu.\n";
        break;
    }
    default:
        cout << "Invalid option. Returning to main menu.\n";
        return;
    }
}

// Deposit money into student account
void depositmoney(node*& studentnode, node*& tail) {
    if (studentnode == nullptr) {
        cout << "Error: No student account found.\n";
        return;
    }

    cout << "Your current balance is: $" << studentnode->balance << endl;

    int depositamount = getPositiveNumber("Enter the amount you wish to deposit: ");

    studentnode->balance += depositamount;
    cout << "Deposit successful. Your new balance is: $" << studentnode->balance << endl;

    student(studentnode, tail);
}

// Initialize books
void initializeBooks(node*& head) {
    int numBooks = getPositiveNumber("Enter the number of books to add: ");

    for (int i = 0; i < numBooks; ++i) {
        string bookname = getNonEmptyString("Enter the name of book " + to_string(i + 1) + ": ");
        int quantity = getPositiveNumber("Enter the quantity of book " + bookname + ": ");

        booknode* newBook = new booknode("Library", bookname, quantity);

        if (bookhead == nullptr) {
            bookhead = newBook;
            booktail = newBook;
        }
        else {
            booktail->next = newBook;
            newBook->prev = booktail;
            booktail = newBook;
        }
    }

    cout << "Books initialized successfully!\n";
    cout << endl;
    cout << "Heading Back to Admin Panel..." << endl;

    admin(head);
}

// Borrow books
void borrowBooks(node*& studentnode, node*& tail) {
    if (studentnode == nullptr) {
        cout << "Error: No student account found.\n";
        return;
    }

    cout << "Available Books:\n";
    booknode* temp = bookhead;
    int index = 1;
    while (temp != nullptr) {
        if (temp->username == "Library") {
            cout << index << ". " << temp->bookname << " (Available: " << temp->quantity << ")\n";
        }
        temp = temp->next;
        index++;
    }

    int bookIndex = getPositiveNumber("Enter the index of the book you want to borrow: ");

    // Validate the input index
    if (bookIndex < 1 || bookIndex >= index) {
        cout << "Invalid book index.\n";
        return;
    }

    // Find the book at the given index
    temp = bookhead;
    for (int i = 1; i < bookIndex; ++i) {
        if (temp == nullptr) {
            cout << "Invalid book index.\n";
            return;
        }
        temp = temp->next;
    }

    // Check if the book is available
    if (temp->quantity <= 0) {
        cout << "Book '" << temp->bookname << "' is out of stock.\n";
        student(studentnode, tail);
    }

    // Check if the student has already borrowed the book
    booknode* tempB = bookhead;
    while (tempB != nullptr) {
        if (tempB->bookname == temp->bookname && tempB->username == studentnode->username) {
            cout << "You have already borrowed this book.\n";
            return;
        }
        tempB = tempB->next;
    }

    // Decrement the book quantity
    temp->quantity--;

    // Create a new booknode for the borrowed book
    booknode* newBook = new booknode(studentnode->username, temp->bookname);

    // Add the new booknode to the book list
    if (studentnode->balance >= 20) {
        if (bookhead == nullptr) {
            bookhead = newBook;
            booktail = newBook;
        }
        else {
            booktail->next = newBook;
            newBook->prev = booktail;
            booktail = newBook;
        }
        studentnode->balance -= 20;
        cout << "Book '" << temp->bookname << "' borrowed successfully!\nYour new balance is: " << studentnode->balance << endl;
    }
    else {
        cout << "Insufficient Balance!\n";
    }

    // Deduct the cost from the student's balance
    cout << "Press Esc to go back to Student Panel OR Press Enter to Borrow Another Book.\n";

    char ch;
    while (true) {
        if (_kbhit()) {
            ch = _getch();
            break;
        }
    }

    if (ch == 13) {
        borrowBooks(studentnode, tail);
    }
    else if (ch == 27) {
        cout << endl;
        cout << "Back to Main Menu Selected\n";
        student(studentnode, tail);
    }
}

// Return books
#include <iostream>
#include <string>
#include <conio.h> // For _kbhit() and _getch() (Windows-specific)

using namespace std;

// Function to check if the Esc key is pressed
bool isEscPressed() {
    return _kbhit() && _getch() == 27; 
}

void returnBooks(node*& studentnode, node*& tail) {
    if (studentnode == nullptr) {
        cout << "Error: No student account found.\n";
        return;
    }

    cout << "Books borrowed by " << studentnode->username << ":\n";
    booknode* temp1 = bookhead;
    bool hasBooks = false;

    // Display all books borrowed by the student
    while (temp1 != nullptr) {
        if (temp1->username == studentnode->username) {
            cout << "- " << temp1->bookname << endl;
            hasBooks = true;
        }
        temp1 = temp1->next;
    }

    if (!hasBooks) {
        cout << "No books borrowed.\n";
        return;
    }

    // Loop to repeatedly ask for the book name
    while (true) {
        cout << "Enter the name of the book you want to return (or press Esc to exit): ";
        string bookname;

        //read input as chars and storing them in a string variable
        char ch;
        while ((ch = _getch()) != '\r') { // '\r' is the Enter key
            if (ch == 27) { // Esc key pressed
                cout << "\nExiting book return process.\n";
                student(studentnode,tail);
            }
            else if (ch == '\b') { // Backspace key pressed
                if (!bookname.empty()) {//goes back removing chars until bookname is empty
                    bookname.pop_back();
                    cout << "\b \b"; // Move cursor back and erase the character
                }
            }
            else {
                bookname += ch;
                cout << ch;
            }
        }
        cout << endl;

        // Search for the book in the book list
        booknode* temp = bookhead;
        bool bookFound = false;

        while (temp != nullptr) {
            if (temp->username == studentnode->username && temp->bookname == bookname) {
                // Increment the quantity of the returned book in the library
                booknode* libraryBook = bookhead;
                while (libraryBook != nullptr) {
                    if (libraryBook->bookname == bookname && libraryBook->username == "Library") {
                        libraryBook->quantity++;
                        studentnode->balance += 18;
                        break;
                    }
                    libraryBook = libraryBook->next;
                }

                // Remove the borrowed book from the list
                if (temp->prev != nullptr) {
                    temp->prev->next = temp->next;
                }
                else {
                    bookhead = temp->next;
                }

                if (temp->next != nullptr) {
                    temp->next->prev = temp->prev;
                }
                else {
                    booktail = temp->prev;
                }

                delete temp;
                cout << "Book '" << bookname << "' returned successfully!\n";
                student(studentnode, tail);

                
            }
            temp = temp->next;
        }

        // If the book is not found
        cout << "You have not borrowed this book. Please try again.\n";
    }
}

// View borrowed books
void viewBorrowedBooks(node*& studentnode, node*& tail) {
    if (studentnode == nullptr) {
        cout << "Error: No student account found.\n";
        return;
    }

    cout << "Books borrowed by " << studentnode->username << ":\n";
    booknode* temp = bookhead;
    bool hasBooks = false;

    while (temp != nullptr) {
        if (temp->username == studentnode->username) {
            cout << "- " << temp->bookname << endl;
            hasBooks = true;
        }
        temp = temp->next;
    }

    if (!hasBooks) {
        cout << "No books borrowed.\n";
    }
    cout << "Press Esc for Main Menu\n";
    char ch;
    while (true) {
        if (_kbhit()) {
            ch = _getch();
            break;
        }
    }
    if (ch == 27) {
        student(studentnode, tail);
    }
}

// Main function
int main() {
    node* head = new node("admin", adminpass);
    node* tail = head;

    booknode* book1 = new booknode("Library", "BookA", 3);
    booknode* book2 = new booknode("Library", "BookB", 1);

    book1->next = book2;
    book2->prev = book1;

    bookhead = book1;
    booktail = book2;
    cout << "----------WELCOME TO E-LIBRARY----------" << endl;
    registerstudent(head, tail);

    return 0;
}