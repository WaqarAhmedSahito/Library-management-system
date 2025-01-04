
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;



// Variables
string studName;
string idNum;
string studentPassword;
string adminPass = "1234";
bool foundAcc = false;


//Linked Node
class Node {
public:
    string data;
    Node* next;
    Node* prev;
    int balance;

    Node(string data) {
        this->data = data;
        this->next = NULL;
        this->prev = NULL; 
        this->balance = 0;
    }
};


// prototypes
//void login_student(Node*& head);
void login_admin();
void admin();
void loginStudentUpdated(Node* &head);
void DepositMoney(Node* studentNode);
void inserthead(Node*& head, string data);




void inserthead (Node*& head, string data) {
    Node* temp = new Node(data);
    temp->next = head;
    if (head != NULL) {
        head->prev = temp;
    }
    head = temp;
}

void insertend(Node*& tail, string data) {
    Node* temp = new Node(data);
    tail->next = temp;
    temp->prev = tail;
    tail = temp;
}

void insert(Node*& head, int K, string data) {
    Node* temp = head;
    int cnt = 1;
    while (cnt < K - 1) {
        temp = temp->next;
        cnt++;
    }
    Node* Nodenew = new Node(data);
    Nodenew->next = temp->next;
    if (temp->next != NULL) {
        temp->next->prev = Nodenew;
    }
    temp->next = Nodenew;
    Nodenew->prev = temp;
}

void del(Node*& head, int k) {
    Node* temp = head;
    int cnt = 1;

    if (k == 1) {
        head = head->next;
        if (head != NULL) {
            head->prev = NULL;
        }
        delete temp;
        return;
    }

    while (cnt < k) {
        temp = temp->next;
        cnt++;
    }
    temp->prev->next = temp->next;
    if (temp->next != NULL) {
        temp->next->prev = temp->prev;
    }
    delete temp;
}

void search(Node*& head, string data, bool isFound) {
    Node* temp = head;
    int cnt = 1;
    while (temp != NULL) {
        if (temp->data == data) {
            
           
            isFound = true;
            foundAcc = isFound;
            studentPassword = temp->prev->data;
            return;
        }

        temp = temp->next;
        cnt++;
    }
    cout << "User not found :( " << endl;
}





void traverse(Node*& head) {
    Node* temp = head;

    while (temp != NULL) {
        cout << temp->data << " ";
        temp = temp->next;
    }
    cout << endl;
}

void GenidNum() {

    idNum = "";

    char idnumarr[10] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

    for (int i = 0; i <= 4; i++) {
        int randindex = rand() % 10;
        idNum += idnumarr[randindex];
    }
    cout << "ID number: " << idNum << endl;
}

void AdminAccount(Node*& head, Node*& tail) {
    insertend(tail, adminPass);
}


void RegisterStudent(Node* &head, Node* &tail) {
    int cont = 0;
    while (cont == 0)
    {
        cout << "1) Register Account " << endl;
        cout << "2) Login Existing Account " << endl;
        cout << "3) Login as Admin " << endl;
        cout << "4) Exit " << endl;


        cout << "Enter Choice: ";
        int loginChoice;
        cin >> loginChoice;

        switch (loginChoice) {
        case 1: {
            cout << "Assign Username: ";
            cin >> studName;
            cout << endl;

            cout << "Assign Password: ";
            cin >> studentPassword;
            cout << endl;

            GenidNum();

            Node* studentNode = new Node(studentPassword);
            studentNode->prev = tail;
            tail->next = studentNode;
            tail = studentNode;

            studentNode->balance = 100; // Assign default balance.
            insertend(tail, studName);
            insertend(tail, idNum);

            insertend(tail, studentPassword);
            insertend(tail, studName);
            insertend(tail, idNum);
            

            break;
        }
        case 2: {
            //login_student(head);
            loginStudentUpdated(head);
            break;
        }
        case 3: {
            login_admin();
            break;
        }
        default:
            cout << "Invalid choice.\n";
            break;
        }


        cout << "Continue? 0=yes 1=no";
        cin >> cont;


    }
}







