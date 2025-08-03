#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <cstdlib>
#include <random>
#include <ctime>
#include <limits>  // for numeric_limits
#include <sstream>
#include <map>

using namespace std;

// ---------------- Helper functions ----------------

string getCurrentDate(int offsetDays = 0) {
    time_t t = time(nullptr);
    t += offsetDays * 24 * 60 * 60;
    tm *timePtr = localtime(&t);
    char buffer[11];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", timePtr);
    return string(buffer);
}

int getValidatedIntInput(int minVal, int maxVal, const string& prompt) {
    int choice;
    while (true) {
        cout << prompt;
        if (cin >> choice && choice >= minVal && choice <= maxVal) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return choice;
        } else {
            cout << "Invalid input. Please enter a number between " << minVal << " and " << maxVal << ".\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

int getValidatedIntInputMin(int minVal, const string& prompt) {
    int val;
    while (true) {
        cout << prompt;
        if (cin >> val && val >= minVal) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return val;
        } else {
            cout << "Invalid input. Please enter a number greater than or equal to " << minVal << ".\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

string getValidatedSeatInput(int maxRow, int maxCol, const string& prompt) {
    while (true) {
        cout << prompt;
        string seatId;
        getline(cin, seatId);

        if (seatId.length() < 2 || seatId.length() > 3) {
            cout << "Invalid seat format. Example valid seat: A1\n";
            continue;
        }

        transform(seatId.begin(), seatId.end(), seatId.begin(), ::toupper);

        char rowChar = seatId[0];
        if (rowChar < 'A' || rowChar >= 'A' + maxRow) {
            cout << "Invalid row letter. Must be between A and " << (char)('A' + maxRow - 1) << ".\n";
            continue;
        }

        string colStr = seatId.substr(1);
        try {
            int colNum = stoi(colStr);
            if (colNum < 1 || colNum > maxCol) {
                cout << "Invalid column number. Must be between 1 and " << maxCol << ".\n";
                continue;
            }
        } catch (...) {
            cout << "Invalid seat number format.\n";
            continue;
        }

        return seatId;
    }
}

// ---------------- USER CLASS ----------------

class User {
private:
    string username;
    string password;

public:
    User() {}

    bool registerUser() {
        cout << "\n--- REGISTER ---\n";
        cout << "Enter new username: ";
        cin >> username;
        cout << "Enter new password: ";
        cin >> password;

        ifstream infile("users.txt");
        if (infile) {
            string u, p;
            while (infile >> u >> p) {
                if (u == username) {
                    cout << "Username already exists!\n";
                    return false;
                }
            }
        }
        infile.close();

        ofstream outfile("users.txt", ios::app);
        outfile << username << " " << password << "\n";
        cout << "Registration successful!\n";
        return true;
    }

    bool loginUser() {
        cout << "\n--- LOGIN ---\n";
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;

        ifstream infile("users.txt");
        if (!infile) {
            cout << "No registered users found.\n";
            return false;
        }

        string u, p;
        while (infile >> u >> p) {
            if (u == username && p == password) {
                cout << "Login successful. Welcome, " << username << "!\n";
                return true;
            }
        }
        cout << "Invalid username or password.\n";
        return false;
    }

    string getUsername() const { return username; }
};

// ---------------- MOVIE CLASS ----------------

class Movie {
private:
    string id, title, genre, trailer;
    int duration;
    double price;

public:
    Movie(string i, string t, string g, int d, double p, string u)
        : id(i), title(t), genre(g), duration(d), price(p), trailer(u) {}

    string getId() const { return id; }
    string getTitle() const { return title; }
    string getGenre() const { return genre; }
    int getDuration() const { return duration; }
    double getPrice() const { return price; }
    string getTrailerURL() const { return trailer; }

    void displayInfo() const {
        cout << "Title: " << title << "\nGenre: " << genre
             << "\nDuration: " << duration << " mins"
             << "\nPrice: Rs. " << fixed << setprecision(2) << price << endl;
    }

    void playTrailer() const {
        cout << "Opening trailer for " << title << "...\n";
        string command = "start " + trailer;
        system(command.c_str());
    }
};

// ---------------- SEAT CLASS ----------------

class Seat {
private:
    string seatId;
    bool isBooked;

public:
    Seat(string id) : seatId(id), isBooked(false) {}

    string getSeatId() const { return seatId; }
    bool available() const { return !isBooked; }

    bool book() {
        if (isBooked) return false;
        isBooked = true;
        return true;
    }

    void setAvailable(bool val) { isBooked = !val; }
};

// ---------------- BOOKING CLASS ----------------

class Booking {
public:
    static int bookingCounter;

private:
    string bookingId;
    string username;
    string movieTitle;
    string date;
    string showtime;
    vector<string> seatIds;
    double totalAmount;

public:
    Booking() {}

    Booking(const string& user, const string& mov, const string& d, const string& time)
        : username(user), movieTitle(mov), date(d), showtime(time), totalAmount(0) {
        bookingId = "BKG" + to_string(++bookingCounter);
    }

    string getUsername() const { return username; }
    string getBookingId() const { return bookingId; }
    string getMovieTitle() const { return movieTitle; }
    string getDate() const { return date; }
    string getShowtime() const { return showtime; }
    vector<string> getSeats() const { return seatIds; }
    double getTotalAmount() const { return totalAmount; }

    void setSeats(const vector<string>& seats) { seatIds = seats; }
    void setTotalAmount(double amt) { totalAmount = amt; }
    
    // Add setter for bookingId to fix loading from file
    void setBookingId(const string& id) { bookingId = id; }

    void generateReceipt() const {
        string filename = "receipt_" + bookingId + ".txt";
        ofstream receipt(filename);
        if (!receipt) {
            cout << "Error creating receipt file.\n";
            return;
        }
        receipt << "----- NSR CINEMA BOOKING RECEIPT -----\n";
        receipt << "Booking ID: " << bookingId << "\n";
        receipt << "Username: " << username << "\n";
        receipt << "Movie: " << movieTitle << "\n";
        receipt << "Date: " << date << "\n";
        receipt << "Showtime: " << showtime << "\n";
        receipt << "Seats Booked: ";
        for (const string& s : seatIds) receipt << s << " ";
        receipt << "\nTotal Amount: Rs. " << totalAmount << "\n";
        receipt << "Thank you for booking with NSR Cinema!\n";
        receipt.close();
        cout << "Receipt saved to '" << filename << "'\n";
    }

    void displayTicket() const {
        cout << "\n--- TICKET ---\nBooking ID: " << bookingId
             << "\nUsername: " << username
             << "\nMovie: " << movieTitle
             << "\nDate: " << date
             << "\nShowtime: " << showtime
             << "\nSeats: ";
        for (auto s : seatIds) cout << s << " ";
        cout << "\nAmount Paid: Rs. " << totalAmount << "\n";
        cout << "----------------\n";
    }
};

int Booking::bookingCounter = 0;

// ---------------- THEATER CLASS ----------------

class Theater {
private:
    string name;
    vector<Movie> movies;
    int rows = 4, cols = 5;

    // seatMatrix[movieIdx][date][showtime] = 2D vector seats
    map<int, map<string, map<string, vector<vector<Seat>>>>> seatMatrix;

    // Predefined showtimes per day
    vector<string> showtimes = {"10:00 AM", "1:00 PM", "6:00 PM"};

public:
    Theater(string _name, const vector<Movie>& _movies) : name(_name), movies(_movies) {
        // Initialize seatMatrix for each movie, today + tomorrow + 3 more days, each showtime
        vector<string> datesToInit = {
            getCurrentDate(0), getCurrentDate(1), getCurrentDate(2), getCurrentDate(3), getCurrentDate(4)
        };

        for (int m = 0; m < (int)movies.size(); ++m) {
            for (const auto& d : datesToInit) {
                for (const auto& s : showtimes) {
                    vector<vector<Seat>> seats;
                    for (int r = 0; r < rows; ++r) {
                        vector<Seat> row;
                        for (int c = 0; c < cols; ++c) {
                            string seatId = string(1, 'A' + r) + to_string(c + 1);
                            row.push_back(Seat(seatId));
                        }
                        seats.push_back(row);
                    }
                    seatMatrix[m][d][s] = seats;
                }
            }
        }
    }

    void displayMovies() const {
        int numMovies = movies.size();
        int colWidth = 30;

        for (int i = 0; i < numMovies; ++i) {
            cout << left << setw(colWidth) << (to_string(i + 1) + ". " + movies[i].getTitle());
        }
        cout << "\n";

        for (const auto& m : movies) {
            cout << left << setw(colWidth) << ("Genre: " + m.getGenre());
        }
        cout << "\n";

        for (const auto& m : movies) {
            cout << left << setw(colWidth) << ("Duration: " + to_string(m.getDuration()) + " mins");
        }
        cout << "\n";

        for (const auto& m : movies) {
            cout << left << setw(colWidth) << ("Price: Rs. " + to_string(int(m.getPrice())));
        }
        cout << "\n";
    }

    void showSeats(int movieIdx, const string& date, const string& showtime) const {
        cout << "\nSeat Availability for " << movies[movieIdx].getTitle()
             << " on " << date << " at " << showtime << ":\n";
        const auto& seats = seatMatrix.at(movieIdx).at(date).at(showtime);

        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                const Seat& seat = seats[r][c];
                cout << seat.getSeatId() << (seat.available() ? "( ) " : "(X) ");
            }
            cout << endl;
        }
        cout << "( ) = Available, (X) = Booked\n";
    }

    bool bookSeat(int movieIdx, const string& date, const string& showtime, const string& seatId) {
        auto& seats = seatMatrix[movieIdx][date][showtime];
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (seats[r][c].getSeatId() == seatId) {
                    return seats[r][c].book();
                }
            }
        }
        return false;
    }

    void freeSeats(int movieIdx, const string& date, const string& showtime, const vector<string>& seatsToFree) {
        auto& seats = seatMatrix[movieIdx][date][showtime];
        for (const string& seatId : seatsToFree) {
            for (int r = 0; r < rows; ++r) {
                for (int c = 0; c < cols; ++c) {
                    if (seats[r][c].getSeatId() == seatId) {
                        seats[r][c].setAvailable(true);
                    }
                }
            }
        }
    }

    int getMovieIndexByTitle(const string& title) const {
        for (int i = 0; i < (int)movies.size(); ++i) {
            if (movies[i].getTitle() == title)
                return i;
        }
        return -1;
    }

    double getPriceByTitle(const string& title) const {
        int idx = getMovieIndexByTitle(title);
        if (idx == -1) return 0;
        return movies[idx].getPrice();
    }

    void playTrailer(int idx) { movies[idx].playTrailer(); }
    
    // Add missing methods
    int getMoviesCount() const { return movies.size(); }
    string getMovieTitle(int idx) const { return movies[idx].getTitle(); }
    int getShowtimesCount() const { return showtimes.size(); }
    string getShowtime(int idx) const { return showtimes[idx]; }
};

