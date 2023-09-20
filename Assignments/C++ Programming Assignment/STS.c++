#include <iostream>
#include <math.h>
#include <set>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

/* 
    This software assumes that astronauts and commanders do not have the right to make bookings. They are assigned journeys
    by the system. Passengers may book tickets however they need not be assigned a space travel based on the availability of
    a comamnder or an astronaut.
*/

/* 
    Also, the validity is assumed to expire at the end of the year. So, suppose that I register on 13/03/2023, my validity
    will expire on 31/12/2033
*/

struct coordinates {
    int x, y, z;
};

struct date {
    int dd, mm, yyyy;
};


//The implementation of Traveller class with all its members
class Traveller {
    protected:
        string name;
        int id;
        int type;
        int system_id;
        vector<int> associated_ticket_ids;
        /* type = 0 : Passenger
           type = 1 : Astronaut
           type = 2 : Commander */
    
    public:
        static int traveller_count;
        int getID();
        string getName();
        int getType();
        int getSystemID();
        void setID(int id);
        void setName(string name);
        void addTicketID(int ticket_id);
        void removeTicketID(int ticket_id);
        vector<int> getAssociatedTicketIDs() {
            return this->associated_ticket_ids;
        }
        
        
        static int updateTravellerCount() { 
            return ++traveller_count; 
        }

        Traveller() {
            this->name = "Unknown";
            this->id = 0;
            this->type = -1;
        }

        Traveller(string name, int id, int type);
};

Traveller::Traveller(string name, int id, int type) {
    this->name = name;
    this->id = id;
    this->type = type;
    this->system_id = traveller_count;
    updateTravellerCount();
}


int Traveller::traveller_count = 0;

int Traveller::getID() {
    return id;
}

string Traveller::getName() {
    return name;
}

int Traveller::getType() {
    return type;
}

int Traveller::getSystemID() {
    return system_id;
}

void Traveller::setID(int id) {
    this->id = id;
}

void Traveller::setName(string name) {
    this->name = name;
}

void Traveller::addTicketID(int ticket_id) {
    this->associated_ticket_ids.push_back(ticket_id);
}

void Traveller::removeTicketID(int ticket_id) {
    for (int i = 0; i < this->associated_ticket_ids.size(); i++) {
        if (this->associated_ticket_ids[i] == ticket_id) {
            this->associated_ticket_ids.erase(this->associated_ticket_ids.begin() + i);
            break;
        }
    }
}


//The implementation of passenger class as an inherited class of Traveller class
class Passenger: public Traveller {
    private:
        date validity;
    
    public:
        Passenger(string name, int id, date d);
        date getValidity();
        Passenger() {
            this->name = "Unknown";
            this->id = 0;
            this->type = 0;
        }
};

Passenger::Passenger(string name, int id, date registration_date) {
    this->type = 0;
    this->name = name;
    this->id = id;
    this->validity = registration_date;
    this->validity.yyyy += 10; //valid for 10 years after registration
    this->system_id = traveller_count;
    updateTravellerCount();
}

date Passenger::getValidity() {
    return this->validity;
}


class Astronaut: public Traveller {
    private:
        const int registration_year;
        int years_of_experience;
        const int license_ID; // must be unique
        const string space_travel_license;

        public:
            static int astronaut_count;
            static int updateAstronautCount() { 
                return ++astronaut_count; 
            }

            int getYearsOfExperience();
            int getLicenseID(); 
            string getSpaceTravelLicense();
            Astronaut(string name, int id, int registration_year);
};
int Astronaut::astronaut_count = 0;


int Astronaut::getYearsOfExperience() {
    time_t current_time = time(nullptr);
    tm *const pTInfo = localtime(&current_time);
    int current_year = 1900 + pTInfo->tm_year;
    this->years_of_experience = current_year - this->registration_year;
    return this->years_of_experience;
}

int Astronaut::getLicenseID() {
    return license_ID;
}

string Astronaut::getSpaceTravelLicense() {
    return space_travel_license;
}

Astronaut::Astronaut(string name, int id, int registration_year): 
license_ID(astronaut_count), 
space_travel_license("STC" + to_string(this->license_ID)), 
registration_year(registration_year) {
    this->type = 1;
    this->name = name;
    this->id = id;
    updateAstronautCount();
    this->system_id = traveller_count;
    updateTravellerCount();
}

