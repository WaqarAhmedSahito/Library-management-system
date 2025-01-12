#include <iostream>
#include <string>
#include <cstdlib>
#include <conio.h>

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
void initializeBooks(node* &head);
void deleteBook(booknode*& head, string k);

// Function definitions

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
        cout << "Username: " << temp->username << ", Balance: " << temp->balance << endl;
        temp = temp->next;
    }
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
    int cont = 0;
    while (cont == 0) {
        cout << "1) Register Account\n2) Login Existing Account\n3) Login as Admin\n4) Exit\n";
        cout << "Enter choice: ";
        int loginchoice;
        cin >> loginchoice;

        switch (loginchoice) {
        case 1: {
            bool nameexist = false;
            int attempts = 0;
            int maxattempts = 5;

            while (attempts < maxattempts) {
                studname = "";
                cout << "Assign username: ";
                cin >> studname;

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
                    cout << "Assign password: ";
                    cin >> studentpassword;

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

        cout << "Continue? 0=Yes 1=No: ";
        cin >> cont;
        cout << endl;
    }
}

// Admin login
void login_admin(node*& head) {
    string username, password;
    cout << "Enter admin username: ";
    cin >> username;
    cout << "Enter admin password: ";
    cin >> password;

    if (username == "admin" && password == adminpass) {
        cout << "Admin logged in successfully.\n";
        admin(head);
    }
    else {
        cout << "Invalid admin credentials.\n";
    }
}

// Student menu
void student(node*& studentnode, node*& tail) {
    cout << "\nStudent Options:\n1. View Books\n2. Borrow Book\n3. Return Book\n4. Deposit Money\n5. Logout User\n6. Exit\n";
    int option;
    cin >> option;

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
    string inputpass;
    string inputid;

    cout << "Enter username: ";
    cin >> inputid;

    node* temp = head;
    while (temp != nullptr) {
        if (temp->username == inputid) {
            int trycnt = 0; // Counter for password attempts
            const int maxattempts = 3;

            while (trycnt < maxattempts) {
                cout << "Enter password: ";
                cin >> inputpass;

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
    cout << "\nAdmin Options:\n1. Add Book\n2. Edit Book\n3. View Books\n4. Generate Users List\n5. Exit\n";
    int option;
    cout << "Enter Choice: ";
    cin >> option;

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

    int depositamount;
    cout << "Enter the amount you wish to deposit: ";
    cin >> depositamount;

    if (depositamount > 0) {
        studentnode->balance += depositamount;
        cout << "Deposit successful. Your new balance is: $" << studentnode->balance << endl;
    }
    else {
        cout << "Invalid deposit amount. Try again.\n";
    }
    student(studentnode, tail);
}

//void initializeBooks() {
//    // Add 5 books to the list with initial quantities
//    booknode* book1 = new booknode("Library", "BookA", 3);
//    booknode* book2 = new booknode("Library", "BookB", 2);
//    booknode* book3 = new booknode("Library", "BookC", 5);
//    booknode* book4 = new booknode("Library", "BookD", 4);
//    booknode* book5 = new booknode("Library", "BookE", 1);
//
//    // Link the books in a doubly linked list
//    book1->next = book2;
//    book2->prev = book1;
//    book2->next = book3;
//    book3->prev = book2;
//    book3->next = book4;
//    book4->prev = book3;
//    book4->next = book5;
//    book5->prev = book4;
//
//    // Set the head and tail of the book list
//    bookhead = book1;
//    booktail = book5;
//}



void initializeBooks(node* &head) {
    int numBooks = 0;

    cout << "Enter the number of books to add OR leave empty to Go back to Admin Panel: ";
    cin >> numBooks;

    

    booknode* temp = bookhead;

    for (int i = 0; i < numBooks; ++i) {
        string bookname;
        int quantity;
        bool bookExist = false;
        cout << "Enter the name of book " << i + 1 << ": ";
        cin >> bookname;

        while (bookhead != NULL) {
            if(bookhead->bookname==bookname){
                cout << "Book already exist! " << endl;
                bookExist = true;
                cout << "Press Enter to increase Quantity\nPress Esc to Cancel " << endl;
                
                char ch;
                while (true) {
                    if (_kbhit()) {
                        ch = _getch();
                        break;
                    }
                }
                if (ch == 13) {
                    cout << "Current Quantity: " << bookhead->quantity << " Enter Increment size: ";
                    cin >> quantity;
                    bookhead->quantity += quantity;
                    cout << "New Quantity of Book " << bookhead->bookname << " is " << bookhead->quantity << endl;
                    break;
                }
                if (ch == 27) {
                    
                    cout << "Moving to next Book..." << endl;
                    break;
                }
            }
        }
        

        // Create a new book node
        if(bookExist==false)
        {
            cout << "Enter the quantity of book " << bookname << ": ";
            cin >> quantity;

            booknode* newBook = new booknode("Library", bookname, quantity);

            // Add the new book to the list
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
    }

    cout << "Books initialized successfully!\n";
}




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

    int bookIndex;
    cout << "Book is $20 each\n";
    cout << "Enter the index of the book you want to borrow: ";
    cin >> bookIndex;

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
  
        return ;

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
    if (bookhead == nullptr) {
        bookhead = newBook;
        booktail = newBook;
    }
    else {
        booktail->next = newBook;
        newBook->prev = booktail;
        booktail = newBook;
    }

    // Deduct the cost from the student's balance
    studentnode->balance -= 20;
    cout << "Book '" << temp->bookname << "' borrowed successfully!\nYour new balance is: " << studentnode->balance << endl;
    
    cout << "Press Esc to go back to Main Menu OR Press Enter to Desposit amount." << endl;

    char ch ;

    while (true) {
        if (_kbhit()) {
            ch = _getch();
            break;
        }
    }
   
       
    if(ch==13)
    {

       borrowBooks(studentnode, tail);
    }
    else if (ch == 27) {
        cout << endl;
       cout << "Back to Main Menu Selected" << endl;
       student(studentnode, tail);
    }
    


}


void returnBooks(node*& studentnode, node*& tail) {
    if (studentnode == nullptr) {
        cout << "Error: No student account found.\n";
        return;
    }

    string bookname;
    cout << "Enter the name of the book you want to return: ";
    cin >> bookname;

    // Search for the book in the book list
    booknode* temp = bookhead;
    while (temp != nullptr) {
        if (temp->username == studentnode->username && temp->bookname == bookname) {
            // Increment the quantity of the returned book
            booknode* libraryBook = bookhead;
            while (libraryBook != nullptr) {
                if (libraryBook->bookname == bookname && libraryBook->username == "Library") {
                    libraryBook->quantity++;
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
            return;
        }
        temp = temp->next;
    }

    cout << "You have not borrowed this book.\n";
    student(studentnode, tail);
}


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
    student(studentnode, tail);
}

// Main function
int main() {
    node* head = new node("admin", adminpass);
    node* tail = head;
    

    registerstudent(head, tail);

    return 0;
}


