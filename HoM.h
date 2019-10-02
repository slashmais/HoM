#ifndef _HoM_HoM_h_
#define _HoM_HoM_h_

#include <string>
#include <map>


//---------------------------------------------------------------------------------------------------
extern std::string HoM_PATH;
const std::string HoM_Message();
bool HoMicile();

//---------------------------------------------------------------------------------------------------
struct AppConfig
{
private:
	AppConfig(const AppConfig &ac);
	AppConfig& operator=(const AppConfig&);
	
public:
	std::string configfile{};
	std::string configfilepath{};
	std::map<std::string, std::string> data{};
	
	~AppConfig();
	AppConfig();
	AppConfig(const std::string &qdf);
	//AppConfig(const AppConfig &ac) { *this=ac; }
	//AppConfig& operator=(const AppConfig &ac);
	
	void clear() { configfile.clear(); configfilepath.clear(); data.clear(); }
	void setval(const std::string &k, const std::string &v, bool bcase=false); //create/change
	const std::string getval(const std::string &k, bool bcase=false);
	
	void delkey(const std::string &k, bool bcase=false); //removes key (and value)
	
	bool hasval(const std::string &v); //search thru all vals (all keys)
	bool haskey(const std::string &k, bool bcase=false); //if key exists
	bool haskeyval(const std::string &k, const std::string &v, bool bcase=false); //if [k,v]-pair exists

	bool Save();
	bool Load(const std::string &qdf="");
};

//---------------------------------------------------------------------------------------------------
bool MakeAppHoM(const std::string &AppFullPath);
bool GetAppHoMConfig(const std::string &AppFullPath, AppConfig &AC);




#endif
