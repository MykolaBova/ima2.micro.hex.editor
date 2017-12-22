/*
	IMA
	Simple command-line editor of binary files (images)
	Functions:
	---
	- report piece of binary file
	- write one byte to binary file
	- convert hex to int
	- see current configuration
*/
/*
	Nick Bova (NB) Kiev, Ukraine
	system32@narod.ru
	www.system32.narod.ru (russian)
*/
/*

command: 
* 	report piece of binary file
	ima2.exe a
	ima2.exe b
	ima2.exe d
	ima2.exe e
required:
  ima2.exe f 12.txt     - name of file to read from
  ima2.exe o 2          - offset in file
  ima2.exe n 1          - number of bytes to read

command: 
* 	write one byte to binary file
	ima2.exe g
	ima2.exe h
required:
	ima2.exe f 12.txt   - name of file to read from
	ima2.exe o 2        - offset in file

command: 
* 	convert hex to int
	ima2.exe v
required:
	ima2.exe s 1       - size of hex to convert

command: 
* 	see current configuration
	ima2.exe c
*/

// ima1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <cstdio>

// 4K
// 4096
// 100k
#define kMaxBuffer 102400

void write1();
void write2();
void write3(char i);
void write4(unsigned long i);
bool checkHex(char i);
int	 hex2long();
void write5();

void hex2short();
void hex2int();
void hex2ulong();

void writeConfig();
void readConfig();
void about();

void Size(::std::string param);
void File(::std::string param);
void Offset(::std::string param);
void Count(::std::string param);
void Config();
void Convert();
void getHex();
void getHex1();
void getHex2();
void getHex3();

void setHex();
void processHex1(unsigned char Num);

void processHex2();

void char2hex(unsigned char Symbol,unsigned char& Res0,unsigned char& Res1);
void processHex(unsigned char Num);
void char2hex8(unsigned long Offset);
void char2hex8_1(unsigned long Offset);

void Test0();	// structure allign
void Test1();	// write int and long
void Tests(::std::string param);

struct {
	// 4
	enum {
		one = 1,
		two = 2,
		four = 4
	} n_size;
	// 10
	char f_name[10];
	// int -> 4
	int offset;
	// int -> 4
	int count;
} config;

	// 8 !!!
struct {
	// allign by 4*word border
	// 4
	enum {
		one = 1,
		two = 2,
		four = 4
	} n_size;
	// 1
	char f_name;
} config1;

int main(int argc, char* argv[])
{
	readConfig();

	int iii = 0;
	if(argc == 3) {
		::std::string command = argv[1]; 
		::std::string param = argv[2];

		if(command		== "size"	|| command == "s")
			Size(param);
		else if(command == "file"	|| command == "f")
			File(param);
		else if(command == "offset" || command == "o")
			Offset(param);
		else if(command == "count"	|| command == "n")
			Count(param);
		else if(command == "test"	|| command == "t")
			Tests(param);
		else
			about();	

		int iii = 0;
	}
	else if(argc == 2) {
		::std::string command = argv[1]; 
		if(command		== "config"		|| command == "c")
			Config();
		else if(command == "convert"	|| command == "v")
			Convert();
		else if(command == "read"		|| command == "a")
			getHex();
		else if(command == "read1"		|| command == "b")
			getHex1();
		else if(command == "read2"		|| command == "d")
			getHex2();
		else if(command == "read3"		|| command == "e")
			getHex3();
		else if(command == "write"		|| command == "g")
			setHex();
		else if(command == "write1"		|| command == "h")
			processHex2();
		else if(command == "test"	|| command == "t")
			Tests("-1");
		else
			about();
	}
	else
		about();

	return 0;
}
// ---
void write1() {
	FILE *fp;
	// 4 bytes
	int i = 65537;
	if((fp = fopen("write1","wb")) == NULL) {
		::std::cout << "Can not open file" << ::std::endl;
		exit(1);
	}
	fseek(fp,0,SEEK_SET);
	fwrite(&i,sizeof(int),1,fp);
	fclose(fp);
}

