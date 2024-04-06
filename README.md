# Football Match Ticket Booking System

This repository contains a C program for simulating a ticket booking system for football matches using shared memory and semaphores for synchronization among multiple processes.

## Overview

The program allows multiple clients to request tickets for football matches concurrently. It utilizes shared memory to store information about football matches, including match ID, team names, available tickets, and ticket price. Semaphores are used to ensure mutual exclusion when accessing the shared memory to prevent race conditions.

## Features

- Allows multiple clients to request tickets for football matches concurrently.
- Utilizes shared memory to store match information, enabling efficient communication between processes.
- Implements semaphores for synchronization to prevent race conditions when accessing shared resources.

## Installation

To compile and run the program:

1. Clone the repository to your local machine.
2. Navigate to the repository directory.
3. Compile the program using a C compiler (e.g., gcc).
    ```
    gcc SharedMemory.c.c -o SharedMemory -lrt
    ```
4. Run the executable.
    ```
    ./SharedMemory
    ```

## Usage

The program will simulate multiple clients requesting tickets for football matches. Each client process will generate random requests for match IDs and the number of tickets. The program will then check if the requested tickets are available and update the shared memory accordingly. Finally, the program will print the details of the booked tickets.

## Authors

- [ Katerina Antoniou ](https://github.com/k-antoniou)

## License

This project is licensed under the [MIT License](LICENSE).

## Contributing

Contributions are welcome! Feel free to open an issue or submit a pull request for any improvements or bug fixes.
