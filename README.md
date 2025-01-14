# ATM-MANAGEMENT-SYSTEM


## Description

This is an ATM platform developed in C. It allows the user to do the following actions:
*   Login/Register
*   Create a new account
*   Check the details of an existing account they own
*   Check all accounts they own
*   Update information of existing accounts
*   Remove existing accounts
*   Check list of owned accounts
*   Make transactions
*   Transfer account owner
*   Check account balance

## Usage

* To get the program use the following command to clone the repo:
```bash
$  git clone https://learn.zone01kisumu.ke/git/rotieno/atm-management-system.git
```

* Navigate to the directory where you've cloned the repo:
```bash
$ cd atm-management-system
``` 

* To create an executable and run it use the following commands:
```bash
$  make
$  ./atm
```

* After successfully running the program, follow the onscreen instructions to use the program.
## NOTE: ## 
* Account number accepts a minimum of 1 and a maximum of 18 characters.
* Phone number accepts a minimum of 5 and maximum of 14 characters.
* Name, and password accepts a minimum of 1 character and maximum of 49 characters.
* Country accepts a minimum of 2 characters and a maximum of around 65.
* If you want to do a full-rebuild use the following command:
```bash
$  make clean
```

## File System

###  1. data
 This directory contains 2 text file, namely; **records.txt** and **users.txt**. The former is used to store account records while the latter is used to store registration/ signup data. 

 ### 2. src
 This directory contains various C files.
 * **auth.c** - contains user authentication functionalities.
 * **header.h** - contains the declaration of functions and data structures used in the rest of the files.
 * **main.c** - this is the entry point of the program. It coordinates the program flow thus ensuring the menus are displayed accordingly.
 * **secondary.c** - contains additional functions that are used to support the primary functions.
 * **system.c** - contains the primary functions used to implements the key features of the system.

### 3. makefile
This file is used by the `make` build automation tool to manage the build process. It defines how to compile the program.

## Author


* **[Rabin Otieno](https://github.com/Rabinnnn)**

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
