/******************************
*@email:zjluestc@outlook.com
*@created date:2019/5/16
******************************/
#pragma once
#ifndef ZJL_UTILITY_H
#define ZJL_UTILITY_H
#include <ctime>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>

//#define CLOSE_ALL_LOGS

namespace zjl_utility {

class Timer {
public:
    Timer(long interval) :start_time(clock()), end_time(start_time + interval) {};

    bool isTimeOut() const { return clock() > end_time; }

    long usedTime() const { return clock() - start_time; }

    long restTime() const { return end_time - clock(); }
private:
    std::clock_t start_time;
    std::clock_t end_time;
};

class Date {
public:
    static const std::string shortDateStr();

    static const std::string humanDataStr();
};

class Random {
public:
    Random(unsigned int seed = 0);

    void setSeed(unsigned int seed);

    unsigned int getSeed() const { return seed_; }

    int gen(int ub = INT_MAX, int lb = 0) const;

    double gen(double ub = 1.0, double lb = 0.0) const;
private:
    static bool set_seed_;
    static unsigned int seed_;
};

struct LogSwitch {
    bool msg_on;        // print this log or not.
    bool time_on;       // print timestamp or not.
    std::string name;   // logSwitch name.
    // eg: 0, 1, "DEBUG"
    LogSwitch(bool msg_on_ = false, bool time_on_ = false, std::string name_ = "") :
        msg_on(msg_on_), time_on(time_on_), name(name_) {}
};

class Log {
public:
    Log(std::string dir_path = "");
    ~Log();

    template<typename T>
    Log& operator<<(const T &msg) {
        #ifndef CLOSE_ALL_LOGS
        oss << msg;
        #endif
        return *this;
    }

    template<>
    Log& operator<<<LogSwitch>(const LogSwitch &info) {
        #ifndef CLOSE_ALL_LOGS
        if (info.msg_on) {
            if (info.time_on) {
                clock_t now = clock();
                if (now > 99999) {
                    std::cout << "[" << now / 1000 << "s]";
                    if (ofs.is_open()) ofs << "[" << now / 1000 << "s]";
                } else {
                    std::cout << "[" << now << "ms]";
                    if (ofs.is_open()) ofs << "[" << now << "ms]";
                }
            }
            if (info.name.size()) {
                std::cout << "[" << info.name << "]";
                if (ofs.is_open())ofs << "[" << info.name << "]";
            }
            std::cout << oss.str() << std::endl;
            if (ofs.is_open())ofs << oss.str() << std::endl;
            nb_msg++;
        }
        oss.str("");
        #endif
        return *this;
    }
private:
    size_t nb_msg;  // log number.
    std::ofstream ofs;
    std::ostringstream oss; // log buffer.
};

template<typename IdType>
class IdMapInt {   // the map between identity and zero based integer.
public:
    IdType get_id(int num) const { return int2id[num]; }
    int get_num(IdType id) const { return id2int[id]; }
    
    int add_id(IdType id) {
        if (id2int.find(id) != id2int.end()) {
            return id2int[id];
        } else {
            int2id.push_back(id);
            id2int[id] = static_cast<int>(int2id.size()) - 1;
            return static_cast<int>(int2id.size()) - 1;
        }
    }
private:
    std::vector<IdType> int2id;
    std::unordered_map<IdType, int> id2int;
};

#pragma region Non-member function
inline std::string itos(int i) {
    std::stringstream s; s << i;
    return s.str();
}

inline bool equal(double lhs, double rhs, double eps = 0.00001) {
    return abs(lhs - rhs) < eps;
}
#pragma endregion


#pragma region External_variable
extern Log mylog;   // global log object for the project.

extern LogSwitch logsw_debug;
extern LogSwitch logsw_info;
extern LogSwitch logsw_warn;
extern LogSwitch logsw_error;
extern LogSwitch logsw_critical;

extern Random myrand;
#pragma endregion

}

#endif // !ZJL_UTILITY_H