void write2() {
	FILE *fp;
	// p3 * 16777216 + p2 * 65536 + p1 * 256 + p0
	// p3 * ( 256 ^ 3) + p2*(256 ^ 2) + p1 * (256 ^ 1) + p0 * (256 ^ 0)
	// ---
	// 4 bytes
	// 16777216 = 65536 * 256
	// unsigned int i = 16777216;	// 00 00 00 01  // 01 00 00 00 // 01 * 16777216
	// unsigned int i = 33554432;	// 00 00 00 02  // 02 00 00 00 // 02 * 16777216 = 33554432
	// ---
	// unsigned int i = 65537;	// 01 00 01 00	// 00 01 00 01 // 01 * 65536 + 00 * 256 + 01 = 65536  + 1 = 65536
	// 65536 / 256 = 256
	// unsigned int i = 65535;	// FF FF 00 00  //	255 * 256 + 255 = 65280 + 255 = 65535
	// unsigned int i = 10;		// 0A 00 00 00	//	00 * 256 + 10
	// 1 bytes
	// char i = 10;
	// ---
	// 2 bytes
	// short i = 10;	// 0A 00
	// short i = 256;	// 00 01	//  01 * 256 + 00
	// short i = 255;	// FF 00	//  00 * 256 + 255
	// short i = 521;	// 09 02	//  02 * 256 + 09 = 512 + 9 = 521
	// ---
	// 4 bytes
	// long i = 1;
	// ---
	// FF FF FF FF
	int p0 = 255;
	int p1 = 255;
	int p2 = 255;
	int p3 = 255;
	// ---
	long i = p3 * 16777216 + p2 * 65536 + p1 * 256 + p0;
	// 8 bytes
	// double long i = 1;


	if((fp = fopen("write2","wb")) == NULL) {
		::std::cout << "Can not open file" << ::std::endl;
		exit(1);
	}
	fseek(fp,0,SEEK_SET);
	//fwrite(&i,sizeof(int),1,fp);
	//fwrite(&i,sizeof(char),1,fp);
	//fwrite(&i,sizeof(unsigned short),1,fp);
	//fwrite(&i,sizeof(double long),1,fp);
	fwrite(&i,sizeof(long),1,fp);
	fclose(fp);
}

int hex2long() {
	char a[2];
	// 0F
	// 0
	::std::cin >> a[1];
	// F
	::std::cin >> a[0];
	//::std::cout << ::std::endl;
	//::std::cout << "HEX: " << ::std::endl;
	//
	::std::cout << a[1];
	//
	::std::cout << a[0];
	::std::cout << " ";
	//::std::cout << ::std::endl;
	//::std::cout << "DEC: " << ::std::endl;
	// ---

	// ---
	// '0' - num.
	// 'A' - alfa
	int n0 = a[0];
	if(isdigit(n0))
		n0 = n0 - '0';
	else if(isalpha(n0) && checkHex(a[0])) {
		n0 = toupper(n0);
		n0 = n0 - 'A' + 10;
	}
	else {
		::std::cout << "wrong symbol: ";
		::std::cout << a[1];
		::std::cout << a[0];
		return -1;
	}

	int n1 = a[1];
	if(isdigit(n1))
		n1 = n1 - '0';
	else if(isalpha(n1) && checkHex(a[1])) {
		n1 = toupper(n1);
		n1 = n1 - 'A' + 10;
	}
	else {
		::std::cout << "wrong symbol: ";
		::std::cout << a[1];
		::std::cout << a[0];
		return -1;
	}

	// ---
	int p = n1 * 16 + n0 ;
	//::std::cout << p;
	// ---
	return p;
}

bool checkHex(char i) {
	i = toupper(i);
	if(i == 'A' || i == 'B' || i == 'C' || i == 'D' || i == 'E' || i == 'F')
		return true;
	else
		return false;
}

void write3(char i) {
	FILE *fp;

	if((fp = fopen("write1","wb")) == NULL) {
		::std::cout << "Can not open file" << ::std::endl;
		exit(1);
	}
	fseek(fp,0,SEEK_SET);
	fwrite(&i,sizeof(char),1,fp);
	fclose(fp);
}

void write4(unsigned long i) {
	FILE *fp;

	if((fp = fopen("write1","wb")) == NULL) {
		::std::cout << "Can not open file" << ::std::endl;
		exit(1);
	}
	fseek(fp,0,SEEK_SET);
	fwrite(&i,sizeof(unsigned long),1,fp);
	fclose(fp);
}

void hex2short(){
	::std::cout << "HEX:   ";
	int p0 = hex2long();
	if(p0 >= 0) {
		::std::cout << ::std::endl;
		::std::cout << "SHORT: " << p0 << ::std::endl;
	}
}

void hex2int(){
	::std::cout << "HEX: ";
	int p1 = hex2long();
	int p0 = hex2long();
	if(p0 > 0 && p1 > 0) {
		long i = p1 * 256 + p0;
		::std::cout << ::std::endl;
		::std::cout << "INT: " << i << ::std::endl;
	}
}

