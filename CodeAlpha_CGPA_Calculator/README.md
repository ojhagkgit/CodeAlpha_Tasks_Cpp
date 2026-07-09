# CGPA Calculator

A simple console-based C++ program that calculates **SGPA (Semester Grade Point Average)** and **CGPA (Cumulative Grade Point Average)** for a student on a **10.0 grading scale**, based on grades and credit hours entered for each course across multiple semesters.

## Features

- Supports **1 to 18 semesters**.
- Supports **1 to 40 courses** per semester.
- Accepts grades on a **0–10 scale** and credit hours from **0 to 400**.
- Validates all numeric input (rejects non-integer or out-of-range values, with a re-prompt loop).
- Cleans up subject names by trimming leading/trailing spaces and collapsing multiple spaces into one.
- Calculates and displays:
  - Individual course grades per semester.
  - **SGPA** for each semester (2 decimal places).
  - Final **CGPA** across all semesters (2 decimal places).

## How It Works

1. The program asks how many semesters the student attended.
2. For each semester, it asks:
   - The number of courses taken.
   - For each course: subject name, grade (0–10), and credit hours.
3. Once all data is collected, it prints a results summary:
   - A list of courses and grades for each semester.
   - The SGPA for each semester, calculated as:
     ```
     SGPA = Σ(grade × credit hours) / Σ(credit hours)
     ```
   - The overall CGPA, calculated as:
     ```
     CGPA = Σ(SGPA × semester credit hours) / Σ(total credit hours)
     ```

## Requirements

- A C++ compiler supporting C++11 or later (uses `<sstream>`, `<vector>`, `<string>`, etc.)

## Build & Run

Using `g++`:

```bash
g++ -o cgpa_calculator CodeAlpha_CGPA_Calculator.cpp
./cgpa_calculator
```

On Windows (MinGW):

```bash
g++ -o cgpa_calculator.exe CodeAlpha_CGPA_Calculator.cpp
cgpa_calculator.exe
```

## Example Usage

```
This is a 10.0 Scale CGPA Calculator.
It displays GPA upto 2 decimal points.

Enter the number of Semesters attended by the Student (1-18) : 2

Semester 1 Details :

Enter total number of Courses taken by the Student in the Semester : 2

Enter the details of the Course 1 -
Subject Name : Mathematics
Grade (0-10) : 9
Credit Hours : 4

Enter the details of the Course 2 -
Subject Name : Physics
Grade (0-10) : 8
Credit Hours : 3
...

Result :

Semester 1 Course Grades -

1. Mathematics : 9
2. Physics : 8

SGPA : 8.57

...

CGPA of the Student is 8.40.
```

## Project Structure

```
CodeAlpha_CGPA_Calculator.cpp   # Main source file containing all logic
```

### Key Components

| Component | Description |
|---|---|
| `struct Semester` | Holds a semester's subject names, grades, and credit hours as parallel vectors. |
| `extra_Spaces()` | Utility function that trims and normalizes whitespace in subject names. |
| `get_int_value()` | Reusable, validated integer input function with prompt, min/max range checks, and error handling. |
| `input_Semester()` | Collects course details (subject, grade, credit hours) for a single semester. |
| `main()` | Drives the program: gathers semester count and details, then computes and displays SGPA/CGPA. |

## Notes

- Credit hours of `0` are allowed (e.g., for audit/non-credit courses) and simply don't contribute to the weighted average.
- If a semester ends up with `0` total credit hours, its SGPA is reported as `0` to avoid division by zero (the same safeguard applies to the overall CGPA).
- Input validation loops indefinitely until a valid value is provided, so the program will not crash on bad input (e.g., letters typed instead of numbers).

## Author's Note

This project was built as part of the **CodeAlpha** Internship task series (see filename prefix `CodeAlpha_`).

Build by - Gaurav Kumar Ojha.