// ---------------- BOOKING STORAGE ----------------

vector<Booking> loadBookings() {
    vector<Booking> bookings;
    ifstream file("bookings.txt");
    if (!file) return bookings;

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string bookingId, username, movieTitle, date, showtime;
        iss >> bookingId >> username >> movieTitle >> date >> showtime;

        vector<string> seats;
        string token;
        double amount = 0.0;
        while (iss >> token) {
            // try convert to double (last token)
            try {
                amount = stod(token);
                break;
            } catch (...) {
                seats.push_back(token);
            }
        }

        Booking b(username, movieTitle, date, showtime);
        b.setSeats(seats);
        b.setTotalAmount(amount);

        // Use the setter method instead of direct access
        b.setBookingId(bookingId);
        int num = stoi(bookingId.substr(3));
        if (num > Booking::bookingCounter)
            Booking::bookingCounter = num;

        bookings.push_back(b);
    }
    return bookings;
}

void saveBookings(const vector<Booking>& bookings) {
    ofstream file("bookings.txt");
    for (const auto& b : bookings) {
        file << b.getBookingId() << " "
             << b.getUsername() << " "
             << b.getMovieTitle() << " "
             << b.getDate() << " "
             << b.getShowtime() << " ";
        for (const string& seat : b.getSeats())
            file << seat << " ";
        file << b.getTotalAmount() << "\n";
    }
}