class Commander: public Traveller {
    private:
        const int registration_year;
        int years_of_experience;
        const int license_ID; // must be unique
        const string space_travel_license;
        bool authority;

        public:
            static int commander_count;
            static int updateCommanderCount() { 
                return ++commander_count; 
            }
            int getYearsOfExperience();
            int getLicenseID();
            string getSpaceTravelLicense();
            void changeAuthority();
            Commander(string name, int id, int registration_year, bool authority);
};
int Commander::commander_count = 0;

int Commander::getYearsOfExperience() {
    time_t current_time = time(nullptr);
    tm *const pTInfo = localtime(&current_time);
    int current_year = 1900 + pTInfo->tm_year;
    this->years_of_experience = current_year - this->registration_year;
    return this->years_of_experience;
}

int Commander::getLicenseID() {
    return license_ID;
}

string Commander::getSpaceTravelLicense() {
    return space_travel_license;
}

void Commander::changeAuthority() {
    this->authority = !this->authority;
    if(this->authority) {
        cout << "Congratulations " << this->name << "! you have been granted the authority to command a spaceship" << endl;
    }
    else {
        cout << "Sorry " << this->name << "! you have been stripped off the authority to command a spaceship" << endl;
    }
}

Commander::Commander(string name, int id, int registration_year, bool authority): 
license_ID(commander_count), 
space_travel_license("STCCOM" + to_string(this->license_ID)),
registration_year(registration_year) {
    this->type = 2;
    this->name = name;
    this->id = id;
    this->authority = authority;
    updateCommanderCount();
    this->system_id = traveller_count;
    updateTravellerCount();
}


class Planet {
    public:
        coordinates location;
        string name;
        
        string getName();
        coordinates getCoordinates();
        void setCoordinates(coordinates location);

        Planet() {
            coordinates default_location;
            string default_name = "Black Hole";

            default_location.x = default_location.y = default_location.z = 0;

            this->location = default_location;
            this->name = default_name;
        }

        Planet(string name, coordinates loc);
};

Planet::Planet(string name, coordinates loc) {
    this->name = name;
    this->location = loc;
}

string Planet::getName() {
    return name;
}

coordinates Planet::getCoordinates() {
    return location;
}

void Planet::setCoordinates(coordinates location) {
    this->location = location;
    cout << "Location of planet " << this->name << " has been updated" << endl;
}

long long int getDifference(date d1, date d2) {
    long long int difference_dates;
    difference_dates = (d1.yyyy - d2.yyyy) * 365 + (d1.mm - d2.mm) * 30 + (d1.dd - d2.dd);
    return difference_dates;
}


//The implementation of Ticket class with all its members
class Ticket {
    private:
        float price;
    
    public:
        Planet source;
        Planet destination;
        date date_booked;
        date date_travel;
        bool has_return_ticket;
        vector<int> traveller_system_ids;
        int ticket_system_id;
        static int ticket_count;
        Ticket *return_ticket;

        static int updateTicketCount() { 
            return ++ticket_count; 
        }
        float getPrice();
        void addTraveller(int traveller_system_id);
        Planet getSource();
        Planet getDestination();
        date getDateBooked();
        date getDateTravel();
        bool getHasReturnTicket();
        vector<int> getTravellerSystemIDs();
        int getTicketSystemID();
        void setReturnTicket(Ticket *return_ticket);
        void setSource(string s);
        void setDestination(string d);
        void setTravelDate(date dt);

        Ticket(Planet s, Planet d, date db, date dt, bool hrt, int traveller_system_id) {
            this->source = s;
            this->destination = d;
            this->date_booked = db;
            this->date_travel = dt;
            this->has_return_ticket = hrt;
            this->traveller_system_ids.push_back(traveller_system_id);
            // this->ticket_system_id = updateTicketCount();
            // updateTicketCount();

            cout << "Ticket object has been instantiated" << endl;

            double k;
            long long int difference_dates;
            coordinates source_location = s.location;
            coordinates destination_location = d.location;
            k = sqrt(pow(source_location.x - destination_location.x, 2) +
                     pow(source_location.y - destination_location.y, 2) +
                     pow(source_location.z - destination_location.z, 2));
            difference_dates = getDifference(dt, db);


            this->price = (k * 1000) / ( difference_dates + 1);
        }
};


