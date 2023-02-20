//
// Copyright 2023, Raymond Yang.
//

#include <Windows.h>
#include <stdio.h>

#include <guiddef.h>
#include <initguid.h>
#include <combaseapi.h>


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

constexpr UINT8 value_of_hex_digit(char c)
{
	UINT8 value;
	if ('0' <= c && c <= '9') {
		value = c - '0';
	}
	else if ('a' <= c && c <= 'z') {
		value = c - 'a' + 10;
	}
	else if ('A' <= c && c <= 'Z') {
		value = c - 'A' + 10;
	}
	else {
		throw "not a hex digit";
	}
	return value;
}

template<typename T, bool CheckEnding = true, int WIDTH = sizeof(T) * 2 >
constexpr T read_xx(const char* s)
{
	T value = 0;
	for (int i = 0; i < WIDTH; ++i)
	{
		char ch = s[i];
		UINT8 d = value_of_hex_digit(ch); // one nibble
		value <<= 4;
		value |= d;
	}
	if (CheckEnding && s[WIDTH] != '-')
		throw "not a valid ending char";
	return value;
}

constexpr auto read_u32 = read_xx<UINT32>;
constexpr auto read_u16 = read_xx<UINT16>;
constexpr auto read_byte = read_xx<UINT8, false>;

consteval GUID operator "" _guid(const char* s, size_t length)
{
	//           1         2         3
	// 01234567890123456789012345678901234567
	// {FCC01D3B-BF5F-43C3-BF10-DF09FCE08107}
	//          ^    ^    ^    ^
	//          9    14   19   24

	if (length != 38 || s[0] != '{' || s[37] != '}') 
	{
		throw "invalid format";
	}

	GUID g = {0};

	g.Data1 = read_u32(s + 1);
	g.Data2 = read_u16(s + 10);
	g.Data3 = read_u16(s + 15);
	g.Data4[0] = read_byte(s + 20);
	g.Data4[1] = read_byte(s + 22);
	if (s[24] != '-')
		throw "invalid format at s[24]";
	int p = 25;
	for (int i = 2; i < 8; ++i)
	{
		g.Data4[i] = read_byte(s + p);
		p += 2;
	}
	return g;
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