void hex2ulong(){
	::std::cout << "HEX:  ";
	int p3 = hex2long();
	int p2 = hex2long();
	int p1 = hex2long();
	int p0 = hex2long();
	if(p0 > 0 && p1 > 0 && p2 > 0 && p3 > 0) {
		unsigned long i = p3 * 16777216 + p2 * 65536 + p1 * 256 + p0;	
		::std::cout << ::std::endl;
		::std::cout << "LONG: " << i << ::std::endl;
	}
}

void writeConfig() {
	FILE *fp;
	// 4 bytes
	int i = 65537;
	if((fp = fopen("ima.cfg","wb")) == NULL) {
		::std::cout << "Can not open file" << ::std::endl;
		exit(1);
	}
	fseek(fp,0,SEEK_SET);
	fwrite(&config,sizeof(config),1,fp);
	fclose(fp);
}

void readConfig() {
	FILE *fp;
	// 4 bytes
	int i = 65537;
	if((fp = fopen("ima.cfg","rb")) == NULL) {
		
		config.n_size = config.one;
		writeConfig();		
		Config();

		exit(1);
	}
	fseek(fp,0,SEEK_SET);
	fread(&config,sizeof(config),1,fp);
	fclose(fp);
}

void about() {
		::std::cout << "=== NB IMA UTILITY" << ::std::endl;
		::std::cout << "----------------------------------------------------------" << ::std::endl;
		::std::cout << "SET         SIZE            size    |   s    one|two|four " << ::std::endl;
		::std::cout << "SET         FILE NAME       file    |   f" << ::std::endl;
		::std::cout << "SET         FILE OFFSET     offset  |   o" << ::std::endl;
		::std::cout << "SET         FILE COUNT      count   |   n" << ::std::endl;
		::std::cout << "----------------------------------------------------------" << ::std::endl;
		::std::cout << "GET         CONFIGURATION   config  |   c" << ::std::endl;
		::std::cout << "----------------------------------------------------------" << ::std::endl;	
		::std::cout << "CONVERT                     convert |   v" << ::std::endl;
		::std::cout << "----------------------------------------------------------" << ::std::endl;
		::std::cout << "READ1       (SIMPLEST)      read    |   a" << ::std::endl;
		::std::cout << "READ2       (good header)   read1   |   b" << ::std::endl;
		::std::cout << "READ3       (good greed)    read2   |   d" << ::std::endl;
		::std::cout << "READ4       (good report)   read3   |   e" << ::std::endl;
		::std::cout << "----------------------------------------------------------" << ::std::endl;
		::std::cout << "WRITE1      (integer)       write   |   g" << ::std::endl;
		::std::cout << "WRITE2      (hex)           write1  |   h" << ::std::endl;
		::std::cout << "----------------------------------------------------------" << ::std::endl;
}

void Size(::std::string param) {
	if(param == "one" || param == "1")
		config.n_size = config.one;
	else if(param == "two" || param == "2")
		config.n_size = config.two;
	else if(param == "four" || param == "4")
		config.n_size = config.four;
	else {
		::std::cout << "=== SET SIZE OF HEX NUMBER TO CONVERT size|s  one|two|four " << ::std::endl;
		config.n_size = config.one;
	}

	writeConfig();
	Config();
}

void Config() {
	::std::cout << "=== NB IMA UTILITY CONFIGURATION: " << ::std::endl;
	::std::cout << "--- SIZE   :  "	<< config.n_size	<< ::std::endl;
	::std::cout << "--- FILE   :  "	<< config.f_name	<< ::std::endl;
	::std::cout << "--- OFFSET :  "	<< config.offset	<< ::std::endl;
	::std::cout << "--- COUNT  :  "	<< config.count		<< ::std::endl;
}

void Convert() {
	Config();
	if(config.n_size == config.one) {
		hex2short();
	}
	else if(config.n_size == config.two) {
		hex2int();
	}
	else if (config.n_size == config.four) {
		hex2ulong();
	}
	else {
		::std::cout << "=== SET SIZE OF HEX NUMBER TO CONVERT size|s  one|two|four " << ::std::endl;
		config.n_size = config.one;
		writeConfig();
	}
}

void File(::std::string param) {
	for(int i = 0;i < 10;i++)
			config.f_name[i] = 0;

	if(param.size() < 10) {
		const char *cptr = param.c_str();
		for(int i = 0;i < 10 && cptr[i] != 0;i++)
				config.f_name[i] = cptr[i];
	}

	writeConfig();
	Config();
}