float Ticket::getPrice() {
    return price;
}

void Ticket::addTraveller(int traveller_system_id) {
    this->traveller_system_ids.push_back(traveller_system_id);
}

Planet Ticket::getSource() {
    return source;
}

Planet Ticket::getDestination() {
    return destination;
}

date Ticket::getDateBooked() {
    return date_booked;
}

date Ticket::getDateTravel() {
    return date_travel;
}

bool Ticket::getHasReturnTicket() {
    return has_return_ticket;
}

vector<int> Ticket::getTravellerSystemIDs() {
    return traveller_system_ids;
}

void Ticket::setReturnTicket(Ticket *return_ticket) {
    this->return_ticket = return_ticket;
}

int Ticket::getTicketSystemID() {
    return ticket_system_id;
}

vector<Traveller> traveller_list;
vector<Passenger> passenger_list;
vector<Astronaut> astronaut_list;
vector<Commander> commander_list;
vector<Planet> planet_list;
vector<Ticket> ticket_list;


void Ticket::setSource(string s) {
    for(auto i: planet_list) {
        if(i.getName() == s) {
            this->source = i;
            break;
        }
    }
}

void Ticket::setDestination(string d) {
    for(auto i: planet_list) {
        if(i.getName() == d) {
            this->destination = i;
            break;
        }
    }
}

void Ticket::setTravelDate(date dt) {
    this->date_travel = dt;
}

// unordered_map<Passenger, vector<Ticket>> passenger_tickets;
// unordered_map<Astronaut, vector<Ticket>> astronaut_tickets;
// unordered_map<Commander, vector<Ticket>> commander_tickets;

class SpaceTravelClass {
    private:
        Traveller traveller_list[10];
        int traveller_count;
        Astronaut* A;
        Commander* C;
        Planet source;
        Planet destination;
        date date_travel;

    public:
        Planet getSource();
        Planet getDestination();
        void addTraveller(Traveller t);
        void removeTraveller(Traveller t); //when traveller deletes a ticket
        void viewAllTravellers();
        void setAstronaut(Astronaut* A);
        void setCommander(Commander* C);

        date getDateTravel();
        bool getHasAstronaut();
        bool getHasCommander();

        SpaceTravelClass(Planet s, Planet d, date dt, Astronaut* A, Commander* C, Passenger p1, Passenger p2) {
            this->source = s;
            this->destination = d;
            this->date_travel = dt;
            this->A = A;
            this->C = C;
            this->traveller_list[0] = *A;
            this->traveller_list[1] = *C;
            this->traveller_list[2] = p1;
            this->traveller_list[3] = p2;
            this->traveller_count = 4;
        }
};

Planet SpaceTravelClass::getSource() {
    return source;
}

Planet SpaceTravelClass::getDestination() {
    return destination;
}

void SpaceTravelClass::addTraveller(Traveller t) {
    this->traveller_list[this->traveller_count++] = t;
}

void SpaceTravelClass::removeTraveller(Traveller t) {
    for(int i = 0; i < this->traveller_count; i++) {
        if(this->traveller_list[i].getName() == t.getName()) {
            for(int j = i; j < this->traveller_count - 1; j++) {
                this->traveller_list[j] = this->traveller_list[j + 1];
            }
            this->traveller_count--;
            break;
        }
    }
}

vector<SpaceTravelClass> space_travel_class_list;

void mainMenu();
void Dashboard(Passenger p);


Planet findPlanet(string planet_name) {
    for(int i = 0; i < planet_list.size(); i++) {
        if(planet_list[i].getName() == planet_name) {
            return planet_list[i];
        }
    }
}

Passenger findPassenger(int system_id) {
    for(int i = 0; i < passenger_list.size(); i++) {
        if(passenger_list[i].getSystemID() == system_id) {
            return passenger_list[i];
        }
    }
}

Astronaut findAstronaut(int system_id) {
    for(int i = 0; i < astronaut_list.size(); i++) {
        if(astronaut_list[i].getSystemID() == system_id) {
            return astronaut_list[i];
        }
    }
}

Commander findCommander(int system_id) {
    for(int i = 0; i < commander_list.size(); i++) {
        if(commander_list[i].getSystemID() == system_id) {
            return commander_list[i];
        }
    }
}

