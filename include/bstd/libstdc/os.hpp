#pragma once

#if defined(LINUX) || defined(__unix__) || defined(__unix) || defined(POSIX) || defined(__LINUX__) || defined(__LINUX) || defined(__POSIX) || defined(__linux__) || defined(unix)
#define linx 1
#elif defined(_WIN64) || defined(__CYGWIN__)
#else
#define linx 1
#endif
