#include "logger.h"
#include <sys/time.h>
#include "mutex.h"
using std::string;
using std::ios;

namespace ACE {

pthread_mutex_t* g_logger_mutex = NULL;
Logger g_logger;  //全局变量

Logger::Logger() {};
Logger::~Logger() {};

void Logger::SetLogPath(const std::string& strpath) {
	_strpath.assign(strpath);
}

Logger& Logger::NewLine(int n_filetype/* = 0*/) {
	time_t tnow = time(NULL);
	tm * pTm = localtime(&tnow);
	char szTemp[64];
	memset(szTemp, 0, sizeof(szTemp));

	string format;
	if (n_filetype==1) {
		format = "%Y%m%d.err";           //错误日志
	}
	else {
		format = "%Y%m%d.log";           //一般日志
	}

	strftime(szTemp, sizeof(szTemp), format.c_str(), pTm);

	bool opened = true;
	if(!this->is_open()) {
		opened = false;
	}
	else if(_strfile != szTemp) {
		this->close();
		opened = false;
	}
	else {
		//opened = true;
	}

	if (!opened) {
		_strfile.assign(szTemp);

		if (_strpath.empty()) {
			this->open(szTemp, ios::out|ios::app);
		}
		else {
			char szLogFile[256];
			memset(szLogFile, 0, sizeof(szLogFile));
			snprintf(szLogFile, sizeof(szLogFile), "%s%s", _strpath.c_str(), szTemp);
			this->open(szLogFile, ios::out|ios::app);
		}
	}

	return *this;
}

ostream& EndLine(ostream& logger) {
	logger<<std::endl;
	logger.flush();
	return logger;
}

ostream& EndFile(ostream& logger) {
	logger << std::endl;
	((ofstream&)logger).close();

	return logger;
}

//获得当前时间的格式化的时分秒毫秒 HH:MM:SS.UUUUUU
void _get_time_hmsu_head(char* s, size_t n) {
	struct timeval tv;
	if (gettimeofday(&tv, NULL)==0) {
		time_t& t = tv.tv_sec;
		struct tm* tm2 = localtime(&t);

		snprintf(s, n, "%02d:%02d:%02d.%06ld", tm2->tm_hour, tm2->tm_min, tm2->tm_sec, tv.tv_usec);
	}
	else {
		snprintf(s, n, "??:??:??.??????");
	}
}

template <size_t size>
void _Log(char (&strDest)[size], const char* section, const char* key, const char* msg, va_list& ap) {
	static const char _hmsu_head[] = "17:04:10.762177";
	enum {_hmsu_head_size = sizeof(_hmsu_head)+1,};

	char szHmsu[32];
	memset(szHmsu, 0, 32);

	_get_time_hmsu_head(szHmsu, sizeof(szHmsu));

	int n1 = snprintf(strDest, (int)size, "%s  [%s][%s]", szHmsu, section, key);
	if(n1 > 0 && n1 < (int)size)
	{
		int n2 = vsnprintf(strDest+n1, (int)size-n1, msg, ap);
		if(n2 > 0 && (n1+n2)<(int)size)
		{
			strDest[n1+n2] = '\0';
		}

	}
}

// section: 日志等级
void Log2File(const char* section, const char* key, const char* msg, va_list& ap, int nFileType = 0) {
	char szTmp[MAX_LOG_BUFF_SIZE];
	memset(szTmp, 0, MAX_LOG_BUFF_SIZE * sizeof(char));

	_Log(szTmp, section, key, msg, ap);

	if(g_logger.IsSetPath())
	{
		if(g_logger_mutex == NULL)
		{
			g_logger.NewLine(nFileType) << szTmp << EndLine;
		}
		else
		{
			//dbmgr的多线程日志要加锁
			Mutex gm(*g_logger_mutex);
			g_logger.NewLine(nFileType) << szTmp << EndLine;
		}
	}
	else
	{
		printf("print:%s\n", szTmp);
	}
}

void Log2Console(const char* section, const char* key, const char* msg, va_list& ap, int nFileType = 0) {
	char szTmp[MAX_LOG_BUFF_SIZE];
	_Log(szTmp, section, key, msg, ap);

#ifdef _DEBUG_VERSION_
	printf("%s\n", szTmp);
#endif
	if(g_logger.IsSetPath())
	{
		g_logger.NewLine(nFileType) << szTmp << EndLine;
		printf("%s\n", szTmp);
	}
	else
	{
		printf("%s\n", szTmp);
	}

}

void LogDebug(const char* key, const char* msg, ...) {
	va_list ap;
	memset(&ap, 0, sizeof ap);

	va_start(ap, msg);
	Log2File("DEBUG   ", key, msg, ap);
	va_end(ap);
}

void LogInfo(const char* key, const char* msg, ...) {
	va_list ap;
	memset(&ap, 0, sizeof ap);

	va_start(ap, msg);
	Log2File("INFO    ", key, msg, ap);
	va_end(ap);
}

void LogWarning(const char* key, const char* msg, ...) {
	va_list ap;
	memset(&ap, 0, sizeof ap);

	va_start(ap, msg);
	Log2File("WARNING ", key, msg, ap);
	va_end(ap);
}

void LogError(const char* key, const char* msg, ...) {
	va_list ap;
	memset(&ap, 0, sizeof ap);

	va_start(ap, msg);
	Log2File("ERROR   ", key, msg, ap);
	va_end(ap);
}

void LogCritical(const char* key, const char* msg, ...) {
	va_list ap;
	memset(&ap, 0, sizeof ap);

	va_start(ap, msg);
	Log2File("CRITICAL", key, msg, ap);
	va_end(ap);
}

void LogScript(const char* level, const char* msg, ...) {
	va_list ap;
	memset(&ap, 0, sizeof ap);

	va_start(ap, msg);
	Log2File("SCRIPT  ", level, msg, ap);
	va_end(ap);
}

void LogConsole(const char* key, const char* msg, ...) {
	va_list ap;
	memset(&ap, 0, sizeof ap);

	va_start(ap, msg);
	Log2Console("CONSOLE ", key, msg, ap);
	va_end(ap);
}

void Error(const char* level, const char* msg, ...) {
	va_list ap;
	memset(&ap, 0, sizeof ap);

	va_start(ap, msg);
	Log2File("ERROR  ", level, msg, ap, 1);
	va_end(ap);
}

}// namespace ACE