void Offset(::std::string param) {
	config.offset = atoi(param.c_str());

	writeConfig();
	Config();
}

void Count(::std::string param) {
	if(atoi(param.c_str()) > kMaxBuffer)
		config.count = kMaxBuffer;
	else
		config.count = atoi(param.c_str());

	writeConfig();
	Config();
}

void getHex() {
	FILE *fp;
	// 4 bytes
	// unsigned char* inPtr;
	// inPtr = new unsigned char[config.count];
	unsigned char inPtr[kMaxBuffer];

	if(config.f_name[0] == 0) {
		::std::cout << "Wrong file name" << ::std::endl;		
		exit(1);
	}

	if((fp = fopen(config.f_name,"rb")) == NULL) {
		::std::cout << "Can not open file" << ::std::endl;		
		exit(1);
	}
	fseek(fp,config.offset,SEEK_SET);
	unsigned long r_count = fread(&inPtr,sizeof(unsigned char),config.count,fp);

	if(r_count == 0) {
		::std::cout <<  "Possible count longer than file length" << ::std::endl;		
		exit(1);
	}

	int a11 = 0;
	int b11 = 0;

	::std::cout	 << ::std::endl;
	::std::cout  << "          | 00 01 02 03 04 05 06 07 | 08 09 0A 0B 0C 0D 0E 0F |" << ::std::endl;
	::std::cout  << "--------- | ----------------------- | ----------------------- |" << ::std::endl;

	for(int i = 0; i < config.count && i < r_count; i++) {
		if(b11 == 0) {
			char2hex8(i);
		}		
		
		a11++;
		b11++;


		processHex(inPtr[i]);
		::std::cout  << " ";

		if(a11 == 8) {
			::std::cout  << "| ";
			a11 = 0;
		}
		if(b11 == 16) {
			::std::cout  << ::std::endl;
			b11 = 0;
		}
	}

	int iii = 0;
	fclose(fp);
}

void char2hex(unsigned char Symbol,unsigned char& Res0,unsigned char& Res1) {
	// unsigned char a[2];
	// --- FA
	// --- a[0] = 'A';
	// --- a[1] = 'F';
	int r0 = 0;
	int r1 = 0;
	unsigned char rr0 = 0;
	unsigned char rr1 = 0;

	r1 = Symbol/16;		
	if(r1 >= 10)
		rr1 = r1 + 'A' - 10;
	else
		rr1 = r1 + '0';

	r0 = Symbol - (r1*16);
	if(r0 >= 10)
		rr0 = r0 + 'A' - 10;
	else
		rr0 = r0 + '0';

	Res0 = rr0;
	Res1 = rr1;

	// Res0 = a[0];
	// Res1 = a[1];

}

void processHex(unsigned char Num) {
	unsigned char a0;
	unsigned char a1;
	char2hex(Num,a0,a1);
	//::std::cout << "HEX : ";
	::std::cout << a1;
	::std::cout << a0;
}

void write5() {
	FILE *fp;
	// 4 bytes
	// int i = 65537;
	if((fp = fopen("ima1.cfg","wb")) == NULL) {
		::std::cout << "Can not open file" << ::std::endl;
		exit(1);
	}
	fseek(fp,0,SEEK_SET);
	fwrite(&config1,sizeof(config1),1,fp);
	fclose(fp);
}

// offset
void char2hex8(unsigned long Offset) {
	int r[8];
	int deg[8];
	int t_deg;
	// int res;
	int t_Offset;
	unsigned char rr[8];	

	
	deg[0] = 1;
	t_deg = 16;
	for(int i = 1;i < 8; i++) {		
		deg[i]	= t_deg;
		t_deg	= t_deg*16;
	}

	for(i = 0;i < 8; i++) {
		r[i] = 0;
		rr[i] = '0';
	}

	// 16*16*16*16*16*16*16	= 268435456		// 10000000
	// 16*16*16*16*16*16	= 16777216		// 01000000
	// 16*16*16*16*16		= 1048576		// 00100000
	// 16*16*16*16	 		= 65536			// 00010000
	// 16*16*16				= 4096			// 00001000
	// 16*16				= 256			// 00000100
	// 16					= 16			// 00000010
	// 0					= 1				// 00000001

	
	t_Offset = Offset;

	for(i = 7;i >= 0; i--) {
		r[i] = t_Offset / deg[i];
		if(r[i] >= 10)
			rr[i] = r[i] + 'A' - 10;
		else
			rr[i] = r[i] + '0';
		t_Offset = t_Offset - r[i]*deg[i];
	}

	::std::cout << " ";
	for(i = 7;i >=0;  i--) {
		::std::cout << rr[i];
	}
	// ::std::cout << ::std::endl;
	::std::cout << " | ";
}

