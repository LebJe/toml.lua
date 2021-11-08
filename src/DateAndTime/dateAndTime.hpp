#ifndef DATE_AND_TIME_H
#define DATE_AND_TIME_H

#include <cstddef>
#include <optional>
#include <toml.hpp>

/// Wrapper for `toml::date`.
struct TOMLDate {
	toml::date date;

	TOMLDate(uint16_t year, uint8_t month, uint8_t day) { date = toml::date { year, month, day }; }
	TOMLDate(toml::date d) { date = d; }

	uint16_t getYear() const { return date.year; };
	uint8_t getMonth() const { return date.month; };
	const uint8_t getDay() const { return date.day; };

	void setYear(uint16_t year) { date.year = year; };
	void setMonth(uint8_t month) { date.month = month; };
	void setDay(uint8_t day) { date.day = day; };

	bool operator==(const TOMLDate & right) const { return date == right.date; }

	bool operator<(const TOMLDate & right) const { return date < right.date; }

	bool operator<=(const TOMLDate & right) const { return date <= right.date; }
};

/// Wrapper for `toml::time`.
struct TOMLTime {
	toml::time time;

	TOMLTime(uint8_t hour, uint8_t minute, uint8_t second, uint32_t nanoSecond) {
		time = toml::time { hour, minute, second, nanoSecond };
	}

	TOMLTime(toml::time t) { time = t; }

	uint8_t getHour() const { return time.hour; };
	uint8_t getMinute() const { return time.minute; };
	const uint8_t getSecond() const { return time.second; };
	const uint32_t getNanoSecond() const { return time.nanosecond; };

	void setHour(uint8_t hour) { time.hour = hour; };
	void setMinute(uint8_t minute) { time.minute = minute; };
	void setSecond(uint8_t second) { time.second = second; };
	void setNanoSecond(uint32_t nanoSecond) { time.nanosecond = nanoSecond; };

	bool operator==(const TOMLTime & right) const { return time == right.time; }

	bool operator<(const TOMLTime & right) const { return time < right.time; }

	bool operator<=(const TOMLTime & right) const { return time <= right.time; }
};

/// Wrapper for `toml::time_offset`.
struct TOMLTimeOffset {
	toml::time_offset timeOffset;

	TOMLTimeOffset(toml::time_offset tO) { timeOffset = tO; }

	TOMLTimeOffset(int8_t hours, int8_t minutes) {
		timeOffset = toml::time_offset(hours, minutes);
	};

	int16_t minutes() const { return timeOffset.minutes; };

	bool operator==(const TOMLTimeOffset & right) const { return timeOffset == right.timeOffset; }

	bool operator<(const TOMLTimeOffset & right) const { return timeOffset < right.timeOffset; }

	bool operator<=(const TOMLTimeOffset & right) const { return timeOffset <= right.timeOffset; }
};

/// Wrapper for `toml::date_time`.
struct TOMLDateTime {

	TOMLDate date;
	TOMLTime time;
	std::optional<TOMLTimeOffset> timeOffset = std::optional<TOMLTimeOffset>();

	TOMLDateTime(TOMLDate d, TOMLTime t) : date(d), time(t) {};

	TOMLDateTime(TOMLDate d, TOMLTime t, TOMLTimeOffset tO) : date(d), time(t), timeOffset(tO) {};

	TOMLDate getDate() { return date; };
	TOMLTime getTime() { return time; };
	TOMLTimeOffset getTimeOffset() { return timeOffset.value(); };

	void setDate(TOMLDate d) { date = d; };
	void setTime(TOMLTime t) { time = t; };
	void setTimeOffset(TOMLTimeOffset tO) { timeOffset = tO; };

	toml::date_time asDateTime() const {
		auto dt = toml::date_time(date.date, time.time);

		if (timeOffset) { dt.offset = timeOffset.value().timeOffset; }

		return dt;
	};

	bool operator==(const TOMLDateTime & right) const { return asDateTime() == right.asDateTime(); }

	bool operator<(const TOMLDateTime & right) const { return asDateTime() < right.asDateTime(); }

	bool operator<=(const TOMLDateTime & right) const { return asDateTime() <= right.asDateTime(); }
};

std::ostream & operator<<(std::ostream & os, const TOMLDate & date);
std::ostream & operator<<(std::ostream & os, const TOMLTime & time);
std::ostream & operator<<(std::ostream & os, const TOMLTimeOffset & timeOffset);
std::ostream & operator<<(std::ostream & os, const TOMLDateTime & dateTime);

#endif /* DATE_AND_TIME_H */
