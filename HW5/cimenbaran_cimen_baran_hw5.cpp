#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;



struct student{
	double grade;
	string name, id;
};


// Prototype of Functions
void vectorSorting(vector<student> & myVec);
bool gradesDatabaseCheck(string &database, ifstream &input);
bool studentsDatabaseCheck(string &database, ifstream &input);
vector<student> createDatabase(ifstream &students, ifstream &grades);


void vectorSorting(vector<student> & myVec){ //sorts the vector in decreasing order
    int sizeofVec = myVec.size();
    for(int j=0; j < sizeofVec ; j++) {
        int maxIndex = j;            
		for(int i=j; i < sizeofVec; i++) {
			if (myVec[i].grade > myVec[maxIndex].grade) {maxIndex = i;}}
        student temp = myVec[j];        
        myVec[j] = myVec[maxIndex];
        myVec[maxIndex] = temp;
		
	}
}

bool gradesDatabaseCheck(string &database, ifstream &input) { //grades file name input and check
	cout << "Please enter a filename for Students Grades Results: ";
	cin >> database;
	input.open(database.c_str());
	if (input.fail() ) {cout << "Can not find the requested file. Terminating application ..." << endl; return false;}
	else {return true;}
}

bool studentsDatabaseCheck(string &database, ifstream &input) { //students file name input and check
	cout << "Please enter a filename for Students Names : ";
	cin >> database;
	input.open(database.c_str());
	if (input.fail()) {cout << "Can not find the requested file. Terminating application ..." << endl; return false;}
	else {return true;}
}

vector<student> createDatabase(ifstream &students, ifstream &grades) { // creates a vector of students and returns it
	string line, line2;
	vector<student> stuDatabase;
	while(getline(students,line)) { // students file 
		int hms = 0, mids = 0, fins= 0;
		string id, name, tempname, which;
		student temp;
		istringstream input(line);
		input >> id >> name; // line is "id name middlename surname more-name more-more-name..."
		while (input >> tempname) {name += " " + tempname;} // take all names
		input.clear(); // in case of any error flags
		input.ignore();
		while(getline(grades,line2)) { // grades file
			string id2;
			double grade;
			istringstream input2(line2);
			if(line2 == "***HOMEWORK***") {which = "homework";} // which will be use to determine what the grade is
			else if (line2 == "***MIDTERM***") {which = "midterm";}
			else if (line2 == "***FINAL***") {which = "final";}
			else {
				input2 >> id2;
				if (id == id2) {
					input2 >> grade;
					input2.ignore(); // in case of any error flags
					if(which == "homework") {hms += grade;} // adding the grade to corresponding type
					else if(which == "midterm") {mids += grade;}
					else if(which == "final") {fins += grade;} 
					else {}
			
				}
			}
		}
		grades.clear(); // clears the error flag of grades ifstream 
		grades.seekg(0); // returns to the start of grades so it can read it again
		double totalGrade = 0;
		totalGrade = (hms/10.0) + (mids/5.0) + (fins/(10/3.0)); // find the total grade with the grades
		temp.id = id;
		temp.name = name;
		temp.grade = totalGrade;
		if(temp.grade != 0) {stuDatabase.push_back(temp);} // add to the vector
	}
	return stuDatabase;
}


int main() {
	string gradesDatabase, studentsDatabase;
	int query ;
	ifstream grades, students;
	if(!gradesDatabaseCheck(gradesDatabase, grades)) {} // do nothing if unable to open grades file, it prints error message in the func
	else if(!studentsDatabaseCheck(studentsDatabase, students)) {} // do nothing if unable to open students file, it prints error message in the func
	else {
		vector<student> stuDatabase = createDatabase(students, grades);
		vectorSorting(stuDatabase);
		cout << "Enter the rank you want to query (enter 0 to exit): ";
		cin >> query;
		while (query != 0 ) { // until user enters 0
			
			if(query <stuDatabase.size()+1 && query > 0) { // input check
				int qur = query - 1; // index
				cout << query << ", " << stuDatabase[qur].id << ", " << stuDatabase[qur].name << ", " << stuDatabase[qur].grade << endl;
			} else {
				cout << "Rank needs to be greater than 0 and smaller than " << stuDatabase.size()+1 <<"!" << endl;
			}

			cout << "Enter the rank you want to query (enter 0 to exit): ";
			cin >> query;
		}
		cout << "Exiting..." << endl;
	}
	return 0;
}