Ticket findTicket(int ticket_id) {
    for(int i = 0; i < ticket_list.size(); i++) {
        if(ticket_list[i].getTicketSystemID() == ticket_id) {
            return ticket_list[i];
        }
    }
}

void bookTicket(Passenger p) {
    date date_booked;
    date validity = p.getValidity();
    cout << "Please enter today's date in the format dd (space) mm (space) yyyy: " << endl;
    cin >> date_booked.dd >> date_booked.mm >> date_booked.yyyy;

    if(validity.yyyy < date_booked.yyyy) {
        cout << "Sorry, your validity has expired! Please login as another user to use the application" << endl;
        cout << endl;
        mainMenu();
    }
    else {
        string source_planet;
        cout << "Please enter your source planet: ";
        cin >> source_planet;
        cout << endl;
        
        string destination_planet;
        cout << "Please enter your destination planet: ";
        cin >> destination_planet;
        cout << endl;

        date date_travel;
        cout << "Please enter the date of travel in the format dd (space) mm (space) yyyy: ";
        cin >> date_travel.dd >> date_travel.mm >> date_travel.yyyy;
        cout << endl;

        if(date_travel.yyyy < date_booked.yyyy) {
            cout << "Sorry, you cannot travel back in time! Please enter a valid date of travel" << endl;
            cout << endl;
            bookTicket(p);
        }
        else if(date_travel.yyyy == date_booked.yyyy) {
            if(date_travel.mm < date_booked.mm) {
                cout << "Sorry, you cannot travel back in time! Please enter a valid date of travel" << endl;
                cout << endl;
                bookTicket(p);
            }
            else if(date_travel.mm == date_booked.mm) {
                if(date_travel.dd < date_booked.dd) {
                    cout << "Sorry, you cannot travel back in time! Please enter a valid date of travel" << endl;
                    cout << endl;
                    bookTicket(p);
                }
            }
        }
        else {
            cout << "Do you wish to book a return ticket? (Y/N) *Case sensitive";
            char return_ticket;
            int has_return_ticket;
            Planet source, destination;
            source = findPlanet(source_planet);
            destination = findPlanet(destination_planet);

            if(source.getName() == "NULL" || destination.getName() == "NULL") {
                cout << "Sorry, one of the planets you entered does not exist! Please enter a valid planet name" << endl;
                cout << endl;
                bookTicket(p);
            }

            cin >> return_ticket;
            cout << endl;
            if(return_ticket == 'Y') {
                Ticket t1(source, destination, date_booked, date_travel, 1, p.getSystemID());
                Ticket t2(destination, source, date_booked, date_travel, 1, p.getSystemID());
                t1.setReturnTicket(&t2);
                t2.setReturnTicket(&t1);
                ticket_list.push_back(t1);
                ticket_list.push_back(t2);
            }
            else {
                Ticket t1(source, destination, date_booked, date_travel, 0, p.getSystemID());
                ticket_list.push_back(t1);
            }

            //check if space travel class already exists with same source and destination and travel date. If it does, add the traveller to it.
            for(auto i: space_travel_class_list) {
                if(i.getDateTravel().dd == date_travel.dd && i.getDateTravel().mm == date_travel.mm && i.getDateTravel().yyyy == date_travel.yyyy && i.getSource().getName() == source_planet && i.getDestination().getName() == destination_planet) {
                    i.addTraveller(p);
                    return;
                }
            }

            //check if a ticket already exists from given source to given destination on the same day. If it does, instantiate an object of the class SpaceTravelClass and add the travellers to it.
            //if it doesn't, create a new ticket object and add it to the ticket_list vector
            for(auto i: ticket_list) {
                if(i.getDateTravel().dd == date_travel.dd && i.getDateTravel().mm == date_travel.mm && i.getDateTravel().yyyy == date_travel.yyyy && i.getSource().getName() == source_planet && i.getDestination().getName() == destination_planet) {
                    int id1 = i.getTravellerSystemIDs()[0];
                    int id2 = i.getTravellerSystemIDs()[1];
                    Passenger p1 = findPassenger(id1);
                    Passenger p2 = findPassenger(id2);
                    Astronaut A = astronaut_list[0];
                    Commander C = commander_list[0];
                    Planet source = i.getSource();
                    Planet destination = i.getDestination();
                    SpaceTravelClass s(source, destination, date_travel, &A, &C, p1, p2);
                    i.addTraveller(p.getSystemID());
                    break;
                }
            }

            cout << "Ticked booked succesfully!";
        }
    }
    Dashboard(p);
}

