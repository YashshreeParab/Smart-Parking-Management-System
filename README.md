# Smart Parking Management System

A console-based **Smart Parking Management System** developed in C using fundamental data structures. The project manages vehicle queues, parking allocation, parking zones, vehicle searching, and exit billing.

## 📌 Project Overview

The Smart Parking Management System is designed to simplify parking operations by organizing waiting vehicles, managing parking spaces, maintaining vehicle records, and calculating parking fees when vehicles exit.

It demonstrates the practical implementation of multiple data structures in C.

## ✨ Features

- **Vehicle Registration:** Accepts vehicle details and validates vehicle registration numbers.
- **Waiting Queue:** Maintains waiting vehicles using a Circular Queue.
- **Parking Allocation:** Allocates available parking spaces based on vehicle type and parking zone.
- **Parked Vehicle Records:** Maintains currently parked vehicles using a Singly Linked List.
- **Parking Zone Management:** Uses a Binary Search Tree (BST) to organize parking zones and track occupancy.
- **Vehicle Search:** Uses a Hash Table to search for currently parked vehicles by registration number.
- **Exit and Billing:** Processes vehicle exits and calculates parking fees.
- **Exit History:** Uses a Stack to maintain recent vehicle exit records.
- **Console Menu:** Provides a menu-driven interface for performing parking operations.

## 🛠️ Technologies Used

- **Programming Language:** C
- **Compiler:** GCC or another compatible C compiler
- **Interface:** Console-based application

## 📚 Data Structures Implemented

| Data Structure | Application |
|---|---|
| Circular Queue | Stores vehicles waiting for parking using FIFO order |
| Stack | Maintains vehicle exit and billing history |
| Singly Linked List | Stores records of currently parked vehicles |
| Binary Search Tree (BST) | Organizes parking zones and tracks occupied spaces |
| Hash Table | Supports searching for parked vehicles by registration number |

## 🚀 Getting Started

### Prerequisites

Install a C compiler such as GCC.

### Compilation

Open a terminal in the folder containing the C source file and run:

```bash
gcc parking_management_final.c -o parking
```

### Run the Program

**Windows:**

```bash
parking.exe
```

**Linux / macOS:**

```bash
./parking
```

## 🚗 How It Works

1. Add a vehicle to the waiting queue.
2. Allocate parking when a suitable zone has an available space.
3. View waiting vehicles and currently parked vehicles.
4. Search for a vehicle using its registration number.
5. Process a vehicle's exit and calculate the parking fee.
6. View or manage recent exit records.
7. Display parking zones and their occupancy information.

## 🎯 Learning Objectives

- Understand and implement common data structures in C.
- Apply FIFO and LIFO principles to real-world problems.
- Practice pointers, structures, dynamic memory allocation, and linked nodes.
- Implement searching and insertion operations.
- Understand how multiple data structures can work together in one application.

## 🔮 Future Enhancements

- Database integration for persistent vehicle records.
- Graphical user interface or web-based dashboard.
- Real-time parking availability updates.
- Automated parking slot detection.
- Digital payment integration.
- Support for larger parking facilities.

## 👩‍💻 Project Type

Academic project focused on Data Structures and Programming in C.

---

**Note:** This is a console-based academic project. Parking capacity and fee rules are determined by the current program implementation.