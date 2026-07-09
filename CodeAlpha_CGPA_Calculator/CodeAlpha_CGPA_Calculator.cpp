# include <iostream>
# include <iomanip>
# include <vector>
# include <string>
# include <cctype>
# include <limits>
# include <climits>
# include <sstream>
using namespace std;


// structure of Semester (user-defined) data type
struct Semester{
    vector <string> subjects;
    vector <int> grades;
    vector <int> hours;
};


// extra_Spaces is user defined function to remove extra whitespaces
string extra_Spaces (const string& str) {

    string result = "";
    bool last_space = true;

    for(char c : str) {
        if (isspace (static_cast<unsigned char>(c))) { 
            if(!last_space){
                result += ' ';
                last_space = true;
            }
        }
        else {
            result += c;
            last_space = false;
        }
    }
    // removing the last ' ' empty space
    if(!result.empty() && (result.back() == ' ')) result.pop_back();

    return result;
}


// This function takes input from user. 
// Function reuseability. (Helped by ChatGPT)
int get_int_value ( const string& prompt, int minimum, int maximum) {
    
    int value;
    string line;

    while (true) {
        cout << prompt;
        getline(cin >> ws , line);

        stringstream front(line);

        if (!(front >> value)) {
            cout << "\nError : Please enter an integer value." << endl;
            continue;
        }

        /*
        After reading the int value, check whether there is any non-whitespace character left in the stream.

        Examples:
        
        "1"
         ^
        front >> value = 1
        There is nothing left
        front >> extra => false

        "2.5"
         ^
        value = 2
        ".5" is still left
        extra = '.'
        front >> extra => true
        */
        
        char extra;
        if (front >> extra){
            cout << "\nError : Please enter an integer value." << endl;
            continue;
        }
        if (value < minimum || value > maximum) {
            cout << "\nThe Value must be between " << minimum << " and " << maximum  << "." << endl;
            continue;
        }

        return value;
    }
}


// input semester's details
Semester input_Semester (int asem){
    
    cout << "\n\nSemester " << asem << " Details : \n" << endl;
    int n = get_int_value ("Enter total number of Courses taken by the Student in the Semester : ", 1 , 40);
    // Atleast there must be 1 course per semester
    // There won't be any semester with more than 40 courses

    // Three vectors are used for names, grade points and credit hours of course 
    vector <string> subjects(n,"");
    vector <int> grades(n,0);
    vector <int> hours(n,0);

    int a=0;

    while (a<n) {    // Input of course details by the user

        cout << endl << "\nEnter the details of the Course " << a+1 << " -" <<endl;

        do {
            cout << "Subject Name : ";
            getline (cin >> ws , subjects[a]);
            subjects[a] = extra_Spaces (subjects[a]);

            // getline() is used to store more than 1 words
            // ws is used to remove leading whitespaces

        } while (subjects[a].empty());

        grades[a] = get_int_value( "Grade (0-10) : ", 0 , 10);

        hours[a] = get_int_value( "Credit Hours : ", 0 , 400); 
        // 0 credit subjects are also considered here
        // Some rare degrees require 300+ credit hours in total

        a++;
    }

    return {subjects,grades,hours};
}


// Calculations and result displayed here
int main(){

    cout << "\n\nThis is a 10.0 Scale CGPA Calculator." << endl;
    cout << "It displays GPA upto 2 decimal points.\n" << endl;
    int sem = get_int_value ("Enter the number of Semesters attended by the Student (1-18) : ", 1 , 18);
    // Some countries have courses upto 9 years(18 semesters) in a rare structure.
    
    vector <Semester> sems(sem);

    int a=1;
    do {
        sems[a-1] = input_Semester(a);
        a++;
    } while(a <= sem);

    // Printing the result part below
    cout << endl << endl << "\nResult :" << endl << endl;

    a=0;
    int total_credit_hours = 0;
    double CGPA = 0;

    while (a < sem){

        cout << "\nSemester " << a+1 << " Course Grades -" << endl << endl;

        int credit_hours = 0;
        double total_grade_points = 0, SGPA;
        /* 
        total_grade_points is used as double, bcz we have to divide it by credit_hours
        then add that result in SGPA(which is itself a double)
        1. (int\int) != double
        2. (double\int) = double 
        */

        for(size_t i=0; i < sems[a].grades.size(); i++){

            // Displaying course grade of each course of the semester
            cout << i+1 << ". " << sems[a].subjects[i] << " : " << sems[a].grades[i] << endl;

            total_grade_points += ((sems[a].grades[i]) * (sems[a].hours[i]));
            credit_hours += (sems[a].hours[i]);
        }

        // SGPA calculation
        if (credit_hours == 0) SGPA=0;
        else SGPA = total_grade_points / credit_hours;

        // Displaying SGPA(upto 2 decimal places) for the semester
        cout << "\nSGPA : " << fixed << setprecision(2) << SGPA << endl << endl;

        // Adding values for CGPA
        total_credit_hours += credit_hours;
        CGPA += (SGPA * credit_hours);

        ++a;
    }
    
    // CGPA calculation
    if (total_credit_hours == 0) CGPA=0;
    else CGPA /= total_credit_hours;

    // Displaying final CGPA(upto 2 decimal point) of all semesters
    cout << "\nCGPA of the Student is " << fixed << setprecision(2) << CGPA << ".\n" << endl ;

    return 0;
}