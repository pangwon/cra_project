#include <string>

using namespace std;

class PointPolicy {
public:
	virtual int getPoint(string weekday) = 0;
	virtual int getAdditionalPoint(int wed, int weeken) = 0;
};

class LegacyPointPolicy : public PointPolicy {
public:
	int getPoint(string weekday) override {
		int point = 1;
		if (weekday == "wednesday") {
			point = 3;
		}
		else if (weekday == "saturday" || weekday == "sunday") {
			point = 2;
		}

		return point;
	}
	int getAdditionalPoint(int wed, int weeken) override {
		int points = 0;
		if (wed > 9) {
			points += 10;
		}
		if (weeken > 9) {
			points += 10;
		}
		return points;
	}
};