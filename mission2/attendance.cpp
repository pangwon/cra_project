#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "grade.cpp"
#include "point.cpp"
#include "gmock/gmock.h"

using namespace std;
using namespace testing;

const int MAX_PLAYER = 20;

class AttendanceManager {
public:
	void manage() {
		for (int id = 1; id <= idCnt; id++) {
			calculateAdditionalPointAndGrade(id);
			isRemovedPlayer(id);
			printPlayerAttendance(id);
		}
		printRemovedPlayer();
	}

	void getAttendanceInfo(string name, string weekday) {
		int id = checkNameAndRegisterId(name);

		if (weekday == "wednesday") {
			numAttendOnWed[id] += 1;
		}
		else if (weekday == "saturday" || weekday == "sunday") {
			numAttendOnWeeken[id] += 1;
		}

		points[id] += pointPolicy->getPoint(weekday);
	}

	void selectPointPolicy(PointPolicy* p) {
		pointPolicy = p;
	}

	void selectGradePolicy(GradePolicy* p) {
		gradePolicy = p;
	}

	string getPlayerGrade(int id) {
		return grade[id];
	}

	int getPlayerPoints(int id) {
		return points[id];
	}

	int getPlayerId(string name) {
		if (idMap.count(name) == 0) {
			return -1;
		}
		return idMap[name];
	}

private:
	void calculateAdditionalPointAndGrade(int id)
	{
		points[id] += pointPolicy->getAdditionalPoint(numAttendOnWed[id], numAttendOnWeeken[id]);
		grade[id] = gradePolicy->getGrade(points[id]);
	}

	int checkNameAndRegisterId(string name)
	{
		if (idMap.count(name) == 0) {
			idMap.insert({ name, ++idCnt });

			names[idCnt] = name;
		}
		return idMap[name];
	}

	void isRemovedPlayer(int id)
	{
		if (grade[id] != "SILVER" && grade[id] != "GOLD" && numAttendOnWed[id] == 0 && numAttendOnWeeken[id] == 0) {
			removed.push_back(names[id]);
		}
	}

	void printPlayerAttendance(int id)
	{
		cout << "NAME : " << names[id] << ", ";
		cout << "POINT : " << points[id] << ", ";
		cout << "GRADE : " << grade[id] << "\n";
	}

	void printRemovedPlayer()
	{
		std::cout << "\n";
		std::cout << "Removed player\n";
		std::cout << "==============\n";
		for (auto name : removed)
			std::cout << name << "\n";
	}

	map<string, int> idMap;
	vector<string> removed;
	int idCnt = 0;
	int points[MAX_PLAYER];
	string grade[MAX_PLAYER];
	string names[MAX_PLAYER];
	int numAttendOnWed[MAX_PLAYER];
	int numAttendOnWeeken[MAX_PLAYER];

	PointPolicy* pointPolicy;
	GradePolicy* gradePolicy;
};

class AttendanceManagerTest : public Test {
protected:
	AttendanceManager manager;

	void SetUp() override {
		manager.selectGradePolicy(new LegacyGradePolicy());
		manager.selectPointPolicy(new LegacyPointPolicy());
	}
};

TEST_F(AttendanceManagerTest, ShouldBeNormalGrade) {
	string name = "Alice";
	string weekday = "tuesday";

	manager.getAttendanceInfo(name, weekday);
	manager.manage();

	int id = manager.getPlayerId(name);
	EXPECT_TRUE(manager.getPlayerPoints(id) < 30);
	EXPECT_EQ(manager.getPlayerGrade(1), "NORMAL");
}

TEST_F(AttendanceManagerTest, ShouldBeSilverGrade) {
	string name = "Alice";
	string weekday = "tuesday";
	for (int i = 0; i < 35; i++)
		manager.getAttendanceInfo(name, weekday);
	manager.manage();

	int id = manager.getPlayerId(name);

	EXPECT_TRUE(manager.getPlayerPoints(id) >= 30);
	EXPECT_EQ(manager.getPlayerGrade(id), "SILVER");
}

TEST_F(AttendanceManagerTest, ShouldBeGoldGrade) {
	string name = "Alice";
	string weekday = "tuesday";
	for (int i = 0; i < 55; i++)
		manager.getAttendanceInfo(name, weekday);
	manager.manage();

	int id = manager.getPlayerId(name);

	EXPECT_TRUE(manager.getPlayerPoints(id) >= 50);
	EXPECT_EQ(manager.getPlayerGrade(id), "GOLD");
}

TEST_F(AttendanceManagerTest, GetAdditionalPointForWednesday) {
	string name = "Alice";
	string weekday = "wednesday";
	for (int i = 0; i < 10; i++)
		manager.getAttendanceInfo(name, weekday);
	manager.manage();

	int id = manager.getPlayerId(name);

	// 30 + 10;
	EXPECT_EQ(manager.getPlayerPoints(id), 40);
}

TEST_F(AttendanceManagerTest, GetAdditionalPointForWeekend) {
	string name = "Alice";
	string weekday = "saturday";
	for (int i = 0; i < 10; i++)
		manager.getAttendanceInfo(name, weekday);
	manager.manage();

	int id = manager.getPlayerId(name);

	// 20 + 10;
	EXPECT_EQ(manager.getPlayerPoints(id), 30);
}

TEST_F(AttendanceManagerTest, WrongNameRequested) {
	string name = "Alice";
	string weekday = "saturday";
	for (int i = 0; i < 10; i++)
		manager.getAttendanceInfo(name, weekday);
	manager.manage();

	string wrongName = "Steve";
	int id = manager.getPlayerId(wrongName);

	EXPECT_EQ(id, -1);
}

int main() {
#ifdef NDEBUG
	ifstream fin{ "attendance_weekday_500.txt" };
	AttendanceManager manager;
	manager.selectGradePolicy(new LegacyGradePolicy());
	manager.selectPointPolicy(new LegacyPointPolicy());

	string name, weekday;
	for (int i = 0; i < 500; i++) {
		fin >> name >> weekday;
		manager.getAttendanceInfo(name, weekday);
	}
	manager.manage();
#else
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
#endif
}
