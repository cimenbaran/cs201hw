#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "strutils.h"


using namespace std;

void routeDatabaseCheck(string &database, ifstream &input) {
	do {
		cout << "Please enter a filename for route database: ";
		cin >> database;
		input.open(database.c_str());
		if (database != "routes.txt" || input.fail() ) {cout << "cannot open routes database file" << endl;}
	} while (database != "routes.txt");
}

void timeDatabaseCheck(string &database, ifstream &input) {
	do {
		cout << "Please enter a filename for time database: ";
		cin >> database;
		input.open(database.c_str());
		if (database != "times.txt" || input.fail()) {cout << "cannot open times database file" << endl;}
	} while (database != "times.txt");
}

bool timeCheck(string &time) {
	cout << "Please enter start time of travel: ";
	cin >> time;
	if (time.length() == 5 && time.at(2) == ':' && time.at(0) <= '9' && time.at(0) >= '0' && time.at(1) <= '9' && time.at(1) >= '0' && time.at(3) <= '9' && time.at(3) >= '0' && time.at(4) <= '9' && time.at(4) >= '0')
	{
		int hour = atoi(time.substr(0,2)), minute = atoi(time.substr(3,2));
		if (hour < 24 && hour >= 0 && minute < 60 && minute >= 0) {
			return true;
		}
	}
	return false;
}

bool isTimeOkay(string time, string clock) {
	int timeH = atoi(time.substr(0,2)),
		timeM = atoi(time.substr(3,2)),
		clockH = atoi(clock.substr(0,2)),
		clockM = atoi(clock.substr(3,2));
	if (timeH < clockH) {return true;}
	else if (timeH == clockH && timeM <= clockM) {return true;}
	else {return false;}
}



int main() {
	ifstream inputRoute, inputTime;
	string routeDatabase = "", timeDatabase = "", departure = "", time = "", s, id, from, to, t, ids, clock;
	routeDatabaseCheck(routeDatabase, inputRoute);
	timeDatabaseCheck(timeDatabase, inputTime);
	cout << "Please enter departure location: ";
	cin >> departure;
	while (departure != "EXIT") {
		while (!(timeCheck(time))) { // time check
			cout << "Time is not in correct format" << endl;
		}
		// do everything
		cout << "The search results are: " << endl;
		while (getline(inputRoute, s)) {
			istringstream input(s);
			input >> id >> from >> to;
			if (from == departure) {
				
				while (getline(inputTime,t)) {
					istringstream input2(t);
					input2 >> ids >> clock;
					if (ids == id && isTimeOkay(time,clock)) {
						cout << from << " " << to << " " << clock << endl;
					}
				}
				inputTime.clear();
				inputTime.seekg(0);
			}
		}
		inputRoute.clear();
		inputRoute.seekg(0);
		cout << "Please enter departure location: ";
		cin >> departure;
	}
	return 0;
} 