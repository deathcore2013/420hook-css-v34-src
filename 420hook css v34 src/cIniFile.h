#ifndef CINIFILE_H
#define CINIFILE_H

#include "Include.h"

class cIni
{
public:
	bool CreateCfgFile();

	int ReadInteger(char* szSection, char* szKey, int iDefaultValue);
	float ReadFloat(char* szSection, char* szKey, float fltDefaultValue);
	char* ReadString(char* szSection, char* szKey, const char* szDefaultValue);

	void WriteInteger(char* szSection, char* szKey, int iValue);
	void WriteFloat(char* szSection, char* szKey, float fltValue);
	void WriteString(char* szSection, char* szKey, char* szValue);

	int ReadMenuInteger(char* szSection, char* szKey, int iDefaultValue);
	void WriteMenuInteger(char* szSection, char* szKey, int iValue);

	void LoadSettings();
	void SaveSettings();
	void ResetColors();
};

extern char soundsdir[255];

#endif//INIREADER_H