#include <string>

using namespace std;

class GradePolicy {
public:
	virtual string getGrade(int points) = 0;
};

class LegacyGradePolicy : public GradePolicy {
public:
	string getGrade(int points) override {
		string grade = "NORMAL";
		if (points >= 50) {
			grade = "GOLD";
		}
		else if (points >= 30) {
			grade = "SILVER";
		}

		return grade;
	}
};