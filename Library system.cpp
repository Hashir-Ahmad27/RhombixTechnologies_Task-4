#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// -------------------- BASE CLASS --------------------
class Book {
protected:
    int bookID;
    string title;
    string author;
    bool isIssued;

public:
    Book() {}

    Book(int id, string t, string a, bool issued = false) {
        bookID = id;
        title = t;
        author = a;
        isIssued = issued;
    }

    virtual void display() {
        cout << "ID: " << bookID
             << " | Title: " << title
             << " | Author: " << author
             << " | Status: " << (isIssued ? "Issued" : "Available") << endl;
    }

    int getID() { return bookID; }
    bool getStatus() { return isIssued; }
    void setStatus(bool status) { isIssued = status; }

    string toFile() {
        return to_string(bookID) + "|" + title + "|" + author + "|" + to_string(isIssued);
    }
};

// -------------------- ADMIN CLASS --------------------
class Admin : public Book {
public:
    void addBook() {
        ofstream file("books.txt", ios::app);
        int id;
        string t, a;

        cout << "Enter Book ID: ";
        cin >> id;
        cin.ignore();

        cout << "Enter Title: ";
        getline(cin, t);

        cout << "Enter Author: ";
        getline(cin, a);

        file << id << "|" << t << "|" << a << "|0\n";
        file.close();

        cout << "Book added successfully!\n";
    }

    void deleteBook() {
        ifstream in("books.txt");
        ofstream out("temp.txt");

        int id;
        cout << "Enter Book ID to delete: ";
        cin >> id;

        string line;
        bool found = false;

        while (getline(in, line)) {
            if (stoi(line.substr(0, line.find('|'))) != id)
                out << line << endl;
            else
                found = true;
        }

        in.close();
        out.close();

        remove("books.txt");
        rename("temp.txt", "books.txt");

        cout << (found ? "Book deleted.\n" : "Book not found.\n");
    }

    void viewBooks() {
        ifstream file("books.txt");
        string line;

        cout << "\n--- Book List ---\n";
        while (getline(file, line)) {
            int id = stoi(line.substr(0, line.find('|')));
            line.erase(0, line.find('|') + 1);

            string title = line.substr(0, line.find('|'));
            line.erase(0, line.find('|') + 1);

            string author = line.substr(0, line.find('|'));
            line.erase(0, line.find('|') + 1);

            bool issued = stoi(line);
            Book b(id, title, author, issued);
            b.display();
        }
        file.close();
    }
};

// -------------------- USER CLASS --------------------
class User {
public:
    void searchBook() {
        ifstream file("books.txt");
        string keyword, line;

        cin.ignore();
        cout << "Enter title or author keyword: ";
        getline(cin, keyword);

        cout << "\nSearch Results:\n";
        while (getline(file, line)) {
            if (line.find(keyword) != string::npos)
                cout << line << endl;
        }
        file.close();
    }

    void issueBook() {
        int id;
        cout << "Enter Book ID to issue: ";
        cin >> id;

        ifstream in("books.txt");
        ofstream out("temp.txt");

        string line;
        bool issued = false;

        while (getline(in, line)) {
            int bid = stoi(line.substr(0, line.find('|')));

            if (bid == id && line.back() == '0') {
                line.back() = '1';
                issued = true;
            }
            out << line << endl;
        }

        in.close();
        out.close();

        remove("books.txt");
        rename("temp.txt", "books.txt");

        cout << (issued ? "Book issued successfully!\n" : "Book unavailable.\n");
    }

    void returnBook() {
        int id;
        cout << "Enter Book ID to return: ";
        cin >> id;

        ifstream in("books.txt");
        ofstream out("temp.txt");

        string line;
        bool returned = false;

        while (getline(in, line)) {
            int bid = stoi(line.substr(0, line.find('|')));

            if (bid == id && line.back() == '1') {
                line.back() = '0';
                returned = true;
            }
            out << line << endl;
        }

        in.close();
        out.close();

        remove("books.txt");
        rename("temp.txt", "books.txt");

        cout << (returned ? "Book returned successfully!\n" : "Invalid return.\n");
    }
};

// -------------------- MAIN FUNCTION --------------------
int main() {
    Admin admin;
    User user;
    int choice;

    do {
        cout << "\n===== Library Management System =====\n";
        cout << "1. Admin Menu\n";
        cout << "2. User Menu\n";
        cout << "0. Exit\n";
        cout << "Choice: ";
        cin >> choice;

        if (choice == 1) {
            int a;
            cout << "\n1. Add Book\n2. Delete Book\n3. View Books\nChoice: ";
            cin >> a;

            if (a == 1) admin.addBook();
            else if (a == 2) admin.deleteBook();
            else if (a == 3) admin.viewBooks();
        }

        else if (choice == 2) {
            int u;
            cout << "\n1. Search Book\n2. Issue Book\n3. Return Book\nChoice: ";
            cin >> u;

            if (u == 1) user.searchBook();
            else if (u == 2) user.issueBook();
            else if (u == 3) user.returnBook();
        }

    } while (choice != 0);

    cout << "Exiting system...\n";
    return 0;
}
