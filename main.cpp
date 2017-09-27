#include <iostream>
#include <string>

using namespace std;

enum Month {January=1,February,March,April,May,June,July,August,September,October,November,December} ;

/*  The function easter_base uses the Meeus/Jones/Butcher formula to compute
    the number that is used to determine on which month (easter_month below)
    and day (easter_day below) Easter is in the given year.
*/
int easter_base ( int year )
{
    const int A     = year % 19 ;
    const int B     = year / 100 ;
    const int C     = year % 100 ;
    const int D     = B / 4 ;
    const int E     = B % 4 ;
    const int F     = (B + 8) / 25 ;
    const int G     = (B - F + 1) / 3 ;
    const int H     = (19 * A + B - D - G + 15) % 30 ;
    const int I     = C / 4 ;
    const int K     = C % 4 ;
    const int L     = (32 + 2 * E + 2 * I - H - K) % 7 ;
    const int M     = (A + 11 * H + 22 * L) / 451 ;
    return H + L - 7 * M + 114 ;
}

/*  The result of easter_year is the day number of the month in which Easter occurs in
    the given year.
*/
int easter_day ( int year )
{
    return (easter_base (year) % 31) + 1 ;
}

/*  The result of easter_month is the month in which Easter occurs in the given year.
*/
Month easter_month ( int year )
{
    return static_cast<Month> (easter_base (year) / 31) ;
}


/********************************************************************
    Assignment part 1: Leap years
********************************************************************/
// Returns whether or not a given year is a leap year
bool is_leap_year ( int year )
{
    // Year must be divisible by 4
    if (year % 4 == 0) {

        // if year is both a multiple of 100 and 400
        // then it is not a leap year
        if (year % 100 == 0 && year % 400 == 0) {
            return false;
        } 

        return true;
    }

    return false;
}

// Returns the number of days a given month has in a given year
int number_of_days_in_month ( int year, Month month )
{
    switch (month) {
        case January:
        case March:
        case May:
        case July:
        case August:
        case October:
        case December:
            return 31;
            break; 
        case February:
            return is_leap_year(year) ? 29 : 28;
            break;
        default:
            return 30;
    }
}

/********************************************************************
    Assignment part 2: Holy days based on Easter
********************************************************************/
int numeric_easter_month(int year) 
{
    return easter_base (year) / 31;
}

int day_number(int day, int month, int year)
{
    bool month_is_january = month == 1;
    if (month_is_january) {
        return day;
    }

    // initial value is the number of days in current date
    int daysTotal = day;
    for(int i = 1; i <= month - 1; i++) {
        Month currentMonth = static_cast<Month>(i);
        int daysInMonth = number_of_days_in_month(year, currentMonth);
        daysTotal = daysTotal + daysInMonth;
    }
    return daysTotal;
}

int day_in_month_by_day_number(int days, int year)
{
    if (days < 0 || days > 365) {
        return -1;
    }

    if (days < 31) {
        // Then it is in january
        return days;
    }

    for(int i = 0; i <= 11; i++) {
        Month month = static_cast<Month>(i + 1);
        int daysInCurrentMonth = number_of_days_in_month(year, month);
        if (days > daysInCurrentMonth) {
            days = days - daysInCurrentMonth;
        }
    }

    return days;
}

// Given the number of days in a year, return in which month the last day is
int month_by_day_number(int days, int year)
{
    if (days < 31) {
        // Then it is january
        return 1;
    }

    if (days > 365) {
        // an invalid number of days, 
        // return -1 to indicate the result is an error
        return -1;
    }

    int total_days = 0; 
    for(int i = 0; i <= 11; i++) {
        Month month = static_cast<Month>(i + 1);
        total_days = total_days + number_of_days_in_month(year, month);
        if (days < total_days) {
            return i + 1;
        }
    }
}

int easter_day_number_of_year(int year)
{
    int easterDay = easter_day(year);
    int easterMonth = numeric_easter_month(year);
    return day_number(easterDay, easterMonth, year);
}

void show_holy_days (int year)
{
    // Easter day
    int easterDayOfYear = easter_day_number_of_year(year);
    int easterDayOfEasterMonth = easter_day(year);
     // Easter month
    int easterMonth = month_by_day_number(easterDayOfYear, year);

    cout << "Easter: " << easterDayOfEasterMonth << "/" << easterMonth << "/" << year;

}

bool test_easter_day_number()
{
    bool result = true;
    // testing the result the date of easter for years
    int years[]       = { 2017, 2016, 2015, 2014 };
    int easter_days[] = { 106 , 87, 95, 110 };

    for(int i = 0; i < 4; i++) {
        int year = years[i];
        int easterDay = easter_days[i];
        if (easter_day_number_of_year(year) != easterDay) {
            result = false;
            cout << "easter_day_number_of_year is implemented incorrectly" << endl;
        }
    }
    return result;
}


bool test_month_by_day_number() 
{
    bool result = true;
    // 24/01/2017 is in January
    if (month_by_day_number(24, 2017) != 1) {
        cout << "Expected month_by_day_number(24, 2017) to equal 1" << endl;
        result = false;
    } 
    // 16/02/2016 is in February
    if (month_by_day_number(31 + 16, 2016) != 2) {
        cout << "Expected month_by_day_number(31 + 16, 2016) to equal 2" << endl;
        result = false;
    }

    // 29/02/2017 is actually in March (2017 not a leap year)
    // because it is actually 01/03/2017 (29/02/2017 does not exist)
    if (month_by_day_number(31 + 29, 2017) != 3) {
        cout << "Epected month_by_day_number(31 + 29, 2017) to equal 3" << endl;
        result = false;
    }

    return result;
}

int main()
{
    // Run tests for function easter_day_number() and terminate if they fail
    bool easter_day_works = test_easter_day_number();
    if (!easter_day_works) {
        cout << "Program implemented incorrectly" << endl;
        return 0;
    }

    // Run tests for month_by_day_number() and terminate if they fail
    bool month_by_day_number_works = test_month_by_day_number();
    if (!month_by_day_number_works) {
        cout << "Program implemented incorrectly" << endl;
        return 0;
    }

    cout << "This program shows the dates of the holidays for a given year." << endl;
    string yearInput = "";

    cout << "Enter the year: ";
    getline(cin, yearInput);

    // check whether the input string is empty
    // then the user did not provide any input
    if (yearInput.empty()) {
        cout << "You did not enter a year, closing...";
        return 0;
    }

    // parse the input string into an integer
    int year = stoi(yearInput);
    cout << "Dates of Christian holiday:" << endl;
    show_holy_days(year);
    return 0;
}