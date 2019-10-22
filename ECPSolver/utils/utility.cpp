/******************************
*@email:zjluestc@outlook.com
*@created date:2019/5/16
******************************/
#include "utility.hpp"

#include <iomanip>
#include <random>
#include <cmath>

using namespace std;

namespace zjl_utility {

const string Date::shortDateStr() {
    ostringstream os;
    time_t now = std::time(0);
    tm s_time;
    localtime_s(&s_time, &now);
    os << std::put_time(&s_time, "%Y%m%d%H%M%S");
    return os.str();
}

const string Date::humanDataStr() {
    ostringstream os;
    time_t now = std::time(0);
    tm s_time;
    localtime_s(&s_time, &now);
    os << std::put_time(&s_time, "%Y-%m-%d/%H:%M:%S");
    return os.str();
}

Random::Random(unsigned int seed) {
    if (seed != 0 && !set_seed_) {
        set_seed_ = true;
        srand(seed);
        seed_ = seed;
    }
}

void Random::setSeed(unsigned int seed) {
    if (!set_seed_) {
        set_seed_ = true;
        srand(seed);
        seed_ = seed;
    }
}

int Random::genInt(int ub, int lb) const {
    // [zjl][TODO] : add intput parameter check.
    // [zjl][BUG] : when ub - lb > INT_MAX, result will be wrong.
    return rand() % (ub - lb) + lb;
}

double Random::genDouble(double ub, double lb) const {
    // [zjl][TODO] : add intput parameter check.
    return (double)rand() / (double)RAND_MAX*(ub - lb) + lb;
}

Log::Log(std::string dir_path) : nb_msg(0) {
    #ifndef CLOSE_ALL_LOGS
    if (dir_path.size()) {
        ofs.open(dir_path + Date::shortDateStr() + ".log");
        if (ofs.is_open())
            ofs << "Total log number:          " << endl;
    }
    #endif // !CLOSE_ALL_LOGS
}

Log::~Log() {
    #ifndef CLOSE_ALL_LOGS
    if (ofs.is_open()) {
        ofs.seekp(17);
        ofs << nb_msg;
        ofs.close();
    }
    #endif // !CLOSE_ALL_LOGS
}

// init static variable and golball variable.
bool Random::set_seed_ = false;
unsigned int Random::seed_ = 0;
Random myrand;

Log mylog("Logs/");    // output logs into file.
//Log mylog;           // don't output logs into file.

LogSwitch logsw_debug(0, 1, "DEBUG");
LogSwitch logsw_info(1, 1, "INFO");
LogSwitch logsw_warn(1, 1, "WARN");
LogSwitch logsw_error(1, 1, "REEOR");
LogSwitch logsw_critical(1, 1, "CRITICAL");

}
