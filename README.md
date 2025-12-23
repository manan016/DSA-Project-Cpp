Data Structures and Algorithms implemented in C++


Hospital Management System

(DSA Project using C++)

A menu-driven, console-based Hospital Management System implemented entirely in C++, showcasing real-world usage of Data Structures and Algorithms (DSA).
This project efficiently manages patients, appointments, emergencies, doctors, staff, and provides reports & analytics with persistent storage.

📌 Project Highlights

Fully written in C++ (C++17 standard)

Uses multiple DSA concepts

Real-world problem implementation

File handling for permanent data storage

Clean, modular, and well-documented code

🚀 Features
👤 Patient Management

Add, update, delete, and display patients

Implemented using Linked List

Quick Sort for sorting patient records

Binary Search for fast patient lookup

Data stored in patients.txt

 Appointment Management (Routine)

Managed using Queue (FIFO)

Schedule and process routine appointments

Data stored in appointments.txt

Emergency Management

Managed using Priority Queue (Max Heap)

Higher priority patients treated first

Linear Search for patient lookup

Data stored in emergencies.txt

 Doctor & Staff Management

Implemented using Hash Tables (unordered_map)

Manage doctor availability

Manage hospital staff and roles

Data stored in:

doctors.txt

staff.txt

 Reporting & Analytics

Merge Sort for generating patient reports

Binary Search Tree (BST) for ailment-wise analytics

Generates:

Total patients

Average age

Gender distribution

Common ailments

 Data Structures Used
Module	Data Structure
Patients	Linked List
Sorting	Quick Sort, Merge Sort
Searching	Binary Search
Appointments	Queue
Emergencies	Priority Queue
Doctors & Staff	Hash Table
Analytics	Binary Search Tree
🗂️ Project Structure
Hospital-Management-System/
│
├── hospital_system.cpp
├── patients.txt
├── appointments.txt
├── emergencies.txt
├── doctors.txt
├── staff.txt
└── README.md

⚙️ How to Compile and Run
🔧 Compile
g++ -std=c++17 hospital_system.cpp -o hospital_system

Run
./hospital_system

📋 Main Menu Options
1. Patient Management
2. Appointments (Routine)
3. Emergency Management
4. Doctor & Staff Management
5. Reporting & Analytics
6. Save All Data
7. Load All Data
0. Exit

💾 File Persistence

All records are automatically saved on exit

Data is automatically loaded on program startup

Ensures no data loss between executions

Learning Outcomes

Hands-on implementation of DSA

Practical understanding of OOP in C++

Efficient data management

File handling and persistence

Modular and scalable software design

 Technologies Used

Language: C++

Standard: C++17

Compiler: GCC / MinGW

Environment: Console / Terminal

 Future Enhancements

GUI-based interface

Database integration

Authentication system

Performance optimization

 Author
Manan Katpar
DSA Project – Hospital Management System
Developed using C++ only

this one past in readme files 
