#ifndef INC_SD_FILE_H_
#define INC_SD_FILE_H_
#include "main.h"
#include "fatfs.h"
#include <stdio.h>
#include <string.h>
class _FILE{
private:
	char filename[20];
	FATFS  _FatFs; 	//Fatfs handle
	FIL  _file; 		//File handle
	FRESULT  _fres;   //Result after operations
public:
	_FILE(FIL  file,FATFS  FatFs,FRESULT  fres, const char * name){
		this->_file = file;
		this->_FatFs = FatFs;
		this->_fres = fres;
		strcpy(filename,name);
	}

	void read(){
	  //Now let's try to open file "test.txt"
	  _fres = f_open(&_file, filename , FA_READ);
	  if (_fres != FR_OK) {
		printf("f_open error (%i)\r\n", _fres);
		while(1);
	  }
	  printf("I was able to open 'log_file.txt' for reading!\r\n");

	  //Read 30 bytes from "test.txt" on the SD card
	  BYTE readBuf[200];

	  //We can either use f_read OR f_gets to get data out of files
	  //f_gets is a wrapper on f_read that does some string formatting for us
	  while(!f_eof(&_file)){
		  TCHAR* rres = f_gets((TCHAR*)readBuf, 200, &_file);
		  if(rres != 0) {
			printf("Read string from 'log_file.txt' contents: %s\r\n", readBuf);
		  } else {
			printf("f_gets error (%i)\r\n", _fres);
		  }
	  }
	  //Be a tidy kiwi - don't forget to close your file!
	  f_close(&_file);
	}
	void write(char * writeBuf ){
	  //Now let's try and write a file "write.txt"
	  _fres = f_open(&_file, filename, FA_WRITE | FA_OPEN_APPEND );
	  if(_fres == FR_OK) {
		printf("I was able to open 'file.txt' for writing\r\n");
	  } else {
		printf("f_open error (%i)\r\n", _fres);
	  }

	  //Copy in a string
	  UINT bytesWrote;
	  _fres = f_write(&_file, writeBuf, strlen(writeBuf), &bytesWrote);
	  if(_fres == FR_OK) {
		printf("Wrote %i bytes to 'log_file.txt'!\r\n", bytesWrote);
	  } else {
		printf("f_write error (%i)\r\n", _fres);
	  }

	  //Be a tidy kiwi - don't forget to close your file!

	  f_close(&_file);

	  //We're done, so de-mount the drive

}
	void FILE_delete()
	{
		f_unlink(filename);
	}

};

#endif /* INC_SD_FILE_H_ */
