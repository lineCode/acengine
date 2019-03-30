/*--------------------------------------------
**  Module: logger
**  Creator: Tango
**  Time: 2019-02-17
**  Function: log info to file.
**------------------------------------------- */
#ifndef __COMMON_LOGGER_H__
#define __COMMON_LOGGER_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <ostream>
using namespace std;
using std::ofstream;
using std::ostream;

namespace ACE {

class Logger:public ofstream {
public:
    Logger();
    ~Logger();
public:
    void SetLogPath(const std::string &strpath);
    Logger& NewLine(int n_filetype = 0);
    inline bool IsSetPath() const {
		cout<<"_strpath"<<_strpath<<endl;
		return !_strpath.empty();
    }

    friend ostream& EndLine(ostream& logger);

private:
	std::string _strpath;
	std::string _strfile;
};// class Logger

//自定义的操纵符io manip
extern ostream& EndLine(ostream& logger);
extern ostream& EndFile(ostream& logger);

enum {MAX_LOG_BUFF_SIZE = 1024,};

//extern void Log(const char* section, const char* key, const char* msg, va_list& ap);
extern void LogDebug(const char* key, const char* msg, ...);
extern void LogInfo(const char* key, const char* msg, ...);
extern void LogWarning(const char* key, const char* msg, ...);
extern void LogError(const char* key, const char* msg, ...);
extern void LogCritical(const char* key, const char* msg, ...);
extern void LogConsole(const char* key, const char* msg, ...);
extern void LogScript(const char* level, const char* msg, ...);

extern void Error(const char* level, const char* msg, ...);

extern Logger g_logger;
//g_logger对应的线程锁
extern pthread_mutex_t* g_logger_mutex;

}// namespace ACE
#endif 
