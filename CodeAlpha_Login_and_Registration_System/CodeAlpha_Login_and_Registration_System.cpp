# include <iostream>
# include <fstream>
# include <string>
# include <random>
# include <functional>
# include <cctype>
# include <ctime>
using namespace std;


// Accepts input of an entire line after discarding leading whitespace.
string inputLine(){
    string s;
    getline(cin >> ws, s);
    return s;
}


// Prevents multiple users from registering with the same username.
bool duplicate (const string& username){

    ifstream file("users.txt");

    if(!file.is_open()) return false;

    string stored_username, stored_salt;
    size_t stored_password;

    while (file >> stored_username >> stored_salt >> stored_password){
        if(stored_username == username){
            file.close();
            cout << "\nError : Username already exist. \nTry Again!!!" << endl;
            return true;
        }
    }
    file.close();
    return false;
}


// Confirms at least 1 alphabet in username
bool alphabet (const string& username){
    for (char c : username) {
        if (isalpha (static_cast<unsigned char> (c))) return true;
    }
    cout << "\nUsername must contain atleast one Alphabet. \nTry Again!!!" << endl;
    return false;
}


// Checks if the (input)string contains any space
bool contains_space (const string& s) {
    if (s.find(' ') != string::npos) {
        cout << "\nInput cannot contain empty spaces. \nTry Again!!!" << endl;
        return true;
    }
    return false;
}


// Makes each character in string lowercase
void lowercase (string& s) {
    for(char& c : s){
        c = static_cast<char> (tolower (static_cast<unsigned char> (c)));
    }
}


// Helper for hashing function.
size_t hashing (const string& password, const string& salt){
    /*
    1. In a production system, passwords are hashed before storage.
    2. Hashing passwords improves the security in case of Data Breach.
    3. Here, a salt is added to the password, which is being hashed.
    */
   return hash <string>{} (password + salt);
}


// Generates random Salt values for each User. (Helped by ChatGPT)
// A Salt is a random string of characters, prevents identical passwords from producing identical hashes.
string salting (int len = 16){
    
    const string chars = 
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789";

    static mt19937 gen(time(nullptr));
    // time(nullptr) is used instead of an object of random_device, to provide a different seed on each run.

    uniform_int_distribution<> dist(0, chars.size()-1);

    string salt;

    for(size_t i=0; i<len; i++){
        salt += chars[dist(gen)];
    }
    return salt;
}


// Registers new user.
void registration(){
    cout << "\n\n~~~~~~ REGISTRATION PAGE ~~~~~~\n" << endl;

    cout<< "NOTE : The following constraints must be satisfied." << endl
        << "1. Username must be unique, non-empty and contains at least one alphabet." << endl
        << "2. Username and Password are case-sensitive." << endl
        << "3. Do not enter empty space (' ') in Username or Password." << endl
        << "4. Your Password must be atleast 8 characters long and contain atleast - " << endl
        << "\t(i) an UPPERCASE letter(A-Z),\n\t(ii) a lowercase letter(a-z) and\n\t(iii) a Digit(0-9)." << endl;
    
    bool exist, alpha, space, upper, lower, digit;
    string username, pass1, pass2;

    do {
        cout << "\nEnter Valid and Unique Username : ";
        username = inputLine();

        space = contains_space(username);
        exist = duplicate(username);
        alpha = alphabet(username);

    } while (username.empty() || exist || space || !alpha);
    // Validating username with it's constraints.


    do {
        pass2 = "";

        cout << "Enter Password for the Username : ";
        pass1 = inputLine();

        upper = lower = digit = false;
        space = contains_space(pass1);

        for (char c : pass1){
            if (upper && lower && digit) break;

            else if (isupper (static_cast<unsigned char> (c))) upper = true;
            else if (islower (static_cast<unsigned char> (c))) lower = true;
            else if (isdigit (static_cast<unsigned char> (c))) digit = true;
        }
        
        if(!upper){
            cout << "\nPassword must contain an UPPERCASE letter. \nTry Again!!!\n" << endl;
        }
        else if(!lower){
            cout << "\nPassword must contain a lowercase letter. \nTry Again!!!\n" << endl;
        }
        else if(!digit){
            cout << "\nPassword must contain a Digit. \nTry Again!!!\n" << endl;
        }
        else if (pass1.length() < 8){
            cout << "\nPassword must contain at least 8 characters. \nTry Again!!!\n" << endl;
        }

        else {
            cout << "Re-enter the Password to verify : ";
            pass2 = inputLine();
            // Two password entries are used to make sure user had not entered any wrong character.

            if(pass1 != pass2){
                cout << "\nError : Password doesn't match. \nTry Again!!!\n" << endl;
            }
        }
    } while ((pass1 != pass2) || pass1.length()<8 || space || !upper || !lower || !digit);
    // Validating password with it's constraints.

    
    // Generating a unique salt and hash the password before storing it.
    string salt = salting();
    size_t hashed_password = hashing ( pass1, salt );
    /*
    1. Salt must be stored because it's required to recreate the hash during login verification.
    2. At the time of login, the entered password will be added with the same salt,
       then will be hashed, before comparing against the stored hashed_password.
    */

    ofstream file("users.txt", ios::app);

    if (!file.is_open()) {
        cout << "\nError : File couldn't open.\n" << endl;
        return;
    }
    
    file << username << "\t" << salt << "\t" << hashed_password << endl;
    file.close();

    cout << "\nCongratulations " << username << " !\nRegistration Successful!\n" << endl;
}


