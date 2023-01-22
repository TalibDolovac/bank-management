#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <windows.h>
#include <regex>
#include <stdlib.h>
#include <cstdio>
using namespace std;

// Structure to store user information
struct User {
  string name;
  string email;
  string password;
  int balance = 0;
};

// Function prototypes
void registerUser();
void login();
bool checkCredentials(string email, string password);
bool validateEmail(string email);
void withdrawMoney(string email, string password);
void transactionMoney(string email, string password);
void checkBalance(string email, string password);
void displayAccounts();
void closeAccount(string email, string password);

int main() {
  cout << "Welcome to the bank management system." << endl;
  cout << "1. Register" << endl;
  cout << "2. Login" << endl;
  cout << "3. Exit." << endl;

  int choice;
  cin >> choice;

  if (choice == 1) {
  	cout << "If you register you will get 100$ free." << endl;
    registerUser();
  } else if (choice == 2) {
    login();
  } else if (choice == 3) {
  	exit(3);
  	cout << "Bye!";
  } 
  else {
    cout << "Invalid choice." << endl;
  }

  return 0;
}

// Function to register a new user
void registerUser() {
  User user;
  user.balance += 100;
			
  cout << "Enter your name: ";
  cin >> user.name;

   while (true) {
    cout << "Enter your email: ";
    cin >> user.email;

    if (validateEmail(user.email)) {
      // If the email is valid, break out of the loop
      break;
    }

    cout << "Invalid email. Please try again." << endl;
  }
  
    while (true) {
	    cout << "Enter your password: ";
	    cin >> user.password;
	
	    // Check if the name and password are the same
	    if (user.name != user.password) {
	      // If the name and password are not the same, break out of the loop
	      break;
    }

    cout << "Error: Name and password cannot be the same. Please try again." << endl;
  }

  // Open the file in append mode
  ofstream outfile;
  outfile.open("users.txt", ios::app);

  // Write the user data to the file
  outfile << user.name << "," << user.email << "," << user.password << "," << user.balance << endl;

  // Close the file
  outfile.close();

  cout << "Successfully registered user." << endl;
}

// Function to login an existing user
void login() {
  string email;
  string password;

    while (true) {
	    cout << "Enter your email: ";
	    cin >> email;
	
	    if (validateEmail(email)) {
	      // If the email is valid, break out of the loop
	      break;
	    }
	
	    cout << "Invalid email. Please try again." << endl;
  	}

  cout << "Enter your password: ";
  cin >> password;

  if (checkCredentials(email, password)) {
  	int izbor;
    system("CLS");
	cout << "Successful login." << endl;
	
	cout << "1. Withdraw Money." << endl;
  cout << "2. Transaction Money." << endl;
  cout << "3. Check Balance." << endl;
  cout << "4. Display Accounts." << endl;
  cout << "5. Close Account." << endl;
	cout << "6. Exit." << endl;
	cout << "?:" << endl;
	cin >> izbor;
	
	switch(izbor) {
		case 1:
			withdrawMoney(email, password);
			break;
    case 2: 
      transactionMoney(email, password);
      break;
    case 3:
			checkBalance(email, password);
			break;
		case 4:
			displayAccounts();
			break;
		case 5:
			closeAccount(email, password);
			break;
		case 6:
			exit(6);
			break;
		default:
      cout << "Invalid choice." << endl;
      break;
	} 
  } else {
    cout << "Unsuccessful login." << endl;
  }
}

// Function to check the user's login credentials
bool checkCredentials(string email, string password) {
  // Open the file for reading
  ifstream infile;
  infile.open("users.txt");

  // Read the file line by line
  string line;
  while (getline(infile, line)) {
    // Split the line into the different fields
    string fields[4];
    int i = 0;
    string field;
    stringstream ss(line);
    while (getline(ss, field, ',')) {
      fields[i] = field;
      i++;
    }

    // Check if the email and password match
    if (fields[1] == email && fields[2] == password) {
      return true;
    }
  }

  // Close the file
  infile.close();

  return false;
}

bool validateEmail(string email) {
  // Use a regular expression to check for a valid email format
  regex email_regex("[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}");
  if (!regex_match(email, email_regex)) {
    return false;
  }
  return true;
}

// Function to withdraw money
void withdrawMoney(string email, string password) {
  int amount;
  withdrawAgain:
  cout << "Enter the amount you want to withdraw: ";
  cin >> amount;

  // Open the file for reading
  ifstream infile;
  infile.open("users.txt");

  // Read the file line by line
  string line;
  while (getline(infile, line)) {
    // Split the line into the different fields
    string fields[4];
    int i = 0;
    stringstream ss(line);
    while (getline(ss, fields[i], ',')) {
      i++;
    }

    // Check if the email and password match
    if (fields[1] == email && fields[2] == password) {
      int balance = stoi(fields[3]);
      if (amount > balance) {
        cout << "You don't have enough money to withdraw that amount." << endl;
        goto withdrawAgain;
        infile.close();
        return;
      } else {
        balance -= amount;
        cout << "Successfully withdrawn " << amount << "$" << endl;
        cout << "Your new balance is " << balance << "$" << endl;

        // Open the file for writing
        fstream outfile;
        outfile.open("users.txt");

        // Read the file again and update the user's balance
        ifstream tempfile;
        tempfile.open("users.txt");
        while (getline(tempfile, line)) {
          string fields[4];
          int i = 0;
          stringstream ss(line);
          while (getline(ss, fields[i], ',')) {
            i++;
          }

          if (fields[1] == email && fields[2] == password) {
            outfile << endl << fields[0] << "," << fields[1] << "," << fields[2] << "," << balance << endl;
          } else {
            outfile << line << endl;
          }
        }

//         Close the files
        outfile.close();
        tempfile.close();
        infile.close();
        return;
      }
    }
  }
  
  infile.close();
  cout << "Error: User not found." << endl;
}

