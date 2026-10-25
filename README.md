# Moderated Learning Management System — Phase 4

A C++17 menu-driven LMS based on the assignment brief. The brief requires **SQLite**, rather than MySQL, for the offline/local system.

## Header-based structure

- `include/lms/Database.h` — SQLite `Database` and prepared-statement RAII wrappers.
- `include/lms/Utilities.h` — shared input validation, UTC timestamp, password hashing and salt helpers.
- `include/lms/User.h` — `Account`, abstract `User`, role subclasses and `UserFactory`.
- `include/lms/LatePenalty.h` — late-penalty Strategy interface and school policy.
- `main.cpp` — LMS repository operations, role menus and application entry point.
- `tests/rules_test.cpp` — boundary tests using the same late-penalty header as the application.

The headers have include guards and shared definitions are `inline` where appropriate to avoid multiple-definition linker errors.

## Features

- Four roles: administrator, lecturer, Head of Department and student.
- SQLite persistence, parameterised queries and foreign-key constraints.
- Administrator: create departments, users and courses; enrol students.
- Lecturer: create assignments, grant individual extensions, inspect submissions, mark work and provide feedback.
- Student: view enrolled assignments and effective deadlines, submit and resubmit before deadline, and see only released marks/feedback.
- HoD: review departmental marks, moderate and release approved marks.
- Late penalty: 10% of maximum mark per late day for days 1–3; zero after 3 days.
- Mark audit entries contain actor, timestamp, old/new mark and required reason.
- Salted PBKDF2-HMAC-SHA256 password hashes via OpenSSL; unique 128-bit random salt and 210,000 iterations.
- Input validation and exception handling for menu operations.

The assignment's earlier SRS/design artefacts were not attached; behavior follows the case-study rules. The submission input currently stores text or a path rather than uploading a binary file. Deadline values are expected as UTC `YYYY-MM-DD HH:MM:SS`.

## Build and run

Dependencies: C++17 compiler, CMake 3.16+, SQLite3 and OpenSSL development packages.

```sh
sudo apt-get install build-essential cmake libsqlite3-dev libssl-dev
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
./build/lms
```

A starter SQLite database is included at `lms.db`, with all application tables and the initial administrator account. The matching schema is also provided in `database/schema.sql`. Run `./build/lms` from the project root to use the included database; if it is missing, the application initializes a new database in the current directory. Initial bootstrap login is `admin` / `Admin123!`. Change or remove this bootstrap credential before deployment. Keep the database backed up and restrict filesystem access.

## Design and limitations

- `Repository` in `main.cpp` is the SQLite data-access layer; user-provided values are bound through prepared statements.
- `UserFactory` and `LatePenalty` (Strategy) are implemented; role subclasses provide polymorphic menus.
- Class-wide extensions, password reset/change, binary submission storage, stronger calendar input validation and a broad integration/security test suite are not included in this Phase 4 baseline. Class-wide extensions are specified by the assignment's Phase 8 change request.
- Avoid sharing one SQLite database over an unreliable network filesystem; use a database service if the school needs concurrent multi-machine access.
- Style: C++17, descriptive identifiers, RAII for SQLite statements/database, prepared statements and exceptions for recoverable operational failures.

## Demonstration path

1. Log in as administrator; create a department, lecturer and student accounts, a course, and an enrolment.
2. Log in as lecturer; create an assignment with a UTC deadline and maximum mark.
3. Log in as student; submit work and, if needed, resubmit before the effective deadline.
4. Log in as lecturer; mark it, add feedback and provide the audit reason.
5. Log in as HoD in that department; moderate and release the mark.
6. Log in as student; view released feedback and mark.
