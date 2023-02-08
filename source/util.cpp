#include "util.h"
#include <sstream>

std::vector<byte>	read_bin_file(const std::filesystem::path &file_path)
{
	std::ifstream ifs(file_path, std::ios::binary);
	if (!ifs.is_open())
		throw std::runtime_error("Error: open file");
	ifs.seekg(0, std::ios::end);
	int64_t length = ifs.tellg();
	ifs.seekg(0, std::ios::beg);
	std::vector<byte> result(length);
	ifs.read(reinterpret_cast<char *>(result.data()), length);
	ifs.close();
	return result;
}
//------------------------------------------------------------------------------
std::string			read_file(const std::filesystem::path& file_path)
{
	std::ifstream ifs(file_path);
	if (!ifs.is_open())
		throw std::runtime_error("Error: open file");
	std::stringstream ss;
	ss << ifs.rdbuf();
	ifs.close();
	return ss.str();
}
//------------------------------------------------------------------------------
void				write_bin_file(
	const std::filesystem::path&	file_path, 
	const std::vector<byte>&		data)
{
	std::ofstream ofs(file_path, std::ios::binary);
	if (!ofs.is_open())
		throw std::runtime_error("Error: open file");
	ofs.write(reinterpret_cast<const char*>(data.data()), data.size());
	ofs.close();
}
//------------------------------------------------------------------------------
void				write_file(
	const std::filesystem::path&	file_path, 
	const std::string&				data
)
{
	std::ofstream ofs(file_path);
	if (!ofs.is_open())
		throw std::runtime_error("Error: open file");
	ofs << data;
	ofs.close();
}
//------------------------------------------------------------------------------
std::string						hex_dump(
	const uint8_t*					begin,
	const uint8_t*					end, 
	bool							force_hex,
	const std::vector<int>&			split,
	const std::vector<int>&			line
)
{
	std::vector<std::pair<int, std::string>> split_map;
	int n = 1;
	for (int i: split)
	{
		split_map.emplace_back(i, std::string(n++, ' '));
	}
	n = 1;
	std::vector<std::pair<int, std::string>> line_map;
	for (int i: line)
	{
		line_map.emplace_back(i, std::string(n++, '\n'));
	}

	std::stringstream s;
	int i = 0;
	while (begin != end)
	{
		if (std::isalpha(*begin) && !force_hex)
			s << std::setbase(16) << std::setw(2) << std::setfill(' ') 
			<< *begin++;
		else
			s << std::setbase(16) << std::setw(2) << std::setfill('0')
			<< static_cast<int>(*begin++);
		for (auto& j: split_map)
		{
			if ((i + 1) % j.first == 0)
				s << j.second;
		}
		for (auto& j: line_map)
		{
			if ((i + 1) % j.first == 0)
				s << j.second;
		}
		++i;
	}
	return s.str();
}
//------------------------------------------------------------------------------
std::string						hex_dump(
	const std::vector<uint8_t>&		input,
	bool							force_hex,
	const std::vector<int>&			split,
	const std::vector<int>&			line
)
{
	return hex_dump(
		input.data(), input.data() + input.size(), 
		force_hex, 
		split, 
		line
	);
}

