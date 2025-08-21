#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

const int MAX_PLAYER = 20;

map<string, int> idMap;
vector<string> removed;
int id_cnt = 0;

int points[MAX_PLAYER];
int grade[MAX_PLAYER];
string names[MAX_PLAYER];
int numAttendOnWed[MAX_PLAYER];
int numAttendOnWeeken[MAX_PLAYER];

void checkNameAndRegisterId(string name)
{
	if (idMap.count(name) == 0) {
		idMap.insert({ name, ++id_cnt });

		names[id_cnt] = name;
	}
}

void getAttendanceInfo(string name, string weekday) {
	//ID 부여
	checkNameAndRegisterId(name);

	int id = idMap[name];
	if (weekday == "wednesday") {
		points[id] += 3;
		numAttendOnWed[id] += 1;
	}
	else if (weekday == "saturday" || weekday == "sunday") {
		points[id] += 2;
		numAttendOnWeeken[id] += 1;
	}
	else
		points[id] += 1;
}

void getRemovedPlayer(int id)
{
	if (grade[id] != 1 && grade[id] != 2 && numAttendOnWed[id] == 0 && numAttendOnWeeken[id] == 0) {
		removed.push_back(names[id]);
	}
}

void printGrade(int id)
{
	cout << "NAME : " << names[id] << ", ";
	cout << "POINT : " << points[id] << ", ";
	cout << "GRADE : ";

	if (grade[id] == 1) {
		cout << "GOLD" << "\n";
	}
	else if (grade[id] == 2) {
		cout << "SILVER" << "\n";
	}
	else {
		cout << "NORMAL" << "\n";
	}
}

void getPoints(int i)
{
	if (numAttendOnWed[i] > 9) {
		points[i] += 10;
	}

	if (numAttendOnWeeken[i] > 9) {
		points[i] += 10;
	}
}

void getGrade(int i)
{
	if (points[i] >= 50) {
		grade[i] = 1;
	}
	else if (points[i] >= 30) {
		grade[i] = 2;
	}
	else {
		grade[i] = 0;
	}
}

void AttendanceManager() {
	ifstream fin{ "attendance_weekday_500.txt" }; //500개 데이터 입력
	for (int i = 0; i < 500; i++) {
		string name, weekday;
		fin >> name >> weekday;
		getAttendanceInfo(name, weekday);
	}

	for (int id = 1; id <= id_cnt; id++) {
		getPoints(id);
		getGrade(id);
		getRemovedPlayer(id);
		printGrade(id);
	}
	std::cout << "\n";
	std::cout << "Removed player\n";
	std::cout << "==============\n";
	for (auto name : removed)
		std::cout << name << "\n";
}

int main() {
	AttendanceManager();
}