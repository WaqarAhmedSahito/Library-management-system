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
void borrowBooks(node*& studentnode);
void returnBooks(node*& studentnode);
void viewBorrowedBooks(node*& studentnode);

// Function definitions

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
        viewBorrowedBooks(studentnode);
        break;
    }
    case 2: {
        borrowBooks(studentnode);
        break;
    }
    case 3: {
        returnBooks(studentnode);
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
    cin >> option;

    switch (option) {
    case 1: {
        cout << "Add book selected.\n";
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


// Borrow books
void borrowBooks(node*& studentnode) {
    if (studentnode == nullptr) {
        cout << "Error: No student account found.\n";
        return;
    }

    string bookname;
    cout << "Book is $20 each\n";
    cout << "Enter the name of the book you want to borrow: ";
    cin >> bookname;

    // Check if the student has already borrowed the book
    booknode* temp = bookhead;
    while (temp != nullptr) {
        if (temp->username == studentnode->username && temp->bookname == bookname) {
            cout << "You have already borrowed this book.\n";
            return;
        }
        temp = temp->next;
    }

    // Create a new booknode for the borrowed book
    booknode* newBook = new booknode(studentnode->username, bookname);

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
    studentnode->balance -= 20;
    cout << "Book '" << bookname << "' borrowed successfully!\nyour new balance is: "<<studentnode->balance<<endl;
}


// Return books
void returnBooks(node*& studentnode) {
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
}

// View borrowed books
void viewBorrowedBooks(node*& studentnode) {
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
}

// Main function
int main() {
    node* head = new node("admin", adminpass);
    node* tail = head;
   

    registerstudent(head, tail);

    return 0;
}




//
//#include <iostream>
//#include <string>
//#include <cstdlib>
//#include <conio.h>
//
//
//using namespace std;
//
//
//
//// variables
//string studname;
//string idnum;
//string studentpassword;
//string adminpass = "1234";
//bool foundacc = false;
//
//
////linked node
//class node {
//public:
//    string data;
//    node* next;
//    node* prev;
//    int balance;
//
//    node(string data) {
//        this->data = data;
//        this->next = null;
//        this->prev = null; 
//        this->balance = 0;
//    }
//};
//
//
//// prototypes
//void login_student(node*& head);
//void login_admin(node* &head);
//void admin(node* &head);
//void loginstudentupdated(node* &head);
//void depositmoney(node* studentnode);
//void inserthead(node*& head, string data);
//
//
//
//
//void inserthead (node*& head, string data) {
//    node* temp = new node(data);
//    temp->next = head;
//    if (head != null) {
//        head->prev = temp;
//    }
//    head = temp;
//}
//
//void insertend(node*& tail, string data) {
//    node* temp = new node(data);
//    tail->next = temp;
//    temp->prev = tail;
//    tail = temp;
//}
//
//void insert(node*& head, int k, string data) {
//    node* temp = head;
//    int cnt = 1;
//    while (cnt < k - 1) {
//        temp = temp->next;
//        cnt++;
//    }
//    node* nodenew = new node(data);
//    nodenew->next = temp->next;
//    if (temp->next != null) {
//        temp->next->prev = nodenew;
//    }
//    temp->next = nodenew;
//    nodenew->prev = temp;
//}
//
//void del(node*& head, int k) {
//    node* temp = head;
//    int cnt = 1;
//
//    if (k == 1) {
//        head = head->next;
//        if (head != null) {
//            head->prev = null;
//        }
//        delete temp;
//        return;
//    }
//
//    while (cnt < k) {
//        temp = temp->next;
//        cnt++;
//    }
//    temp->prev->next = temp->next;
//    if (temp->next != null) {
//        temp->next->prev = temp->prev;
//    }
//    delete temp;
//}
//
//
//
//
//
//
//
//void traverse(node*& head) {
//    node* temp = head;
//
//    while (temp != null) {
//        cout << temp->data << " ";
//        temp = temp->next;
//    }
//    cout << endl;
//}
//
//
//
//
//void genidnum() {
//
//    idnum = "";
//
//    char idnumarr[10] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
//
//    for (int i = 0; i <= 4; i++) {
//        int randindex = rand() % 10;
//        idnum += idnumarr[randindex];
//    }
//    cout << "id number: " << idnum << endl;
//}
//
//void adminaccount(node*& head, node*& tail) {
//    insertend(tail, adminpass);
//}
//
//
//void genstudlist(node* &head) {
//    traverse(head);
//
//}
//
//
//
//
//
//
//
//void registerstudent(node* &head, node* &tail) {
//    int cont = 0;
//    while (cont == 0)
//    {
//        cout << "1) register account " << endl;
//        cout << "2) login existing account " << endl;
//        cout << "3) login as admin " << endl;
//        cout << "4) exit " << endl;
//        cout << endl;
//
//        cout << "enter choice: ";
//        int loginchoice;
//        cin >> loginchoice;
//
//        switch (loginchoice) {
//        case 1: {
//            bool nameexist = false;
//            int attempts = 0;
//            int maxattempts = 5;
//            
//            while (attempts < maxattempts)
//            {
//                foundacc = false;
//                studname = "";
//                cout << "assign username: ";
//                cin >> studname;
//
//                
//
//                search(head, studname, foundacc);
//                
//                if (foundacc == false) {
//                    break;
//                }
//                else if (foundacc == true) {
//                    cout << "this username already exists!" << endl;
//                    cout << "press \"enter\" to try again or \"esc\" to proceed to main menu" << endl;
//
//                  
//                    char ch;
//                    while (true) {
//                        if (_kbhit()) { 
//                            ch = _getch(); 
//                            break;
//                        }
//                    }
//
//                    if (ch == 13) { 
//                        std::cout << "enter pressed. trying again." << std::endl;
//                        continue;
//                    }
//                    else if (ch == 27) { 
//                        std::cout << "esc pressed. returning to main menu." << std::endl;
//                        registerstudent(head, tail);
//                        return; 
//                    }
//                }
//            }
//
//
//            cout << "assign password: ";
//            cin >> studentpassword;
//            
//
//            genidnum();
//
//            node* studentnode = new node(studentpassword);
//            studentnode->prev = tail;
//            tail->next = studentnode;
//            tail = studentnode;
//
//            studentnode->balance = 100; 
//            
//
//            insertend(tail, studentpassword);
//            insertend(tail, studname);
//            insertend(tail, idnum);
//            
//
//            break;
//        }
//        case 2: {
//            
//            loginstudentupdated(head);
//            break;
//        }
//        case 3: {
//            login_admin(head);
//            break;
//        }
//        default:
//            cout << "invalid choice.\n";
//            break;
//        }
//
//
//        cout << "continue? 0=yes 1=no: ";
//        cin >> cont;
//        cout << endl;
//
//
//    }
//}
//
//
//
//
//
//
//
//// waqar ahmed
//void login_admin(node* &head) {
//    string username, password;
//    cout << "enter admin username: ";
//    cin >> username;
//    cout << "enter admin password: ";
//    cin >> password;
//
//    if (username == "admin" && password == adminpass) {
//        cout << "admin logged in successfully.\n";
//        admin(head);
//    }
//    else {
//        cout << "invalid admin credentials.\n";
//    }
//}
//
//void student(node* studentnode) {
//    cout << "\nstudent options:\n1. view books\n2. borrow book\n3. return book\n4. deposit money\n5.exit\n";
//    int option;
//    cin >> option;
//
//    switch (option) {
//    case 1: {
//        cout << "view books selected.\n";
//        return;
//    }
//    case 2: {
//        cout << "borrow book selected.\n";
//        return;
//    }
//    case 3: {
//        cout << "return book selected.\n";
//        return;
//    }
//    case 4: {
//        if (studentnode != nullptr) {
//            depositmoney(studentnode); 
//        }
//        else {
//            cout << "error: invalid student node.\n";
//        }
//        break;
//    }
//    case 5: {
//        cout << "exiting student menu.\n";
//        return;
//    }
//    default:
//        cout << "invalid option. returning to main menu.\n";
//        return;
//    }
//}
//
//
//
//
////updated student login 
//void loginstudentupdated(node*& head) {
//    string inputpass;
//    string inputid;
//
//    cout << "enter username: ";
//    cin >> inputid;
//
//    node* temp = head;
//    while (temp != null) {
//        if (temp->data == inputid) {
//            int trycnt = 0; // counter for password attempts
//            const int maxattempts = 3;
//
//            while (trycnt < maxattempts) {
//                cout << "enter password: ";
//                cin >> inputpass;
//
//                if (inputpass == studentpassword) {
//                    cout << "welcome " << inputid << ", you are now logged in as a student." << endl;
//                    student(temp); // passed node* to the student function.
//                    return;
//                }
//                else {
//                    trycnt++;
//                    cout << "invalid password. attempt " << trycnt << " of " << maxattempts << "." << endl;
//
//                    if (trycnt >= maxattempts) {
//                        cout << "maximum login attempts reached. returning to the main menu.\n";
//                       
//                        return;
//                    }
//                }
//            }
//        }
//        temp = temp->next;
//    }
//    cout << endl;
//    cout << "user not found :(.\n";
//}
//
//
//
//
//
//
//void admin(node* &head) {
//    cout << "\nadmin options:\n1. add book\n2. edit book\n3. view books\n4. exit\n5. generate users list\n";
//    int option;
//    cin >> option;
//
//    switch (option) {
//    case 1: {
//        cout << "add book selected.\n";
//        break;
//    }
//    case 2: {
//        cout << "edit book selected.\n";
//        break;
//    }
//    case 3: {
//        cout << "view books selected.\n";
//        break;
//    }
//    case 4: {
//        cout << "exiting admin menu....\n";
//        break;
//    }
//    case 5: {
//        cout << "generating list of user....\n";
//        genstudlist(head);
//        break;
//    }
//    default:
//        cout << "invalid option. returning to main menu.\n";
//        return;
//    }
//}
//
////hasan
//
// void depositmoney(node* studentnode) {
//    if (studentnode == nullptr) {
//        cout << "error: no student account found.\n";
//        return;
//    }
//
//    cout << "your current balance is: $" << studentnode->balance << endl;
//
//    int depositamount;
//    cout << "enter the amount you wish to deposit: ";
//    cin >> depositamount;
//
//    if (depositamount > 0) {
//        studentnode->balance += depositamount;
//        cout << "deposit successful. your new balance is: $" << studentnode->balance << endl;
//    }
//    else {
//        cout << "invalid deposit amount. try again.\n";
//    }
//}
//
//
//
//int main() {
//    node* head = new node("admin");
//    node* tail = head;
//    adminaccount(head, tail);
//
//    registerstudent(head, tail);
//
//
//
//
//    return 0;
//}