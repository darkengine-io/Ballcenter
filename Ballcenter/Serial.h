#include <windows.h>
#include <atlstr.h>
#include <stdint.h>

class Serial
{
public:
	Serial(CString PortSpecifier);
	~Serial();
	uint8_t ReadByte();

private:
	HANDLE hPort;
	DCB dcb;
	BYTE myByte;
	DWORD dwBytesTransferred;
	DWORD dwCommModemStatus;
};