void getHex1() {

	Config();

	FILE *fp;
	// 4 bytes
	// unsigned char* inPtr;
	// inPtr = new unsigned char[config.count];
	unsigned char inPtr[kMaxBuffer];

	if(config.f_name[0] == 0) {
		::std::cout << "Wrong file name" << ::std::endl;		
		exit(1);
	}

	if((fp = fopen(config.f_name,"rb")) == NULL) {
		::std::cout << "Can not open file" << ::std::endl;		
		exit(1);
	}
	fseek(fp,config.offset,SEEK_SET);
	unsigned long r_count = fread(&inPtr,sizeof(unsigned char),config.count,fp);

	if(r_count == 0) {
		::std::cout <<  "Possible count longer than file length" << ::std::endl;		
		exit(1);
	}

	int a11		= 0;
	int b11		= 0;
	int line	= 0;

	::std::cout	 << ::std::endl;
	::std::cout  << "| -------- | ----------------------- | ----------------------- |" << ::std::endl;
	::std::cout  << "|  OFFSET  | 00 01 02 03 04 05 06 07 | 08 09 0A 0B 0C 0D 0E 0F |" << ::std::endl;
	::std::cout  << "| -------- | ----------------------- | ----------------------- |" << ::std::endl;

	for(int i = 0; i < config.count && i < r_count; i++) {
		if(b11 == 0) {
			::std::cout  << "|";
			char2hex8_1(i);
		}		
		
		a11++;
		b11++;


		processHex(inPtr[i]);
		::std::cout  << " ";

		if(a11 == 8) {
			::std::cout  << "| ";
			a11 = 0;
		}
		if(b11 == 16) {
			::std::cout  << ::std::endl;
			b11 = 0;
			line ++;

			if(line == 16) {
				::std::cout  << "| -------- | ----------------------- | ----------------------- |" << ::std::endl;
				::std::cout  << "|  OFFSET  | 00 01 02 03 04 05 06 07 | 08 09 0A 0B 0C 0D 0E 0F |" << ::std::endl;
				::std::cout  << "| -------- | ----------------------- | ----------------------- |" << ::std::endl;
			}
		}
	}

	int iii = 0;
	fclose(fp);
}

// offset
void char2hex8_1(unsigned long Offset) {
	int r[8];
	int deg[8];
	int t_deg;
	// int res;
	int t_Offset;
	unsigned char rr[8];	

	
	deg[0] = 1;
	t_deg = 16;
	for(int i = 1;i < 8; i++) {		
		deg[i]	= t_deg;
		t_deg	= t_deg*16;
	}

	for(i = 0;i < 8; i++) {
		r[i] = 0;
		rr[i] = '0';
	}

	// 16*16*16*16*16*16*16	= 268435456		// 10000000
	// 16*16*16*16*16*16	= 16777216		// 01000000
	// 16*16*16*16*16		= 1048576		// 00100000
	// 16*16*16*16	 		= 65536			// 00010000
	// 16*16*16				= 4096			// 00001000
	// 16*16				= 256			// 00000100
	// 16					= 16			// 00000010
	// 0					= 1				// 00000001

	
	t_Offset = Offset;

	for(i = 7;i >= 0; i--) {
		r[i] = t_Offset / deg[i];
		if(r[i] >= 10)
			rr[i] = r[i] + 'A' - 10;
		else
			rr[i] = r[i] + '0';
		t_Offset = t_Offset - r[i]*deg[i];
	}

	::std::cout << " ";
	for(i = 7;i >=0;  i--) {
		::std::cout << rr[i];
	}
	// ::std::cout << ::std::endl;
	::std::cout << " | ";
}

