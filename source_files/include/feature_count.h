#ifndef _FEATURE_COUNT_H_
#define _FEATURE_COUNT_H_

#include <vector>
#include <boost/filesystem.hpp>

void feature_count(
	const boost::filesystem::path filename, // file name
	std::vector<std::string>& header_file_str,        // file header feature names
	int& header_file_n,            // how many?
	std::vector<std::string>& header_local_str,       // local header feature names
	int& header_local_n_global,           // how many?
	int& header_local_n_local,           // how many?
	int ISLocalToGlobalFile[],
	int ISLocalToGlobalLocal[]
);

#endif