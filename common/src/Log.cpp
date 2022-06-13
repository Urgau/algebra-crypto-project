
#include <iostream>
#include <ostream>
#include <cerrno>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <thread>
#include <iomanip>
#include "Log.hpp"

const std::thread::id MAIN_THREAD_ID = std::this_thread::get_id();

std::ostream &Log::getInfos(std::ostream &ss,
        [[maybe_unused]] const char *file,
        [[maybe_unused]] int line,
        [[maybe_unused]] const char *caller)
{
    std::time_t t = std::time(nullptr);

    ss << ' ' << std::put_time(std::localtime(&t), "%H:%M:%S");
#ifndef NDEBUG
    if (std::this_thread::get_id() == MAIN_THREAD_ID)
        ss << std::setw(13) << std::right << "main_thread";
    else
        ss << std::setw(13) << std::right << std::hex << std::this_thread::get_id() << std::dec;

    ss << "  " <<std::setw(10) << std::left << caller;
    ss << std::setw(20) << std::left << std::string(" (") + file + ":" + std::to_string(line) + ')';
#endif
    ss << " |";
    return (ss);
}