void getHex2() {

	Config();

	FILE *fp;
	// 4 bytes
	// unsigned char* inPtr;
	// inPtr = new unsigned char[config.count];
	unsigned char inPtr[kMaxBuffer];

	if(config.f_name[0] == 0) {
		::std::cout << "Wrong file name" << ::std::endl;		
		exit(1);
	}

	if((fp = fopen(config.f_name,"rb")) == NULL) {
		::std::cout << "Can not open file" << ::std::endl;		
		exit(1);
	}
	fseek(fp,config.offset,SEEK_SET);
	unsigned long r_count =  fread(&inPtr,sizeof(unsigned char),config.count,fp);

	if(r_count == 0) {
		::std::cout <<  "Possible count longer than file length" << ::std::endl;		
		exit(1);
	}

	int a11		= 0;
	int b11		= 0;
	int line	= 0;
	int lc	= 0;

	::std::cout	 << ::std::endl;
	::std::cout  << "|-------| -------- | ----------------------- | ----------------------- |" << ::std::endl;
	::std::cout  << "| LINE  |  OFFSET  | 00 01 02 03 04 05 06 07 | 08 09 0A 0B 0C 0D 0E 0F |" << ::std::endl;
	::std::cout  << "|-------| -------- | ----------------------- | ----------------------- |" << ::std::endl;


	::std::cout  << "| ";
	printf("%05d",line);
	::std::cout  << " ";

	for(int i = 0; i < config.count && i < r_count; i++) {
		if(b11 == 0) {
			::std::cout  << "|";
			char2hex8_1(i);
		}		
		
		a11++;
		b11++;


		processHex(inPtr[i]);
		::std::cout  << " ";

		if(a11 == 8) {
			::std::cout  << "| ";
			a11 = 0;
		}
		if(b11 == 16) {
			::std::cout  << ::std::endl;
			b11 = 0;
			line ++;
			lc ++;

			if(lc == 16) {
				//::std::cout	 << ::std::endl;
				::std::cout  << "|-------| -------- | ----------------------- | ----------------------- |" << ::std::endl;
				::std::cout  << "| LINE  |  OFFSET  | 00 01 02 03 04 05 06 07 | 08 09 0A 0B 0C 0D 0E 0F |" << ::std::endl;
				::std::cout  << "|-------| -------- | ----------------------- | ----------------------- |" << ::std::endl;
				// !!!
				lc = 0;
			}

			if(i < config.count-1) {
				::std::cout  << "| ";
				printf("%005d",line);
				::std::cout  << " ";
			}
		}
		
	}
	for(;b11 < 16 && b11 != 0;b11++) {
		::std::cout  << "__ ";	

		if(b11 == 7) {
			::std::cout  << "| ";			
		}

		if(b11 == 15) {

			::std::cout  << "|" << ::std::endl;
		}
	}	

	::std::cout  << "|-------| -------- | ----------------------- | ----------------------- |" << ::std::endl;

	int iii = 0;
	fclose(fp);
}

