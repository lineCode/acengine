/*--------
// Module: cfg_reader
// Creator: Tango
// Time: 2019-02-17
// Function: read the config file.
//------- */

#include "cfg_reader.h"
#include "util_string.h"
#include <iostream>
#include <fstream>
using std::string;
using std::ifstream;
using std::ios;
using std::cout;
using std::endl;

namespace ACE {

CCfgReader::CCfgReader(const string &strfile):_strfile(strfile), _bscan(false) {
}

CCfgReader::~CCfgReader() {
	map<string, map<string, string> *>::iterator iter = _cfgdict.begin();
	for (; iter != _cfgdict.end(); ++iter)
	{
		delete iter->second;
		iter->second = NULL;
	}
}

string CCfgReader::GetValue(const char * section, const char *szname) {
	if (!_bscan) {
		ScanCfgFile();
	}
	
	map<string, map<string, string>*>::const_iterator iter = _cfgdict.find(section);
	if (iter != _cfgdict.end()) {
		map<string, string> &tmp = *(iter->second);
		map<string, string>::const_iterator iter2 = tmp.find(szname);
		if (iter2 != tmp.end())
		{
			return iter2->second;
		}
	}
	
	return "";
}

string CCfgReader::GetOptValue(const char *section, const char *szname, const string &strdefault) {
	string ret = GetValue(section, szname);
	return (ret=="")? strdefault:ret;
}

int CCfgReader::ScanCfgFile() {
	std::ifstream file(_strfile.c_str(), ios::in);
	if (!file.is_open())	return -1;

	string strsection;
	char szline[128];
	// 0x0A 换行符
	while (!file.eof() && file.getline(szline, sizeof(szline), 0x0A)) {
		string tt = string(szline);
		string &sline = Trim(tt);
		if (sline.empty() || sline[0] == '#') {
			continue;
		}

		if ('[' == sline[0] && ']' == sline[sline.size() - 1]) {
			strsection = sline.substr(1, sline.size()-2);
			continue;
		}

		string::size_type pos = sline.find("=");
		if (string::npos == pos) {
			continue;
		}

		//需要一个所属的 section
		if (strsection.empty()) {
			return -1;
		}

		string strtemp = sline.substr(0, pos);
		string s1 = Trim(strtemp);
		strtemp = sline.substr(pos + 1, sline.size() - pos - 1);
		string s2 = Trim(strtemp);

		auto iter = _cfgdict.lower_bound(strsection);
		if (iter != _cfgdict.end() && strsection == iter->first) {
			iter->second->insert(make_pair(s1, s2));
		} 
		else {
			map<string, string> *tmp = new map<string, string>;
			tmp->insert(make_pair(s1, s2));
			_cfgdict.insert(iter, make_pair(strsection, tmp));
		}

	}// end while

	_bscan = true;
	return 0;
}

void CCfgReader::show() {
	if (!_bscan) ScanCfgFile();

	auto it = _cfgdict.begin();
	while (it != _cfgdict.end()) {
		cout<<it->first<<endl;
		auto it_in = it->second->begin();
		while (it_in != ((it->second))->end()) {
			cout<<(it_in)->first<<"="<<(it_in)->second<<endl;
			++it_in;
		}
		++it;
	}
}

}// namespace ACE