void viewTickets(Passenger p) {
    for(auto i: ticket_list) {
        for(auto j: i.getTravellerSystemIDs()) {
            if(j == p.getSystemID()) {
                cout << i.getTicketSystemID() << ", the travel date is " << i.getDateTravel().dd << "/" << i.getDateTravel().mm << "/" << i.getDateTravel().yyyy << ", the source is " << i.getSource().getName() << ", the destination is " << i.getDestination().getName() << endl;
            }
        }
    }
    Dashboard(p);
}

void updateTicket(Passenger p) {
    string s, d;
    cout << "Which ticket would you like to update? Enter the ticket ID: ";
    viewTickets(p);
    int ticket_id;
    cin >> ticket_id;
    cout << endl;

    cout << "What would you like to update?" << endl;
    cout << "1. Source planet" << endl;
    cout << "2. Destination planet" << endl;
    cout << "3. Date of travel" << endl;
    int update_option;
    cin >> update_option;
    cout << endl;

    switch(update_option) {
        case 1:
            cout << "Enter the new source planet: ";
            cin >> s;
            cout << endl;
            for(auto i: ticket_list) {
                if(i.getTicketSystemID() == ticket_id) {
                    i.setSource(s);
                }
            }
        break;

        case 2:
            cout << "Enter the new destination planet: ";
            cin >> d;
            cout << endl;
            for(auto i: ticket_list) {
                if(i.getTicketSystemID() == ticket_id) {
                    i.setDestination(d);
                }
            }
        break;

        case 3:
            cout << "Enter the new date of travel: ";
            date date;
            cin >> date.dd >> date.mm >> date.yyyy;
            cout << endl;
            for(auto i: ticket_list) {
                if(i.getTicketSystemID() == ticket_id) {
                    i.setTravelDate(date);
                }
            }
        break;

        default:
            cout << "Invalid option! Please enter a valid option" << endl;
            cout << endl;
            updateTicket(p);
        break;
    }
    Dashboard(p);
}

void deleteTicket(Passenger p) {
    cout << "Which ticket would you like to delete? Enter the ticket ID: ";
    viewTickets(p);
    int ticket_id;
    cin >> ticket_id;
    cout << endl;

    for(auto i: p.getAssociatedTicketIDs()) {
        if(i == ticket_id) {
            p.removeTicketID(ticket_id);
        }
    }
    cout << "Ticket succesfully deleted!";
    Dashboard(p);

}

void Dashboard(Passenger p) {
    int input_option;

    cout << "What would you like to do today?" << endl;
    cout << "Enter a number corresponding to the choice" << endl;
    cout << "1. Book a ticket" << endl;
    cout << "2. View all tickets" << endl;
    cout << "3. Update a ticket" << endl;
    cout << "4. Delete a ticket" << endl;
    cout << "5. Get your validity" << endl;
    cout << "6. Exit and login as another user" << endl;
    cout << "7. Exit the program" << endl;
    cin >> input_option;

    switch(input_option) {
        case 1:
            bookTicket(p);
        break;

        case 2:
            viewTickets(p);
        break;

        case 3:
            updateTicket(p);
        break;

        case 4:
            deleteTicket(p);
        break;

        case 5:
            cout << "Your validity is " << p.getValidity().dd << "/" << p.getValidity().mm << p.getValidity().yyyy << endl;
            cout << endl;
            Dashboard(p);
        break;

        case 6:
            mainMenu();
        break;

        case 7:
            return;
        break;

        default:
            cout << "Please enter a valid option" << endl;
            cout << endl;
            Dashboard(p);
        break;
    }
}

void mainMenu() {
    string name;
    Passenger current_user;
    int found = 0, input_option;
    cout << "Welcome to the Space Travel Agency, passenger!" << endl;
    cout << "Please enter your username" << endl;
    cin >> name;

    for(int i = 0; i < passenger_list.size(); i++) {
        if(passenger_list[i].getName() == name) {
            cout << "Passenger found!" << endl;
            current_user = passenger_list[i];
            found = 1;
            break;
        }
    }
    if(!found) {
        cout << "Please enter a valid username" << endl;
        cout << endl;
        mainMenu();
    }
    else {
        cout << "We are glad to have you here " << current_user.getName() << endl;
        cout << endl;
        Dashboard(current_user);
    }
}


