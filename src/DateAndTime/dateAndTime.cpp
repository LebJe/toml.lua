#include <DateAndTime/dateAndTime.hpp>

std::ostream & operator<<(std::ostream & os, const TOMLDate & date) {
	os << date.date;
	return os;
}

std::ostream & operator<<(std::ostream & os, const TOMLTime & time) {
	os << time.time;
	return os;
}

std::ostream & operator<<(std::ostream & os, const TOMLTimeOffset & timeOffset) {
	os << timeOffset.timeOffset;
	return os;
}

std::ostream & operator<<(std::ostream & os, const TOMLDateTime & dateTime) {
	os << dateTime.asDateTime();
	return os;
}