void transactionMoney(string email, string password) {
  // Open the file to read user data
  ifstream infile;
  infile.open("users.txt");

  string line;
  string name, email_from_file, password_from_file;
  int balance;
  bool found = false;

  // Read the file line by line
  while (getline(infile, line)) {
    // Use a stringstream to extract the data from the line
    stringstream ss(line);

    // Use getline() to extract the data from the stringstream
    getline(ss, name, ',');
    getline(ss, email_from_file, ',');
    getline(ss, password_from_file, ',');
    ss >> balance;

    if (email == email_from_file && password == password_from_file) {
      // User is found
      found = true;
      break;
    }
  }

  // Close the file
  infile.close();

  if (found) {
    // User is found, proceed with transaction
    int amount;
    string email_to;

    cout << "Enter the email of the user you want to transaction to: ";
    cin >> email_to;

    cout << "Enter the amount you want to transaction: ";
    cin >> amount;

    if (amount > balance) {
      cout << "Error: Insufficient funds." << endl;
    } else {
      // Open the file to update user data
      fstream file;
      file.open("users.txt", ios::in);

      stringstream ss;
      string line_from_file;

      while (getline(file, line_from_file)) {
        stringstream line_ss(line_from_file);

        getline(line_ss, name, ',');
        getline(line_ss, email_from_file, ',');
        getline(line_ss, password_from_file, ',');
        line_ss >> balance;

        if (email == email_from_file && password == password_from_file) {
          // Update the balance of the current user
          balance -= amount;
        }

        if (email_to == email_from_file) {
          // Update the balance of the user to transaction to
          balance += amount;
        }

        // Write the updated data to the stringstream
        ss << name << "," << email_from_file << "," << password_from_file << "," << balance << endl;
      }

      // Close the file
      file.close();

      // Open the file to write the updated data
      ofstream outfile;
      outfile.open("users.txt");

      // Write the data from the stringstream to the file
      outfile << ss.str();

      // Close the file
      outfile.close();

      cout << "Transaction successful." << endl;
    }
  } else {
    // User is not found
    cout << "Error: User not found." << endl;
  }
}

void checkBalance(string email, string password) {
  // Open the file in read mode
  ifstream infile;
  infile.open("users.txt");

  // Read the file line by line
  string line;
  while (getline(infile, line)) {
    string name, email_, password_, balance;
    stringstream ss(line);
    getline(ss, name, ',');
    getline(ss, email_, ',');
    getline(ss, password_, ',');
    getline(ss, balance, ',');

    // Check if the email and password match
    if (email_ == email && password_ == password) {
      // Print the balance
      cout << "Your balance is: $" << balance << endl;
      return;
    }
  }

  // Close the file
  infile.close();

  cout << "Invalid email or password." << endl;
}

void displayAccounts() {
    ifstream infile;
    infile.open("users.txt");

    cout << "Username" << "\t" << "Email" << "\t" << "Balance" << endl;
    cout << "-------------------------------------------------" << endl;

    string line;
    while (getline(infile, line)) {
        stringstream ss(line);
        string name, email, password, balance;
        getline(ss, name, ',');
        getline(ss, email, ',');
        getline(ss, password, ',');
        getline(ss, balance);

        cout << name << "\t" << email << "\t" << balance << endl;
    }
    infile.close();
}

void closeAccount(string email, string password) {
    // check if the provided email and password match the ones on file
    if (!checkCredentials(email, password)) {
        cout << "Error: Invalid email or password." << endl;
        return;
    }

    // temp file to store the valid users
    ofstream temp;
    temp.open("temp.txt");

    // read the "users.txt" file line by line
    ifstream infile;
    infile.open("users.txt");
    string line;
    while (getline(infile, line)) {
        stringstream ss(line);
        string name, email_, password_, balance;
        getline(ss, name, ',');
        getline(ss, email_, ',');
        getline(ss, password_, ',');
        getline(ss, balance);

        // if the email is not the one being deleted, write to the temp file
        if (email != email_) {
            temp << line << endl;
        }
    }
    infile.close();
    temp.close();

    // remove the original "users.txt" file
    remove("users.txt");

    // rename the temp file to "users.txt"
    rename("temp.txt", "users.txt");
    cout << "Successfully closed account" << endl;
    //log out
    exit(3);
}

