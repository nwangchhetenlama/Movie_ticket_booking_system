
# NSR CINEMA Booking System

Welcome to the **NSR CINEMA Booking System**, a console-based application written in C++ that allows users to register, log in, browse movies, book tickets, manage bookings, and play movie trailers. This system simulates a real-world cinema ticket booking experience with seat selection and receipt generation.

---

## Features

- **User Registration and Login:**  
  Users can create new accounts or log in with existing credentials stored in `users.txt`.

- **Movie Browsing:**  
  Displays a list of currently available movies with details such as genre, duration, and ticket price.

- **Trailer Playback:**  
  Open movie trailers in the default web browser.

- **Ticket Booking:**  
  Choose movie, date, showtime, and seats with real-time seat availability. Generates booking receipt.

- **Manage Bookings:**  
  View existing bookings, cancel bookings (which frees up seats), or modify bookings by changing seats.

- **Persistent Storage:**  
  User credentials and booking data are saved to `users.txt` and `bookings.txt` respectively.

---


## 📂 File Structure

.
├── main.cpp # Main application source code
├── users.txt # Stores registered user credentials
├── bookings.txt # Stores booking records
├── README.md # Project documentation

## Requirements

- **C++11 or later**
- **g++** (or any other compatible C++ compiler)
- **Standard C++ libraries**

## How to Run

1. **Compile the program:**

   Using g++ (or any compatible C++ compiler), compile the source code:

   ```bash
   g++ -o cinema_booking_system main.cpp

2. **Run the program:**

   
   ```bash
   ./cinema_booking_system

## 📝 License

This project is licensed under the **MIT License**.  
See the [LICENSE](LICENSE) file for full details.

