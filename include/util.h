#pragma once
#include "common.h"
#include <cstdint>
#include <stdexcept>
#include <vector>
#include <filesystem>
#include <fstream>
#include <chrono>
#include <thread>

/*##########################

   Inline & template

##########################*/
inline
byte		read1(const byte*& begin, const byte* end)
{
	if (begin + 1 > end)
		throw std::out_of_range(__func__);
	return *begin++;
}
//------------------------------------------------------------------------------
inline
uint16_t	read2(const byte*& begin, const byte* end)
{
	if (begin + 1 > end)
		throw std::out_of_range(__func__);
	uint16_t result = 
		(static_cast<uint16_t>(begin[0]) << 8) |
		static_cast<uint16_t>(begin[1]);
	begin += 2;
	return result;
}
//------------------------------------------------------------------------------
inline
uint32_t	read4(const byte*& begin, const byte* end)
{
	if (begin + 1 > end)
		throw std::out_of_range(__func__);
	uint32_t result = 
		(static_cast<uint32_t>(begin[0]) << 24) |
		(static_cast<uint32_t>(begin[1]) << 16) |
		(static_cast<uint32_t>(begin[2]) << 8) |
		static_cast<uint32_t>(begin[3]);
	begin += 4;
	return result;
}
//------------------------------------------------------------------------------
inline
uint64_t	read8(const byte*& begin, const byte* end)
{
	if (begin + 1 > end)
		throw std::out_of_range(__func__);
	uint64_t result = 
		(static_cast<uint64_t>(begin[0]) << 56) |
		(static_cast<uint64_t>(begin[1]) << 48) |
		(static_cast<uint64_t>(begin[2]) << 40) |
		(static_cast<uint64_t>(begin[3]) << 32) |
		(static_cast<uint64_t>(begin[4]) << 24) |
		(static_cast<uint64_t>(begin[5]) << 16) |
		(static_cast<uint64_t>(begin[6]) << 8) |
		static_cast<uint64_t>(begin[7]);
	begin += 8;
	return result;
}
//------------------------------------------------------------------------------
inline
uint64_t	read_variable(const byte*& begin, const byte* end)
{
	uint64_t result = 0;
	do
	{
		if (begin + 1 > end)
			throw std::out_of_range(__func__);
		result <<= 7;
		result |= (*begin & 0x7f);
	}
	while (*begin++ & 0x80);
	return result;
}
// //------------------------------------------------------------------------------
// template <typename T> inline
// T			clamp(T val, T edge0, T edge1)
// {
// 	return std::min(std::max(val, edge0), edge1);
// }
//------------------------------------------------------------------------------
// inline
// void		sleep(std::chrono::system_clock::duration duration)
// {
// 	std::this_thread
// }


/*##########################

   Headers

##########################*/
std::vector<byte>	read_bin_file(const std::filesystem::path& file_path);
std::string			read_file(const std::filesystem::path& file_path);
void							write_bin_file(
	const std::filesystem::path&	file_path, 
	const std::vector<byte>&		data
);
void							write_file(
	const std::filesystem::path&	file_path, 
	const std::string&				data
);
// std::string						hex_dump(
// 	const byte*						begin,
// 	const byte*						end, 
// 	const std::vector<int>&			split = {2, 8},
// 	const std::vector<int>&			line = {64, 1024}
// );
// std::string						hex_dump(
// 	const std::vector<byte>&		input,
// 	const std::vector<int>&			split = {2, 8},
// 	const std::vector<int>&			line = {64, 1024}
// );
std::string						hex_dump(
	const byte*						begin,
	const byte*						end, 
	bool							force_hex = false,
	const std::vector<int>&			split = {2, 8},
	const std::vector<int>&			line = {64, 1024}
);
std::string						hex_dump(
	const std::vector<byte>&		input,
	bool							force_hex = false,
	const std::vector<int>&			split = {2, 8},
	const std::vector<int>&			line = {64, 1024}
);