// Login already registered users.
bool login (bool allowRegistration) {
    
    bool found = false, loggedin = false;
    string retry;

    while(true){
        cout << "\n\n~~~~~~ LOGIN PAGE ~~~~~~\n" << endl;

        found = loggedin = false;
        
        // Reading the file containing credentials.
        ifstream file("users.txt");

        if (!file.is_open()) {
            cout << "\nNo Users are Registered yet." << endl;
            return false;
        }

        string username, password;

        cout << "Enter Username  : ";
        username = inputLine();

        // getline() is used so that invalid spaces remain in the input
        // and do not accepted at next input terminal

        string stored_username, stored_salt;
        size_t stored_password;
        
        // Checking username to stored_username and if true
        // hashed_password [password + stored_salt] to stored_password.
        
        while (file >> stored_username >> stored_salt >> stored_password){
            if (stored_username == username){
                found = true;

                // Password is requested only after a matching username is found.
                cout << "Password        : ";
                password = inputLine();

                size_t hashed_password = hashing ( password, stored_salt );

                if (stored_password == hashed_password) loggedin = true;
                break;
            }
        }

        file.close();

        if (loggedin) {
            cout << "\n\nLogin Successful! \nWelcome " << username << " !\n\n" << endl;
            return true;
        }
        else if (found) {
            cout << "\nError : Password Incorrect." << endl;
        }
        else {
            cout << "\nError : Invalid Username." << endl;
        }

        // Keep prompting until the login succeeds or the user decides to stop.
        cout << "\nWould you like to retry Login? ( Yes / No ) : ";
        retry = inputLine();

        lowercase(retry);
        if(retry != "yes") break;
    }

    // Only if not logged in and registration allowed, asks if user want to register.
    if (!loggedin && allowRegistration) {
        cout << "\nWould you like to register a new account? ( Yes / No ) : ";
        retry = inputLine();

        lowercase(retry);
        if(retry == "yes") return false;
    }
    return true;
}


int main(){

    cout << "\n\nThis is Login and Registration System for Users.\n" << endl;

    string check;
    cout << "Are You a Registered User? ( Yes / No ) : ";
    check = inputLine();

    lowercase(check);

    bool withoutRegistration = false;

    if(check == "yes") {
        withoutRegistration = login(true);

        // If user want to register, this leads to registration page
        if (!withoutRegistration) {
            cout << "\n\nRedirecting to Registration Page..." << endl;
            check = "no";
        }
    }
    
    if(check == "no"){
        registration();
        cout << "\nNow, login using your new credentials." << endl;
        login(false);
    }

    else if (check != "yes") {
        cout << "\nInvalid Input!!!\n" << endl;
    }

    return 0;
}