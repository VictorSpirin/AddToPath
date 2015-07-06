#include <windows.h>
#include "reg.h"


/* Example of use
int ok=1;
 char str[128];
 DWORD d;
 Reg rg(HKEY_LOCAL_MACHINE,"Software\\VVS\\TView\\2.0");
 if(!rg.Open()) ok=rg.Create();
 if(ok){
 	if(!rg.SetValue("","125-08-49"))
   	MessageBox( 0,"Ошибка 0" ,"!!!", MB_OK | MB_ICONINFORMATION);//"Файл слишком велик!"

 	if(!rg.SetValue("INT",333))
   	MessageBox( 0,"Ошибка 1" ,"!!!", MB_OK | MB_ICONINFORMATION);//"Файл слишком велик!"

 	if(!rg.SetValue("CHAR","Test 1 Мой тест уй-я!"))
   	MessageBox( 0,"Ошибка 2" ,"!!!", MB_OK | MB_ICONINFORMATION);//"Файл слишком велик!"

 	if(!rg.GetValue("INT",&d))
   	MessageBox( 0,"Ошибка 3" ,"!!!", MB_OK | MB_ICONINFORMATION);//"Файл слишком велик!"
   else{
   	sprintf(str,"%d",d);
      MessageBox( 0,str ,"!!!", MB_OK | MB_ICONINFORMATION);//"Файл слишком велик!"
   }

 	if(!rg.GetValue("CHAR",str,128))
   	MessageBox( 0,"Ошибка 4" ,"!!!", MB_OK | MB_ICONINFORMATION);//"Файл слишком велик!"
   else MessageBox( 0,str ,"!!!", MB_OK | MB_ICONINFORMATION);//"Файл слишком велик!"
 }
 */



Reg::Reg(HKEY hKey,LPCTSTR lpSubKey)
{
 mainKey=hKey;
 subName=lpSubKey;
 isOpen=false;
}

int Reg::Open(bool ReadOnly)
{
 LONG ret;

 if(isOpen) Close();

 ret=RegOpenKeyEx(mainKey,subName,0,
	ReadOnly?KEY_READ:KEY_ALL_ACCESS,	// security access mask
	&phk 	// address of handle of open key
 );
 if(ret!=ERROR_SUCCESS) return 0;
 isOpen=true;
 return 1;
}

int Reg::Create()
{
 LONG ret;
 DWORD dw;
 if(isOpen) Close();
  ret=RegCreateKeyEx(mainKey,subName,0,
	 "",	// address of class string
		REG_OPTION_NON_VOLATILE,	// special options flag
		KEY_ALL_ACCESS,	// desired security access
		NULL	,	// address of key security structure
		&phk,	// address of buffer for opened handle
		&dw	// address of disposition value buffer
 );
 if(ret!=ERROR_SUCCESS) return 0;
 isOpen=true;
 return 1;
}

int Reg::SetValue(LPCTSTR lpValueName,char* value)
{
 LONG ret;
 ret=RegSetValueEx(phk,	// handle of key to set value for
			lpValueName,// address of value to set
				0,	// reserved
   	 		REG_SZ,	// flag for value type
	    		(BYTE *)value,	// address of value data
   	 		strlen(value)+1 	// size of value data
 );
 if(ret!=ERROR_SUCCESS) return 0;
 return 1;
}

int Reg::SetValue(LPCTSTR lpValueName,DWORD value)
{
 LONG ret;

 ret=RegSetValueEx(phk,	// handle of key to set value for
			lpValueName,// address of value to set
				0,	// reserved
			REG_DWORD,	// flag for value type
				(BYTE *)&value,	// address of value data
			sizeof(DWORD) 	// size of value data
 );
 if(ret!=ERROR_SUCCESS) return 0;
 return 1;
}

int Reg::GetValue(LPCTSTR lpValueName,char* value,int len)
{
 LONG ret;
 DWORD dw;

 dw=REG_SZ;
 //DWORD tmp=len;

 ret=RegQueryValueEx(
		phk,	// handle of key to query
		lpValueName,	// address of name of value to query
		NULL,	// reserved
	   &dw,//REG_DWORD,	// address of buffer for value type
	   (BYTE *)value,	// address of value data
	 (LPDWORD)&len	// size of value data
   );

 if(ret!=ERROR_SUCCESS){
 	*value=0;
	return 0;
 }
 return 1;
}

int Reg::GetValue(LPCTSTR lpValueName,DWORD* value)
{
 LONG ret;
 DWORD dw;

 dw=REG_DWORD;
 DWORD tmp=sizeof(DWORD);

 ret=RegQueryValueEx(
	    phk,	// handle of key to query
    	lpValueName,	// address of name of value to query
    	NULL,	// reserved
	   &dw,//REG_DWORD,	// address of buffer for value type
	   (BYTE *)value,	// address of value data
   	 &tmp	// size of value data
   );
 if(ret!=ERROR_SUCCESS){
 	*value=0;
 	return 0;
 }
 return 1;
}

void Reg::Close()
{
	if(isOpen)
		RegCloseKey(phk);
	isOpen=false;

}