void getHex3() {

	Config();

	FILE *fp;
	// 4 bytes
	// unsigned char* inPtr;
	// inPtr = new unsigned char[config.count];
	// 4K - max
	unsigned char inPtr[kMaxBuffer];
	int last_line = config.count / 16;
	int u = 0;
	int a_count = 0;

	if(config.f_name[0] == 0) {
		::std::cout << "Wrong file name" << ::std::endl;		
		exit(1);
	}

	if((fp = fopen(config.f_name,"rb")) == NULL) {
		::std::cout << "Can not open file" << ::std::endl;		
		exit(1);
	}
	fseek(fp,config.offset,SEEK_SET);
	unsigned long r_count =  fread(&inPtr,sizeof(unsigned char),config.count,fp);

	if(r_count == 0) {
		::std::cout <<  "Possible count longer than file length" << ::std::endl;		
		exit(1);
	}
		
	int a11		= 0;
	int b11		= 0;
	int line	= 0;
	int lc	= 0;

	::std::cout  <<::std::endl;
	::std::cout  << " A - 10"  << ::std::endl;
	::std::cout  << " B - 11"  << ::std::endl;
	::std::cout  << " C - 12"  << ::std::endl;
	::std::cout  << " D - 13"  << ::std::endl;
	::std::cout  << " E - 14"  << ::std::endl;
	::std::cout  << " F - 15"  << ::std::endl;	

	::std::cout	 << ::std::endl;
	::std::cout  << "| ----- | -------- | ---------- | ----------------------- | ----------------------- | ------------------------------- |" << ::std::endl;
	::std::cout  << "| LINE  |  OFFSET  | DEC OFFSET | 00 01 02 03 04 05 06 07 | 08 09 0A 0B 0C 0D 0E 0F | 0 1 2 3 4 5 6 7 8 9 A B C D E F |" << ::std::endl;
	::std::cout  << "| ----- | -------- | ---------- | ----------------------- | ----------------------- | ------------------------------- |" << ::std::endl;


	::std::cout  << "| ";
	printf("%05d",line);
	::std::cout  << " ";

	for(int i = 0; i < config.count && i < r_count; i++) {
		if(b11 == 0) {
			::std::cout  << "|";
			char2hex8_1(i);
			
			printf("%010d",i);
			::std::cout  << " | ";
		}		
		
		a11++;
		b11++;


		processHex(inPtr[i]);
		::std::cout  << " ";

		if(a11 == 8) {
			::std::cout  << "| ";
			a11 = 0;
		}

		if((b11 == 16 || i >= config.count-1) && line < last_line) {

			for(u = 0;u <= 15;u++) {

				if(a_count < config.count) {
					
					// 0
					if(inPtr[i+u-15] == 0) {
						::std::cout << '-';
						::std::cout << " ";
					}
					// FF
					else if(inPtr[i+u-15] == 255) {
						::std::cout << '+';
						::std::cout << " ";		
					}
					// 
					else if(inPtr[i+u-15] == 9) {
						::std::cout << '*';
						::std::cout << " ";		
					}
					// 
					else if(inPtr[i+u-15] == 13) {
						::std::cout << '*';
						::std::cout << " ";		
					}
					else if(inPtr[i+u-15] == 10) {
						::std::cout << '*';
						::std::cout << " ";		
					}
					else if(inPtr[i+u-15] == 7) {
						::std::cout << '*';
						::std::cout << " ";		
					}

					else {
						::std::cout << inPtr[i+u-15];
						::std::cout << " ";
					}

				} 
				else {
					::std::cout << '=';
					::std::cout << " ";
				}

				a_count++;

				if(u == 15) {
					::std::cout  << "|";
				}
			}
		}


		if(b11 == 16) {
			::std::cout  << ::std::endl;
			b11 = 0;
			line ++;
			lc ++;

			if(lc == 16) {
				//::std::cout	 << ::std::endl;
				::std::cout  << "| ----- | -------- | ---------- | ----------------------- | ----------------------- | ------------------------------- |" << ::std::endl;
				::std::cout  << "| LINE  |  OFFSET  | DEC OFFSET | 00 01 02 03 04 05 06 07 | 08 09 0A 0B 0C 0D 0E 0F | 0 1 2 3 4 5 6 7 8 9 A B C D E F |" << ::std::endl;
				::std::cout  << "| ----- | -------- | ---------- | ----------------------- | ----------------------- | ------------------------------- |" << ::std::endl;
				// !!!
				lc = 0;
			}

			if(i < config.count-1) {
				::std::cout  << "| ";
				printf("%005d",line);
				::std::cout  << " ";
			}
		}
	}
	
	for(;b11 < 16 && b11 != 0;b11++) {
	::std::cout  << "__ ";	

		if(b11 == 7) {
			::std::cout  << "| ";			
		}
	}
	
	if(line == last_line && a_count < config.count) {
		// 
		::std::cout  << "| ";
		for(int u = 0;u <= 15;u++) {

			// ::std::cout  << "| ";
			if(a_count < config.count) {
				
				if(inPtr[i+u-15] == 0) {
					::std::cout << '-';
					::std::cout << " ";
				}
				// FF
				else if(inPtr[i+u-15] == 255) {
					::std::cout << '+';
					::std::cout << " ";		
				}
				// 
				else if(inPtr[i+u-15] == 9) {
					::std::cout << '*';
					::std::cout << " ";		
				}
				// 
				else if(inPtr[i+u-15] == 13) {
					::std::cout << '*';
					::std::cout << " ";		
				}
				else if(inPtr[i+u-15] == 10) {
					::std::cout << '*';
					::std::cout << " ";		
				}
				else if(inPtr[i+u-15] == 7) {
					::std::cout << '*';
					::std::cout << " ";		
				}

				else {
					::std::cout << inPtr[i+u-15];
					::std::cout << " ";
				}
			} 
			else {
				::std::cout << '=';
				::std::cout << " ";
			}

			a_count++;

		}
// /*
		if(u == 16) {

			::std::cout  << "|"; //  << ::std::endl;
			// 
			::std::cout  << ::std::endl;
		}
// */
	}
	
	// ::std::cout  << ::std::endl;
	::std::cout  << "| ----- | -------- | ---------- | ----------------------- | ----------------------- | ------------------------------- |" << ::std::endl;

	int iii = 0;
	fclose(fp);
}