// ---------------- MANAGE BOOKINGS ----------------

void manageBookings(User& currentUser, Theater& theater, vector<Booking>& bookings) {
    vector<Booking*> userBookings;

    for (auto& b : bookings) {
        if (b.getUsername() == currentUser.getUsername())
            userBookings.push_back(&b);
    }

    if (userBookings.empty()) {
        cout << "You have no bookings.\n";
        return;
    }

    cout << "\nYour Bookings:\n";
    for (size_t i = 0; i < userBookings.size(); ++i) {
        Booking* b = userBookings[i];
        cout << i + 1 << ". " << b->getBookingId() << " | Movie: " << b->getMovieTitle()
             << " | Date: " << b->getDate() << " | Showtime: " << b->getShowtime()
             << " | Seats: ";
        for (auto& seat : b->getSeats()) cout << seat << " ";
        cout << "| Total: Rs. " << b->getTotalAmount() << "\n";
    }

    int choice = getValidatedIntInput(1, (int)userBookings.size(), "Select booking to manage: ");
    Booking* selectedBooking = userBookings[choice - 1];

    cout << "\n1. Cancel Booking\n2. Modify Booking\n3. Back\nEnter choice: ";
    int action = getValidatedIntInput(1, 3, "");

    int movieIdx = theater.getMovieIndexByTitle(selectedBooking->getMovieTitle());

    if (action == 1) {
        theater.freeSeats(movieIdx, selectedBooking->getDate(), selectedBooking->getShowtime(), selectedBooking->getSeats());

        bookings.erase(remove_if(bookings.begin(), bookings.end(),
            [&](const Booking& b) { return b.getBookingId() == selectedBooking->getBookingId(); }),
            bookings.end());

        saveBookings(bookings);
        cout << "Booking canceled and seats freed.\n";
    }
    else if (action == 2) {
        theater.freeSeats(movieIdx, selectedBooking->getDate(), selectedBooking->getShowtime(), selectedBooking->getSeats());

        cout << "Modify booking seats.\n";
        theater.showSeats(movieIdx, selectedBooking->getDate(), selectedBooking->getShowtime());

        int numSeats = (int)selectedBooking->getSeats().size();
        cout << "Number of seats to book: " << numSeats << "\n";

        vector<string> newSeats;
        int booked = 0;
        while (booked < numSeats) {
            string seatId = getValidatedSeatInput(4, 5, "Enter new seat (e.g., A1): ");
            if (theater.bookSeat(movieIdx, selectedBooking->getDate(), selectedBooking->getShowtime(), seatId)) {
                newSeats.push_back(seatId);
                booked++;
            } else {
                cout << "Seat unavailable or invalid. Try again.\n";
            }
        }

        selectedBooking->setSeats(newSeats);
        double price = theater.getPriceByTitle(selectedBooking->getMovieTitle());
        selectedBooking->setTotalAmount(price * numSeats);

        saveBookings(bookings);
        cout << "Booking modified successfully.\n";
    }
    else {
        cout << "Returning to main menu.\n";
    }
}