int main() {

    //creating 10 passengers
    Passenger Rahul("Rahul", 1, {1, 1, 2016});
    Traveller t1(Rahul.getName(), Rahul.getID(), Rahul.getType());
    passenger_list.push_back(Rahul);
    traveller_list.push_back(t1);

    Passenger Raj("Raj", 2, {1, 1, 2010});
    Traveller t2(Raj.getName(), Raj.getID(), Raj.getType());
    passenger_list.push_back(Raj);
    traveller_list.push_back(t2);

    Passenger Rohan("Rohan", 3, {1, 1, 2015});
    Traveller t3(Rohan.getName(), Rohan.getID(), Rohan.getType());
    passenger_list.push_back(Rohan);
    traveller_list.push_back(t3);

    Passenger Rohit("Rohit", 1, {1, 10, 2020});
    Traveller t4(Rohit.getName(), Rohit.getID(), Rohit.getType());
    passenger_list.push_back(Rohit);
    traveller_list.push_back(t4);

    Passenger Rajbongshi("Rajbongshi", 10, {3, 1, 2023});
    Traveller t5(Rajbongshi.getName(), Rajbongshi.getID(), Rajbongshi.getType());
    passenger_list.push_back(Rajbongshi);
    traveller_list.push_back(t5);

    Passenger Rajesh("Rajesh", 5, {30, 9, 2019});
    Traveller t6(Rajesh.getName(), Rajesh.getID(), Rajesh.getType());
    passenger_list.push_back(Rajesh);
    traveller_list.push_back(t6);

    Passenger Rajat("Rajat", 7, {1, 1, 2018});
    Traveller t7(Rajat.getName(), Rajat.getID(), Rajat.getType());
    passenger_list.push_back(Rajat);
    traveller_list.push_back(t7);

    Passenger Rajeshwari("Rajeshwari", 8, {1, 1, 2017});
    Traveller t8(Rajeshwari.getName(), Rajeshwari.getID(), Rajeshwari.getType());
    passenger_list.push_back(Rajeshwari);
    traveller_list.push_back(t8);

    Passenger Rajkumar("Rajkumar", 9, {15, 6, 2020});
    Traveller t9(Rajkumar.getName(), Rajkumar.getID(), Rajkumar.getType());
    passenger_list.push_back(Rajkumar);
    traveller_list.push_back(t9);

    Passenger Rajani("Rajani", 10, {1, 1, 2016});
    Traveller t10(Rajani.getName(), Rajani.getID(), Rajani.getType());
    passenger_list.push_back(Rajani);
    traveller_list.push_back(t10);


    //creating 3 astronauts
    Astronaut Harsh("Harsh", 3, 2016);
    Traveller t11(Harsh.getName(), Harsh.getID(), Harsh.getType());
    astronaut_list.push_back(Harsh);
    traveller_list.push_back(t11);

    Astronaut Hamim("Hamim", 6, 2017);
    Traveller t12(Hamim.getName(), Hamim.getID(), Hamim.getType());
    astronaut_list.push_back(Hamim);
    traveller_list.push_back(t12);

    Astronaut Hamza("Hamza", 12, 2020);
    Traveller t13(Hamza.getName(), Hamza.getID(), Hamza.getType());
    astronaut_list.push_back(Hamza);
    traveller_list.push_back(t13);

    //creating 2 commanders
    Commander Siddhant("Siddhant", 1, 2016, 1);
    Traveller t14(Siddhant.getName(), Siddhant.getID(), Siddhant.getType());
    commander_list.push_back(Siddhant);
    traveller_list.push_back(t14);

    Commander Sumit("Siddharth", 2, 2017, 0);
    Traveller t15(Sumit.getName(), Sumit.getID(), Sumit.getType());
    commander_list.push_back(Sumit);
    traveller_list.push_back(t15);


    //creating 3 planets
    Planet Earth("Earth", {0, 0, 0});
    Planet Mars("Mars", {10, -10, 5});
    Planet Jupiter("Jupiter", {25, 25, 25});
    planet_list.push_back(Earth);
    planet_list.push_back(Mars);
    planet_list.push_back(Jupiter);

    mainMenu();

    return 0;
}