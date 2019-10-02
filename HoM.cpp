
#include "HoM.h"
#include <utilfuncs/utilfuncs.h>
#include <unistd.h>
#include <pwd.h>


//===================================================================================================
std::string HoM_PATH{};
std::string HoMsg{"uninitialized"}; // :)

const std::string HoMReadMeFile{"README"};

const std::string HoM_Message() { return HoMsg; }

bool create_readme(const std::string &rmf)
{
	if (!file_exist(rmf))
	{
		std::string readmetxt = "\n*****\n- DO NOT RENAME, MOVE OR DELETE THIS DIRECTORY -\n*****"
								"\n\nThis directory contain data for HoM applications."
								"\n\nThis entire directory should be backed-up along with your"
								"\nother persistent data."
								"\n\nYou can change content in the files found here if you know"
								"\nwhat you are doing, if not, then don't.";
		file_write(rmf, readmetxt);
	}
	return file_exist(rmf);
}

bool HoMicile()
{
	struct passwd *pw=getpwuid(getuid());
	HoM_PATH=path_append(pw->pw_dir, "HoM");
	if (!path_realize(HoM_PATH)) { HoMsg=spf("Cannot create/access '", HoM_PATH, "'"); return false; }
	std::string rmf=path_append(HoM_PATH, HoMReadMeFile);
	if (!create_readme(rmf)) { HoMsg=spf("Cannot create/access '", rmf, "'"); return false; }
	HoMsg="HoM is good";
	return true;
}

//===================================================================================================
//configs

AppConfig::~AppConfig() {}

AppConfig::AppConfig() { }

AppConfig::AppConfig(const std::string &cfile) { Load(cfile); }

//AppConfig& AppConfig::operator=(const AppConfig &ac)
//{
//	configfile=ac.configfile;
//	configfilepath=ac.configfilepath;
//	data=ac.data;
//	return *this;
//}

void AppConfig::setval(const std::string &k, const std::string &v, bool bcase)
{
	if (!bcase) data[lcase(k)]=v; else data[k]=v;
	if (!Save()) report_error(spf("Cannot update '", configfile, "'"), true);
}

bool AppConfig::hasval(const std::string &v)
{
	if (data.size()>0) { for (auto p:data) { if (seqs(p.second, v)) return true; }}
	return false;
}

bool AppConfig::haskey(const std::string &k, bool bcase)
{
	bool b=false;
	if (bcase) b=(data.find(k)!=data.end());
	else b=(data.find(lcase(k))!=data.end());
	return b;
}

bool AppConfig::haskeyval(const std::string &k, const std::string &v, bool bcase)
{
	return (seqs(getval(k, bcase), v));
}

const std::string AppConfig::getval(const std::string &k, bool bcase)
{
	if (haskey(k, bcase)) { if (bcase) return data[k]; else return data[lcase(k)]; }
	return "";
}

bool AppConfig::Save()
{
	file_delete(configfile);
	std::ofstream ofs(configfile);
	if (ofs.good()) { for (auto p:data) ofs << p.first << "=" << p.second << "\n"; return true; }
	return false;
}

bool AppConfig::Load(const std::string &cfile)
{
	if (!cfile.empty()) configfile=cfile;
	if (file_exist(configfile)&&!isnontextfile(configfile))
	{
		std::ifstream ifs(configfile);
		std::string s,k,v;
		size_t p;
		data.clear();
		if (ifs.good())
		{
			while (ifs.good())
			{
				std::getline(ifs, s);
				if ((p=s.find("="))!=std::string::npos) { k=s.substr(0, p); TRIM(k); if (!k.empty()) { v=s.substr(p+1); TRIM(v); data[k]=v; }}
				else { k=s; TRIM(k); if (!k.empty()) { v=""; data[k]=v; }}
			}
		}
		else return false;
	}
	return true; //non-existing is OK
}


//===================================================================================================

bool MakeAppHoM(const std::string &AppFullPath)
{
	if (AppFullPath.empty()) return tellerror("HoM: need full path to application");
	std::string appname;
	std::string apphome;
	std::string cfile;
	
	if (!HoM_PATH.empty()||HoMicile())
	{
		appname=file_name(AppFullPath);
		apphome=path_append(HoM_PATH, appname);
		//todo - check same-name apps - naaah pebcak
		if (path_realize(apphome))
		{
			cfile=path_append(apphome, appname+".config");
			AppConfig(cfile).setval("application", AppFullPath);
			return file_exist(cfile);
		}
	}
	return false;
}

bool GetAppHoMConfig(const std::string &AppFullPath, AppConfig &AC)
{
	AC.clear();
	if (MakeAppHoM(AppFullPath))
	{
		std::string appname=file_name(AppFullPath);
		std::string apphome=path_append(HoM_PATH, appname);
		std::string cfile=path_append(apphome, appname+".config");
		//AC=AppConfig(cfile);
		AC.Load(cfile);
		return true;
	}
	return false;
}