// ---------------- BOOKING FUNCTION ----------------

void bookTickets(User& currentUser, Theater& theater, vector<Booking>& bookings) {
    cout << "\n--- Movie List ---\n";
    theater.displayMovies();

    int movieChoice = getValidatedIntInput(1, theater.getMoviesCount(), "Select movie by number: ") - 1;
    string movieTitle = theater.getMovieTitle(movieChoice);
    double price = theater.getPriceByTitle(movieTitle);

    // Date selection
    cout << "Select date:\n1. Today\n2. Tomorrow\n3. Custom Date (YYYY-MM-DD)\n";
    int dateChoice = getValidatedIntInput(1, 3, "Enter choice: ");
    string selectedDate;
    if (dateChoice == 1)
        selectedDate = getCurrentDate(0);
    else if (dateChoice == 2)
        selectedDate = getCurrentDate(1);
    else {
        cout << "Enter date (YYYY-MM-DD): ";
        getline(cin, selectedDate);
    }

    // Showtimes display
    cout << "\nAvailable showtimes:\n";
    int numShowtimes = theater.getShowtimesCount();
    for (int i = 0; i < numShowtimes; ++i) {
        cout << i + 1 << ". " << theater.getShowtime(i) << "\n";
    }
    int showtimeChoice = getValidatedIntInput(1, numShowtimes, "Select showtime: ") - 1;
    string showtime = theater.getShowtime(showtimeChoice);

    theater.showSeats(movieChoice, selectedDate, showtime);

    int numSeats = getValidatedIntInputMin(1, "Enter number of seats to book: ");

    vector<string> selectedSeats;
    int bookedSeats = 0;
    while (bookedSeats < numSeats) {
        string seatId = getValidatedSeatInput(4, 5, "Enter seat to book (e.g., A1): ");
        if (theater.bookSeat(movieChoice, selectedDate, showtime, seatId)) {
            selectedSeats.push_back(seatId);
            bookedSeats++;
        } else {
            cout << "Seat unavailable or invalid. Please try another seat.\n";
        }
    }

    double totalAmount = price * numSeats;

    Booking newBooking(currentUser.getUsername(), movieTitle, selectedDate, showtime);
    newBooking.setSeats(selectedSeats);
    newBooking.setTotalAmount(totalAmount);
    bookings.push_back(newBooking);
    saveBookings(bookings);

    cout << "\nBooking successful! Total: Rs. " << totalAmount << "\n";
    newBooking.generateReceipt();
}

