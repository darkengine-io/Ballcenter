#include <windows.h>
#include <atlstr.h>
#include <stdint.h>
#include "Serial.h"

Serial::Serial(CString PortSpecifier)
{
	hPort = CreateFile(
		PortSpecifier,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL
		);
	dcb.BaudRate = CBR_38400; //38400 Baud
	dcb.ByteSize = 8; //8 data bits
	dcb.Parity = NOPARITY; //no parity
	dcb.StopBits = ONESTOPBIT; //1 stop
	dcb.fDtrControl = DTR_CONTROL_ENABLE;
	SetCommMask(hPort, EV_RXCHAR | EV_ERR); //receive character event
	WaitCommEvent(hPort, &dwCommModemStatus, 0); //wait for character
}

Serial::~Serial()
{
	CloseHandle(hPort);
}


uint8_t Serial::ReadByte()
{


	ReadFile(hPort, &myByte, 1, &dwBytesTransferred, NULL); //read 1
	return myByte;
}