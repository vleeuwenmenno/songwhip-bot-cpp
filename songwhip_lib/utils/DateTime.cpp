#include <cstdlib>
#include <ctime>
#include <string>

#ifdef _WIN32
#define timegm _mkgmtime
#endif

inline int ParseInt(const char* value)
{
    return std::strtol(value, nullptr, 10);
}

// ParseISO8601 returns milliseconds since 1970

std::time_t ParseISO8601(const std::string& input)
{
    constexpr const size_t expectedLength = sizeof("1234-12-12T12:12:12Z") - 1;
    static_assert(expectedLength == 20, "Unexpected ISO 8601 date/time length");

    if (input.length() < expectedLength)
    {
        return 0;
    }

    std::tm time = { 0 };
    time.tm_year = ParseInt(&input[0]) - 1900;
    time.tm_mon = ParseInt(&input[5]) - 1;
    time.tm_mday = ParseInt(&input[8]);
    time.tm_hour = ParseInt(&input[11]);
    time.tm_min = ParseInt(&input[14]);
    time.tm_sec = ParseInt(&input[17]);
    time.tm_isdst = 0;
    const int millis = input.length() > 20 ? ParseInt(&input[20]) : 0;
    return timegm(&time) * 1000 + millis;
}