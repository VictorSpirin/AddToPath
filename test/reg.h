class Reg
{
 public:
 HKEY mainKey;
 HKEY phk;
 LPCTSTR subName;
 bool isOpen;

 Reg(HKEY hKey,LPCTSTR lpSubKey);
 ~Reg()
 {
 	Close();
 };
 //Open a Key, return 0 in error
 int Open(bool ReadOnly=false);
 //Create if key not present
 int Create();

 void Close();

 int SetValue(LPCTSTR lpValueName,char* value);
 int SetValue(LPCTSTR lpValueName,DWORD value);

 int GetValue(LPCTSTR lpValueName,char* value,int len);
 int GetValue(LPCTSTR lpValueName,DWORD* value);

};
