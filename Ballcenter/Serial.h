#include <windows.h>
#include <atlstr.h>
#include <stdint.h>

class Serial
{
public:
	Serial(CString PortSpecifier);
	~Serial();
	uint8_t ReadByte(uint8_t *buffer, unsigned int nbChar);

private:
	HANDLE hPort;
	DCB dcb;
	BYTE myByte;
	DWORD dwBytesTransferred;
	DWORD dwCommModemStatus;
	//Serial comm handler
	//HANDLE hSerial;
	//Connection status
	bool connected;
	//Get various information about the connection
	COMSTAT status;
	//Keep track of last error
	DWORD errors;
};