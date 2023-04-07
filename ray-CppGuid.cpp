//
// Copyright 2023, Raymond Yang.
//

#include <Windows.h>
#include <stdio.h>

#include <guiddef.h>
#include <initguid.h>
#include <combaseapi.h>

#include "cppguid.h"


// {FCC01D3B-BF5F-43C3-BF10-DF09FCE08107}
DEFINE_GUID(THE_GUID,
	0xfcc01d3b, 0xbf5f, 0x43c3, 0xbf, 0x10, 0xdf, 0x9, 0xfc, 0xe0, 0x81, 0x7);

constexpr GUID THE2_GUID = {
	 0xfcc01d3b,
	 0xbf5f,
	 0x43c3,
	 { 0xbf, 0x10, 0xdf, 0x9, 0xfc, 0xe0, 0x81, 0x7 } };

/*
typedef struct _GUID {
	unsigned long  Data1;
	unsigned short Data2;
	unsigned short Data3;
	unsigned char  Data4[ 8 ];
} GUID;

const GUID THE_GUID = {
     0xfcc01d3b, 
	 0xbf5f, 
	 0x43c3,
	 { 0xbf, 0x10, 0xdf, 0x9, 0xfc, 0xe0, 0x81, 0x7 } 
};

// {FCC01D3B-BF5F-43C3-BF10-DF09FCE08107}
{ 0xfcc01d3b, 0xbf5f, 0x43c3, { 0xbf, 0x10, 0xdf, 0x9, 0xfc, 0xe0, 0x81, 0x7 } }
    FCC01D3B  -BF5F   -43C3     -BF10      -DF 09 FC E0 81 07}
     Data1    Date2   Date3   data4[8] 

C# System.Guid
{int _a, short _b, short _c, byte _d, ... byte _k}

TryParseExactB

*/

constexpr bool is_hex_digit(char c)
{
	return ('0' <= c && c <= '9') || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}


const GUID foo = "{FCC01D3B-BF5F-43C3-BF10-DF09FCE08107}"_guid;

constexpr GUID foo2 = "{FCC01D3B-BF5F-43C3-BF10-DF09FCE08107}"_guid;


int main()
{
	WCHAR buf[40] = { 0 };

	ZeroMemory(buf, sizeof(buf));
	if (!StringFromGUID2(THE_GUID, buf, 40))
		throw "Buffer is too small?";

	wprintf(L"hello    %s\n", buf);


	ZeroMemory(buf, sizeof(buf));
	if (!StringFromGUID2(foo, buf, 40))
		throw "Buffer is too small?";

	wprintf(L"The foo  %s\n", buf);


 	ZeroMemory(buf, sizeof(buf));
	if (!StringFromGUID2(foo2, buf, 40))
		throw "Buffer is too small?";

	wprintf(L"The foo2 %s\n", buf);

}
