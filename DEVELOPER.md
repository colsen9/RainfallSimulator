============================================================
DEVELOPER DOCUMENTATION Project 1 - Rainfall Flood Simulation
============================================================

Author: Cayden Olsen
Course: COM S 3270
Repository: coms3270p1
Date: 10/31/2025

------------------------------------------------------------
Repository Structure
------------------------------------------------------------

coms3270P1/
│
├── mapper.c                          # Contains main() and program entry
├── data.c                            # Contains validate() and helper logic
├── data.h                            # Function prototypes and header guards
├── Makefile                          # Build configuration for the mapper program
├── Ames.csv                          # Sample large input data
├── test_invalid_id.csv               # Small valid test file
├── test_invalid_latitude.csv         # Invalid data (bad lat/long)
├── test_valid.csv                    # Invalid data (empty name)
├── README                            # Description of program features and usage
└── DEVELOPER                         # This file

------------------------------------------------------------
Source Files Overview
------------------------------------------------------------

1. data.c
   - Implements:
       int validate(void);
   - Reads a tab-separated dataset from stdin.
   - Performs data validation for POIs and roads.
   - Returns 0 if valid, or the line number of the first invalid entry.
   - Includes a helper function:
       int is_valid_id(const char *s);
     that allows numeric IDs with an optional leading '-'.

2. data.h
   - Header file that declares the prototype for validate().
   - Uses include guards to prevent multiple inclusions.

3. mapper.c
   - Contains main() which calls validate().
   - Prints "VALID" if the file is valid, otherwise prints
     the line number of the first invalid entry.

4. Makefile
   - Defines the build process without macros or variables.
   - Targets:
       mapper  - Builds the executable
       clean   - Removes all object and executable files

------------------------------------------------------------
Key Data Structures
------------------------------------------------------------
No complex data structures are used in Part A.
Validation is done sequentially using local variables and scanf() parsing.

------------------------------------------------------------
Function Summary
------------------------------------------------------------

int is_valid_id(const char *s);
    - Checks that a string represents an integer.
    - Allows an optional leading '-' for negative IDs.
    - Returns 1 if valid, 0 otherwise.

int validate(void);
    - Reads data from stdin.
    - Validates POIs and roads according to specification.
    - Returns 0 if valid, or the line number of the first invalid line.

int main(void);
    - Calls validate().
    - Prints "VALID" or the error line number.

------------------------------------------------------------
Compilation Instructions
------------------------------------------------------------
Run:
    make

Produces executable:
    ./mapper

Clean up:
    make clean

------------------------------------------------------------
Testing
------------------------------------------------------------
Run the validator:
    ./mapper < test_valid.csv
    ./mapper < test_invalid_latitude.csv
    ./mapper < test_invalid_id.csv

Expected outputs:
    test_valid.csv: VALID
    test_invalid_latitude.csv: 2
    test_invalid_id: 5

------------------------------------------------------------
Notes
------------------------------------------------------------
- File input is read directly from stdin.
- The validator uses tab-based field separation (`\t`).
- The code handles "NaN" as 0 for distance.