void setHex(){
	::std::cout << "INT : ";
	unsigned int i2;
	::std::cin >> i2;
	unsigned char i1 = i2;

	write3(i1);
	processHex1(i1);

}

void processHex1(unsigned char Num) {
	unsigned char a0;
	unsigned char a1;
	char2hex(Num,a0,a1);
	//
	::std::cout << "HEX : ";
	// 
	::std::cout << a1;
	// 
	::std::cout << a0;

	FILE *fp;
	// 4 bytes
	int i = 65537;
	if((fp = fopen(config.f_name,"wb")) == NULL) {
		::std::cout << "Can not open file" << ::std::endl;
		exit(1);
	}
	fseek(fp,config.offset,SEEK_SET);
	fwrite(&Num,sizeof(int),1,fp);
	fclose(fp);
}

void processHex2() {

///
	::std::cout << "HEX:   ";
	int p0 = hex2long();
	if(p0 >= 0) {
		::std::cout << ::std::endl;
		::std::cout << "SHORT: " << p0 << ::std::endl;
	}	
///
	
	unsigned char a0;
	unsigned char a1;
	// char2hex(Num,a0,a1);
	char2hex(p0,a0,a1);
	//
	::std::cout << "HEX : ";
	// 
	::std::cout << a1;
	// 
	::std::cout << a0;

	FILE *fp;
	// 4 bytes
	int i = 65537;
	if((fp = fopen(config.f_name,"wb")) == NULL) {
		::std::cout << "Can not open file" << ::std::endl;
		exit(1);
	}
	fseek(fp,config.offset,SEEK_SET);
	fwrite(&p0,sizeof(int),1,fp);
	fclose(fp);
}

void Test0() {	// structure allign
	// ---  0
	::std::cout << "sizeof(config1.n_size)"	<< sizeof(config1.n_size)	<< ::std::endl;
	::std::cout << "sizeof(config1.f_name)"	<< sizeof(config1.f_name)	<< ::std::endl;
	// allign by 4*word border

	::std::cout << "sizeof(config1)" << sizeof(config1) << ::std::endl;

	config1.n_size = config1.four;
	config1.f_name = 'A';
	write5();
}

void Test1() {  // write int and long
	// --- 1
	write1();
	write2();
}


void Test2() {  // hex 2 int 
	// --- 2
	::std::cout << "hex: ";
	// aa = 170 !!
	int i = hex2long();
	if( i >= 0) {
		::std::cout << ::std::endl; 
		::std::cout << "int: " << i << ::std::endl; 
	}
}

void Test3() {  // 
	// --- 3 (prove 2)
	// 
	int i = 170;
	// 
	write3(i);
}

void Test4() {  // 
	// --- 4
	// hex2short();
	// hex2int();
	// hex2ulong();
}

void Test5() {  // 
	// --- 4
	// hex2short();
	// hex2int();
	// hex2ulong();


	// --- 5 (prove 4)
	// --- 4294967295 FF FF FF FF ?  !!!
	// int i = 4294967295;
	// write4(i);

	// --- INT -> HEX
	// write3(122);
	// processHex(122);
	// return 0;
	
/* !!!
	::std::cout << "INT : ";
	unsigned int i2;
	::std::cin >> i2;
	unsigned char i1 = i2;

	write3(i1);
	processHex(i1);
	return 0;
*/

//
/*
	readConfig();
	getHex();
	return 0;
//
*/

	// char2hex8(1);			// 00000001
	// char2hex8(255);			// 000000FF
	// char2hex8(256);			// 00000100
	// char2hex8(268435457);	// 10000001
	// return 0;
}

#define kMaxTests 2
int nTest;
void Tests(::std::string param) {
	
	if(atoi(param.c_str()) > kMaxTests)
		nTest = kMaxTests - 1;
	else
		nTest = atoi(param.c_str());
	
	if(nTest == -1) {
		::std::cout << "=== NB IMA UTILITY TESTS" << ::std::endl;
		::std::cout << "----------------------------------------------------------" << ::std::endl;
		::std::cout << "structure allign            0                             " << ::std::endl;
		::std::cout << "write int and long          1                             "	<< ::std::endl;
		::std::cout << "hex 2 int                   2                             "	<< ::std::endl;
		::std::cout << "----------------------------------------------------------" << ::std::endl;

	} 
	else if(nTest == 0)
		Test0();
	else if(nTest == 1)
		Test1();
	else if(nTest == 2)
		Test2();
	else
		Test0();

}