// ---------------- MAIN ----------------

int main() {
    vector<Movie> movieList = {
        Movie("M001", "Coolie", "Action/Thriller", 145, 250, "https://youtu.be/qeVfT2iLiu0"),                     
        Movie("M002", "Superman", "Action/Superhero", 143, 350, "https://youtu.be/Ox8ZLF6cGM0"),              
        Movie("M003", "The Fantastic Four: First Steps", "Action/Adventure", 126, 300, "https://youtu.be/18QQWa5MEcs")

    };

    Theater theater("NSR CINEMA", movieList);
    vector<Booking> bookings = loadBookings();

    User user;
    bool loggedIn = false;

    while (!loggedIn) {
        cout << "\n1. Register\n2. Login\n3. Exit\nEnter choice: ";
        int choice = getValidatedIntInput(1, 3, "");
        if (choice == 1) {
            if (user.registerUser()) loggedIn = true;
        } else if (choice == 2) {
            if (user.loginUser()) loggedIn = true;
        } else {
            cout << "Goodbye!\n";
            return 0;
        }
    }

    while (true) {
        cout << "\n--- MAIN MENU ---\n";
        cout << "1. Book Tickets\n2. View Movies\n3. Play Trailer\n4. Manage Bookings\n5. Exit\n";

        int choice = getValidatedIntInput(1, 5, "Enter your choice: ");

        switch (choice) {
            case 1:
                bookTickets(user, theater, bookings);
                break;
            case 2:
                theater.displayMovies();
                break;
            case 3:
                cout << "Select movie to play trailer:\n";
                for (int i = 0; i < (int)movieList.size(); ++i) {
                    cout << i + 1 << ". " << movieList[i].getTitle() << "\n";
                }
                {
                    int idx = getValidatedIntInput(1, (int)movieList.size(), "Enter choice: ") - 1;
                    theater.playTrailer(idx);
                }
                break;
            case 4:
                manageBookings(user, theater, bookings);
                break;
            case 5:
                cout << "Thank you for using NSR CINEMA Booking System!\n";
                return 0;
        }
    }

    return 0;
}