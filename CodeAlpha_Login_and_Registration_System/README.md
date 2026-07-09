# Login and Registration System

A console-based C++ program that implements a simple, file-backed **user registration and login system**, with salted-and-hashed password storage.

## Features

- **User Registration**
  - Enforces unique, non-empty usernames containing at least one alphabet character.
  - Disallows spaces in usernames and passwords.
  - Passwords are case-sensitive and must be:
    - At least 8 characters long.
    - Contain at least one uppercase letter, one lowercase letter, and one digit.
  - Requires password confirmation (entered twice) to catch typos.
- **User Login**
  - Verifies username, then prompts for a password only if the username exists.
  - Supports retrying on failed login attempts.
  - Offers to redirect to the registration page if login fails and the user isn't yet registered.
- **Password Security**
  - Each password is combined with a randomly generated **16-character salt** before hashing.
  - Uses `std::hash<string>` to hash the salted password.
  - Only the **username, salt, and hashed password** are stored — plaintext passwords are never saved.
- **Persistent Storage**
  - User credentials are stored in and read from a plain text file: `users.txt`.

## How It Works

1. On startup, the program asks whether the user is already registered.
2. **If yes:** it goes to the login flow.
   - The user enters a username; if found, they're prompted for a password.
   - The entered password is salted (using the stored salt) and hashed, then compared to the stored hash.
   - On failure, the user can retry or opt to register a new account instead.
3. **If no** (or if login is declined/redirected): it goes to the registration flow.
   - The user picks a valid, unique username and a password meeting all complexity rules.
   - A random salt is generated, the password is hashed with it, and the record is appended to `users.txt`.
   - After successful registration, the user is prompted to log in with the new credentials.

## Requirements

- A C++ compiler supporting C++11 or later (uses `<random>`, `<functional>`, `<ctime>`, etc.)

## Build & Run

Using `g++`:

```bash
g++ -o login_system CodeAlpha_Login_and_Registration_System.cpp
./login_system
```

On Windows (MinGW):

```bash
g++ -o login_system.exe CodeAlpha_Login_and_Registration_System.cpp
login_system.exe
```

> **Note:** Run the program from the same directory as `users.txt` (or an empty directory, since the file will be created automatically on first registration if it doesn't exist).

## Data File: `users.txt`

Each line represents one registered user, with three tab-separated fields:

```
<username>    <salt>    <hashed_password>
```

Example:

```
gaurav	TGpoJKRjYdW5F2IT	2088979352
amit	PzFyLEVunT2JYIB4	3934493171
```

- **username** — the registered username (plaintext, case-sensitive).
- **salt** — a randomly generated 16-character alphanumeric string unique to that user.
- **hashed_password** — the result of `std::hash` applied to `(password + salt)`, stored as an unsigned integer (`size_t`).

This file is read on every login/registration attempt to check for existing usernames and verify credentials, and new entries are appended to it upon successful registration.

⚠️ **Security note:** `std::hash` is a fast, non-cryptographic hash function intended for hash-table use, not password storage. It is not resistant to brute-force or collision attacks. This project is intended for **learning purposes** (demonstrating the *concept* of salting and hashing) — a real-world system should use a dedicated password-hashing algorithm such as **bcrypt**, **scrypt**, or **Argon2**.

## Project Structure

```
CodeAlpha_Login_and_Registration_System.cpp   # Main source file containing all logic
users.txt                                     # Data file storing username, salt, and hashed password per user
```

### Key Components

| Component | Description |
|---|---|
| `inputLine()` | Reads a full line of input, discarding leading whitespace. |
| `duplicate()` | Checks `users.txt` for an existing username to prevent duplicate registrations. |
| `alphabet()` | Ensures a username contains at least one alphabetic character. |
| `contains_space()` | Rejects input containing spaces. |
| `lowercase()` | Converts a string to lowercase (used for Yes/No prompts). |
| `hashing()` | Combines a password with its salt and hashes the result. |
| `salting()` | Generates a random alphanumeric salt (default length 16). |
| `registration()` | Handles the full registration flow: input, validation, salting/hashing, and saving to file. |
| `login()` | Handles the full login flow: username lookup, password verification, retry logic, and optional redirect to registration. |
| `main()` | Entry point; routes the user to login or registration based on their initial response. |

## Notes

- Since a new random salt is generated for each user, **two users with identical passwords will have different stored hashes**.
- Input validation loops indefinitely until valid data is provided, so the program won't crash on malformed input.
- All credential comparisons are case-sensitive.

## Author's Note

This project was built as part of the **CodeAlpha** Internship task series (see filename prefix `CodeAlpha_`).

Build by - Gaurav Kumar Ojha.