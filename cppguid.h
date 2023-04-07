#pragma once

#include <guiddef.h>
#include <stdint.h>

constexpr uint8_t value_of_hex_digit(char c)
{
	uint8_t value;
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
		uint8_t d = value_of_hex_digit(ch); // one nibble
		value <<= 4;
		value |= d;
	}
	if (CheckEnding && s[WIDTH] != '-')
		throw "not a valid ending char";
	return value;
}

constexpr auto read_u32 = read_xx<uint32_t>;
constexpr auto read_u16 = read_xx<uint16_t>;
constexpr auto read_byte = read_xx<uint8_t, false>;

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

	GUID g = { 0 };

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
