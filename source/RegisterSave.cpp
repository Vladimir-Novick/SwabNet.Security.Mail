////////////////////////////////////////////////////////////////////////////
//	Copyright 2008 : Vladimir Novick  (  https://www.linkedin.com/in/vladimirnovick/  )
//
//    NO WARRANTIES ARE EXTENDED. USE AT YOUR OWN RISK. 
//
// To contact the author with suggestions or comments, use  :vlad.novick@gmail.com
//
////////////////////////////////////////////////////////////////////////////


// RegisterSave.cpp: implementation of the CRegisterSave class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <stdio.h>
#include "RegisterSave.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LONG RegOpenKeyExM(
				   HKEY hKey,         // handle to open key
				   LPCTSTR lpSubKey,  // subkey name
				   DWORD ulOptions,   // reserved
				   REGSAM samDesired, // security access mask
				   PHKEY phkResult    // handle to open key
				   );



CRegisterSave::CRegisterSave()
{


	strcpy(UserKeyPatch,_T("Software\\VladimirNovick\\OutlookAgent"));
	//		char szName[60];
	//		unsigned long ls=60;
	//		GetUserName(szName,&ls);
	//		strcat(UserKeyPatch,szName);
}

CRegisterSave::~CRegisterSave()
{
	
}

/*
void CRegisterSave::ActiveServer(char *psz_ServerName,Storage StorageFlag)
{
HKEY hkeyRun;
unsigned long LenRead;
DWORD dwType;
char szName[30];
unsigned long ls=30;
DWORD dwDisposition;

		GetUserName(szName,&ls);
		
		  char s_cszDebugKey[255];
		  strcpy(s_cszDebugKey,UserKeyPatch);
		  strcat(s_cszDebugKey,szName);
		  const char s_ValueName[] = _T("ServerStorage");
		  
			LenRead = 200;
			if (StorageFlag == READ ){
			//	    RegOpenKeyExM(HKEY_LOCAL_MACHINE,s_cszDebugKey,0,KEY_READ,&hkeyRun);
			RegQueryValueEx(hkeyRun,s_ValueName,NULL,&dwType,(unsigned char *)psz_ServerName,&LenRead);
			RegCloseKey(hkeyRun);
			} 
			if (StorageFlag == WRITE) 
			{
			//        RegCreateKeyEx(HKEY_LOCAL_MACHINE,s_cszDebugKey,
			0,
			"",
			REG_OPTION_NON_VOLATILE,
			KEY_ALL_ACCESS,
			NULL,&hkeyRun,&dwDisposition ); 
			RegCloseKey(hkeyRun);
			//	    RegOpenKeyExM(HKEY_LOCAL_MACHINE,s_cszDebugKey,0,KEY_WRITE,&hkeyRun);
			RegSetValueEx(hkeyRun,s_ValueName,0,REG_SZ,(const unsigned char *)psz_ServerName,0);
			RegCloseKey(hkeyRun);
			}
			}
			
*/


void CRegisterSave::AutoRun(BOOL flag)
{
	HKEY hkeyRun;
	LPTSTR lpCmdLine;
	const char s_cszDebugKey[] = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
	const char s_ValueName[] = _T("SwabNet");
	
	if (flag){
        lpCmdLine = GetCommandLine(); //this line necessary for _ATL_MIN_CRT         LPTSTR lpCmdLine;
		
			    RegOpenKeyEx(HKEY_LOCAL_MACHINE,s_cszDebugKey,0,KEY_WRITE,&hkeyRun);
				RegSetValueEx(hkeyRun,s_ValueName,0,REG_SZ,(const unsigned char *)lpCmdLine,strlen(lpCmdLine));
		      RegCloseKey(hkeyRun);
	} else
	{
			    RegOpenKeyEx(HKEY_LOCAL_MACHINE,s_cszDebugKey,0,KEY_ALL_ACCESS,&hkeyRun);
				RegDeleteValue(hkeyRun,s_ValueName);
		        RegCloseKey(hkeyRun);
	}
}



void CRegisterSave::UserKeyStore(char *key, long *value,unsigned long LenRead,Storage StoreFlag)
{
    char BufText[255];
	BufText[0]=0;
	switch(StoreFlag)
	{
	case WRITE :
		{
			sprintf(BufText,"%d",*value);
			UserKeyStore(key,BufText,strlen(BufText),CRegisterSave::WRITE);
		}
	case READ :
		{
			UserKeyStore(key,BufText,254,CRegisterSave::READ);
			*value = atol(BufText); 
		}
	}
	
}




void CRegisterSave::UserKeyStore(char *key, char *value,unsigned long LenRead,Storage StoreFlag)
{
	HKEY hkeyRun;
	DWORD dwType;
	
	
	switch(StoreFlag)
	{
	case READ :
		*value = 0;
		RegOpenKeyEx(HKEY_CURRENT_USER,UserKeyPatch,0,KEY_READ,&hkeyRun);
		RegQueryValueEx(hkeyRun,key,NULL,&dwType,(unsigned char *)value,&LenRead);
        RegCloseKey(hkeyRun);
		
		break;
	case WRITE :
		{
			RegOpenKeyEx(HKEY_CURRENT_USER,UserKeyPatch,0,KEY_WRITE,&hkeyRun);
			DWORD dwDisposition;
			
			if (hkeyRun == NULL ){
				
				
				RegCreateKeyEx(HKEY_CURRENT_USER,_T(UserKeyPatch),
					0,
					"",
					REG_OPTION_NON_VOLATILE,
					KEY_ALL_ACCESS,
					NULL, //Security
					&hkeyRun,&dwDisposition);
				
			}
			
			
			RegSetValueEx(hkeyRun,key,0,REG_SZ,(const unsigned char *)value,LenRead);
			RegFlushKey(hkeyRun);
			RegCloseKey(hkeyRun);
		}
		break;
	case DELETE :
		RegOpenKeyEx(HKEY_CURRENT_USER,UserKeyPatch,0,KEY_ALL_ACCESS,&hkeyRun);
		RegDeleteValue(hkeyRun,key);
        RegCloseKey(hkeyRun);
		break;
	}
}
