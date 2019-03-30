#ifndef __COMMON_CFG_READER__
#define __COMMON_CFG_READER__
#include <string>
#include <map>
using std::string;
using std::map;
namespace ACE
{
class CCfgReader
{
public:
	CCfgReader(const string &strfile);
	~CCfgReader();

	string GetValue(const char * section, const char *szname);
	string GetOptValue(const char *section, const char *szname, const string &strdefault);

	void show();
private:
	// 返回：0 成功
	int ScanCfgFile();
private:
	string _strfile;
	map<string, map<string, string>*> _cfgdict;
	bool _bscan;

};// class CCfgReader
}// namespace ACE
#endif
