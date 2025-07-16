
#include<iostream>
#include<iomanip>
#include<string>
#include<sstream> 
#include <cstdlib>  // For processing input strings
using namespace std;

class Cinema {
protected:
    string movieName[5] = {"COOLIE", "JANA NAYAGAN", "RAMAYANA PART 1", "AVATAR: THE NEW AGE", "FAST 10: THE RETURN OF HOBBS"};
public:
    void displayMovie() {
        cout << "----- Now Playing -----\n";
        for (int i = 0; i < 5; i++) {
            cout << setw(4) << i + 1 << ". " << movieName[i] << endl;
        }
    }
    void url(int choice){

    	
    	if (choice==1){
    		cout << "Opening the Teaser/Trailer of COOLIE : " << endl;
    		
            system("start https://youtu.be/6xqNk5Sf5jo?list=RD6xqNk5Sf5jo");
		}
		else if (choice==2){
		    cout<<"Opening the Teaser/Trailer of JANA NAYAGAN : "<< endl;
		    system("start https://youtu.be/MKUDHKf_pkg?list=RDMKUDHKf_pkg");
		}
		
		else if(choice==3){
		    cout<<"Opening the Teaser/Trailer of RAMAYANA PART 1 : "<< endl;
		    system("start https://youtu.be/gzUu-FJ7s-Y?list=RDgzUu-FJ7s-Y");
		}
		else if(choice==4){
		    cout<<"Opening the Teaser/Trailer of AVATAR:THE NEW AGE : "<< endl;
		    system("start https://youtu.be/d9MyW72ELq0");
		    
		}
		else if(choice==5){
		    cout<<"Opening the Teaser/Trailer of FAST 10:THE RETURN OF HOBBS : "<< endl;
		    system("start https://youtu.be/HZ7PAyCDwEg");
		    
		}
		
		else{
			cout<<"Enter proper choice (1-5): "<<endl;
			
		}
	}
};

class Seats : public Cinema {
private:
    char seatCol[4] = {'A', 'B', 'C', 'D'}; 
    bool seats[4][5] = {{false, false, false, false, false}, 
                        {false, false, false, false, false},
                        {false, false, false, false, false},
                        {false, false, false, false, false}}; 

public:
    void movie(int n) {
        cout << "Selected Movie: " << movieName[n - 1] << endl;
        cout << "Available seats:\n";

        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 5; col++) {
                if (seats[row][col] == false)
                    cout << seatCol[row] << col + 1 << "( ) ";
                else
                    cout << seatCol[row] << col + 1 << "(X) "; 
            }
            cout << endl;
        }
    }

    bool chooseSeat(char r, int c) {
        int rowIndex = r - 'A';
        if (rowIndex < 0 || rowIndex >= 4 || c < 1 || c > 5) {
            cout << "Invalid seat choice: " << r << c << ". Please try again.\n";
            return false;
        }
        if (seats[rowIndex][c - 1]) { 
            cout << "Sorry, seat " << r << c << " is already booked.\n";
            return false;
        } else {
            seats[rowIndex][c - 1] = true; 
            return true;
        }
    }

    void bookingConfirmation(char r, int c) {
        cout << "Seat " << r << c << " has been successfully booked!\n";
    }

    void viewBookings() {
        cout << "----- Current Bookings -----\n";
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 5; col++) {
                if (seats[row][col] == false)
                    cout << seatCol[row] << col + 1 << "( ) ";  
                else
                    cout << seatCol[row] << col + 1 << "(X) ";  
            }
            cout << endl;
        }
    }


    void bookMultipleSeats(char seatRows[], int seatCols[], int numSeats) {
        bool validBooking = true;
        for (int i = 0; i < numSeats; ++i) {
            char row = seatRows[i];
            int col = seatCols[i];
            if (!chooseSeat(row, col)) {
                validBooking = false;
                break;
            }
        }

        if (validBooking) {
            for (int i = 0; i < numSeats; ++i) {
                bookingConfirmation(seatRows[i], seatCols[i]);
            }
        } else {
            cout << "Some seats could not be booked. Please try again with available seats.\n";
        }
    }
};

int main() {
    cout << "Welcome to NSR Cinema\n";

    Cinema c1;
    Seats se;  
    int choice;
    char seatRow;
    int seatCol;
    bool validSeat = false;

    while (true) {
        cout << "\n----- Menu -----\n";
        cout << "1. View Movies\n";
        cout << "2. Book a Ticket\n";
        cout << "3. View Booked Seats\n";
        cout << "4. Exit\n";
        cout << "5. Watch Teaser/Trailer \n";
        cout << "Enter your choice: ";
        
        cin >> choice;

        switch (choice) {
            case 1: {
                c1.displayMovie();
                break;
            }

            case 2: {
                cout << "Choose a movie (1-5): ";
                cin >> choice;

                if (choice < 1 || choice > 5) {
                    cout << "Invalid choice. Please select a valid movie.\n";
                    break;
                }

                se.movie(choice);  
                cout << "Enter seats to book (e.g., A1 A2 B3): ";
                cin.ignore();  
                string input;
                getline(cin, input);  

                stringstream ss(input);
                char seatRows[10];  
                int seatCols[10];   
                int numSeats = 0;

                while (ss >> seatRow >> seatCol) {
                    if (seatCol < 1 || seatCol > 5 || seatRow < 'A' || seatRow > 'D') {
                        cout << "Invalid seat input detected. Please try again.\n";
                        numSeats = 0;  
                        break;
                    }
                    seatRows[numSeats] = seatRow;
                    seatCols[numSeats] = seatCol;
                    numSeats++;
                }

              
                if (numSeats > 0) {
                    se.bookMultipleSeats(seatRows, seatCols, numSeats);
                }

                break;
            }

            case 3: {
                se.viewBookings();  
                break;
            }

            case 4: {
                cout << "Thank you for visiting NSR Cinema! Goodbye.\n";
                return 0;
            }

			case 5: {
                int movieChoice;
                bool validTrailerChoice = false;

                while (!validTrailerChoice) {
                    c1.displayMovie();
                    cout << "Enter the movie number to watch the trailer (1�5): ";
                    cin >> movieChoice;

                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore(1000, '\n');
                        cout << "Invalid movie number. Please enter a number between 1 and 5.\n";
                        continue;  
                    }

                    if (movieChoice < 1 || movieChoice > 5) {
                        cout << "Movie number out of range. Please choose between 1 and 5.\n";
                        continue;  
                    }

                    c1.url(movieChoice);
                    validTrailerChoice = true;  
                }

                break;
            }


            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}
