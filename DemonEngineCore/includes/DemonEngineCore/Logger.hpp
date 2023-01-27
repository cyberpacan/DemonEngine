#pragma once
#include "spdlog/spdlog.h"


namespace DemonEngine {

#ifdef NDEBUG

#define LOG_INFO(...)
#define LOG_WARN(...)
#define LOG_ERROR(...)
#define LOG_CRITICAL(...)

#else

#define INFO(...)       spdlog::info(__VA_ARGS__)
#define WARN(...)       spdlog::warn(__VA_ARGS__)
#define ERROR(...)      spdlog::error(__VA_ARGS__)
#define CRITICAL(...)   spdlog::critical(__VA_ARGS__)

#endif

}
