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
//void transactionMoney(string email, string password); uskoro 
void checkBalance(string email, string password);
void displayAccounts();
void closeAccount(string email, string password);
void modifyAccount(string email, string password);

int main() {
	cout << "--------------------------------------" << endl;
  cout << "Welcome to the bank management system." << endl;
  cout << "1. Register." << endl;
  cout << "2. Login." << endl;
  cout << "3. Exit." << endl;
	
	cout << "Select your option (1-3): ";
  int choice;
  cin >> choice;

  if (choice == 1) {
  	cout << "If you register you will get 100$ free." << endl;
    registerUser();
  } else if (choice == 2) {
    login();
  } else if (choice == 3) {
  	cout << "Bye!";
  	exit(3);
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
  cout << "2. Check Balance." << endl;
  cout << "3. Modify Account." << endl;
  cout << "4. Display Accounts." << endl;
  cout << "5. Close Account." << endl;
	cout << "6. Exit." << endl;
	cout << "Enter your choice: (1-6): ";
	cin >> izbor;
	
	switch(izbor) {
		case 1:
			withdrawMoney(email, password);
			break;
    case 2:
			checkBalance(email, password);
			break;
		case 3:
			modifyAccount(email, password);
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
    User user;
    string line, name, emailAddress, pass, balance;
    int amount;
    bool found = false;

    // Open the file in read mode
    ifstream infile;
    infile.open("users.txt");

    // Read the file line by line
    while (getline(infile, line)) {
        // Create a stringstream from the line
        stringstream ss(line);

        // Extract the user data from the line
        getline(ss, name, ',');
        getline(ss, emailAddress, ',');
        getline(ss, pass, ',');
        getline(ss, balance, ',');

        // Check if the email and password match
        if (emailAddress == email && pass == password) {
            found = true;
            user.name = name;
            user.email = email;
            user.password = pass;
            user.balance = stoi(balance);
            break;
        }
    }

    // Close the file
    infile.close();

    if (found) {
        while (true) {
            cout << "Enter amount to withdraw: ";
            cin >> amount;
            if (amount > user.balance) {
                cout << "Error: Insufficient funds. Current balance is " << user.balance << "$" << endl;
            }
            else {
                break;
            }
        }
        user.balance -= amount;

        cout << "Successfully withdrew " << amount << "$. Current balance is " << user.balance << "$" << endl;

        // Open the file in read mode
        fstream file;
        file.open("users.txt", ios::in);
        string data;

        // Read the file line by line
        while (getline(file, line)) {
            // Create a stringstream from the line
            stringstream ss(line);

            // Extract the user data from the line
            getline(ss, name, ',');
            getline(ss, emailAddress, ',');
            getline(ss, pass, ',');
            getline(ss, balance, ',');

            // Check if the email and password match
            if (emailAddress == email && pass == password) {
                data += name + "," + emailAddress + "," + pass + "," + to_string(user.balance) + "\n";
            } else {
                data += line + "\n";
            }
        }
        file.close();

        // Open the file in write mode
        file.open("users.txt", ios::out);

        // Write the updated data to the file
        file << data;

        // Close the file
        file.close();
    } else {
        cout << "Error: User not found." << endl;
    }
}

void transactionMoney(string email, string password) {
  string email_to;
  while(true) {
    cout << "Enter email of person you want to transfer money: ";
    cin >> email_to;
    if(validateEmail(email_to)) {
      break;
    } else {
      cout << "Try again.";
    }
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

void modifyAccount(string email, string password) {
	system("CLS");
  // Check if the user is logged in
  if (!checkCredentials(email, password)) {
    cout << "Error: Incorrect email or password." << endl;
    return;
  }

  // Open the file in input mode
  ifstream infile("users.txt");
  string line;
  vector<string> lines;

  // Read the file line by line
  while (getline(infile, line)) {
    // Check if the email and password match
    if (line.find(email) != string::npos && line.find(password) != string::npos) {
      // Ask the user what they want to change
      int choice;
      cout << "What would you like to change?" << endl;
      cout << "1. Email" << endl;
      cout << "2. Username" << endl;
      cout << "3. Password" << endl;
      cout << "Enter your choice (1-3): ";
      cin >> choice;

      if (choice == 1) {
        // Get the new email
        string newEmail;
        cout << "Enter your new email: ";
        cin >> newEmail;
        // validate email
        if (validateEmail(newEmail)) {
          // update the email in the line
          stringstream ss(line);
          string name, email, password, balance;
          getline(ss, name, ',');
          getline(ss, email, ',');
          getline(ss, password, ',');
          getline(ss, balance, ',');
          line = name + "," + newEmail + "," + password + "," + balance;
        } else {
          cout << "Invalid email. Please try again." << endl;
          return;
        }
      } else if (choice == 2) {
        // Get the new username
        string newName;
        cout << "Enter your new name: ";
        cin >> newName;
        // update the name in the line
        stringstream ss(line);
        string name, email, password, balance;
        getline(ss, name, ',');
        getline(ss, email, ',');
        getline(ss, password, ',');
        getline(ss, balance, ',');
        line = newName + "," + email + "," + password + "," + balance;
      } else if (choice == 3) {
        // Get the new password
        string newPassword;
        cout << "Enter your new password: ";
        cin >> newPassword;
        //update password in the line
        stringstream ss(line);
        string name, email, password, balance;
        getline(ss, name, ',');
        getline(ss, email, ',');
        getline(ss, password, ',');
        getline(ss, balance, ',');
        line = name + "," + email + "," + newPassword + "," + balance;
      } else {
        cout << "Invalid choice." << endl;
        return;
  		}
		}
		// Add the line to the vector
		lines.push_back(line);
	}

	// Close the file
	infile.close();
	
	// Open the file in output mode
	ofstream outfile("users.txt");
	
	// Write the lines to the file
	for (string s : lines) {
		outfile << s << endl;
	}
	
	// Close the file
	outfile.close();
	
	cout << "Account successfully modified." << endl;
}	