// waqar ahmed
void login_admin() {
    string username, password;
    cout << "Enter Admin Username: ";
    cin >> username;
    cout << "Enter Admin Password: ";
    cin >> password;

    if (username == "admin" && password == adminPass) {
        cout << "Admin logged in successfully.\n";
        admin();
    }
    else {
        cout << "Invalid Admin credentials.\n";
    }
}

void student(Node* studentNode) {
    cout << "\nStudent options:\n1. View books\n2. Borrow book\n3. Return book\n4. Deposit Money\n5.Exit\n";
    int option;
    cin >> option;

    switch (option) {
    case 1: {
        cout << "View Books selected.\n";
        return;
    }
    case 2: {
        cout << "Borrow Book selected.\n";
        return;
    }
    case 3: {
        cout << "Return Book selected.\n";
        return;
    }
    case 4: {
        if (studentNode != nullptr) {
            DepositMoney(studentNode); 
        }
        else {
            cout << "Error: Invalid student node.\n";
        }
        break;
    }
    case 5: {
        cout << "Exiting Student Menu.\n";
        return;
    }
    default:
        cout << "Invalid option. Returning to main menu.\n";
        return;
    }
}




//Updated student login it would be better to update this one if youre ok with it
void loginStudentUpdated(Node*& head) {
    string inputPass;
    string inputId;

    cout << "Enter Username: ";
    cin >> inputId;

    Node* temp = head;
    while (temp != NULL) {
        if (temp->data == inputId) {
            int trycnt = 0; // Counter for password attempts
            const int maxAttempts = 3;

            while (trycnt < maxAttempts) {
                cout << "Enter Password: ";
                cin >> inputPass;

                if (inputPass == studentPassword) {
                    cout << "Welcome " << inputId << ", you are now logged in as a student." << endl;
                    student(temp); // Pass the Node* to the student function.
                    return;
                }
                else {
                    trycnt++;
                    cout << "Invalid password. Attempt " << trycnt << " of " << maxAttempts << "." << endl;

                    if (trycnt >= maxAttempts) {
                        cout << "Maximum login attempts reached. Returning to the main menu.\n";
                        return;
                    }
                }
            }
        }
        temp = temp->next;
    }

    cout << "Student not found.\n";
}




//this is the old module
//void login_student(Node*& head) {
//    cout << "Enter Student Username: ";
//    string username;
//    cin >> username;
//
//    cout << "Enter Password: ";
//    string password;
//    cin.ignore(); 
//
//    Node* temp = head;
//    while (temp != NULL) {
//        if (temp->data == username) {
//            cout << "Welcome, " << username << ". You are now logged in as a student.\n";
//            student();
//            return;
//        }
//        temp = temp->next;
//    }
//
//    cout << "Student not found or invalid credentials.\n";
//}



void admin() {
    cout << "\nAdmin options:\n1. Add book\n2. Edit book\n3. View books\n4. Exit\n";
    int option;
    cin >> option;

    switch (option) {
    case 1: {
        cout << "Add Book selected.\n";
        return;
    }
    case 2: {
        cout << "Edit Book selected.\n";
        return;
    }
    case 3: {
        cout << "View Books selected.\n";
        return;
    }
    case 4: {
        cout << "Exiting Admin Menu.\n";
        return;
    }
    default:
        cout << "Invalid option. Returning to main menu.\n";
        return;
    }
}

//Hasan

 void DepositMoney(Node* studentNode) {
    if (studentNode == nullptr) {
        cout << "Error: No student account found.\n";
        return;
    }

    cout << "Your current balance is: $" << studentNode->balance << endl;

    int depositAmount;
    cout << "Enter the amount you wish to deposit: ";
    cin >> depositAmount;

    if (depositAmount > 0) {
        studentNode->balance += depositAmount;
        cout << "Deposit successful. Your new balance is: $" << studentNode->balance << endl;
    }
    else {
        cout << "Invalid deposit amount. Try again.\n";
    }
}



int main() {
    Node* head = new Node("Admin");
    Node* tail = head;
    AdminAccount(head, tail);

    RegisterStudent(head, tail);




    return 0;
}