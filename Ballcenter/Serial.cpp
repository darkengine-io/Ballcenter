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


}

Serial::~Serial()
{
	CloseHandle(hPort);
}


uint8_t Serial::ReadByte(uint8_t *buffer, unsigned int nbChar)
{
	/*
	SetCommState(hPort, &dcb);
	ReadFile(hPort, &myByte, 1, &dwBytesTransferred, NULL); //read 1
	return myByte;*/



	//Number of bytes we'll have read
	DWORD bytesRead;
	//Number of bytes we'll really ask to read
	unsigned int toRead;

	//Use the ClearCommError function to get status info on the Serial port
	ClearCommError(this->hPort, &this->errors, &this->status);

	//Check if there is something to read
	if (this->status.cbInQue>0)
	{
		//If there is we check if there is enough data to read the required number
		//of characters, if not we'll read only the available characters to prevent
		//locking of the application.
		if (this->status.cbInQue>nbChar)
		{
			toRead = nbChar;
		}
		else
		{
			toRead = this->status.cbInQue;
		}

		//Try to read the require number of chars, and return the number of read bytes on success
		if (ReadFile(this->hPort, buffer, toRead, &bytesRead, NULL) && bytesRead != 0)
		{
			return bytesRead;
		}

	}

	//If nothing has been read, or that an error was detected return -1
	return -1;
}