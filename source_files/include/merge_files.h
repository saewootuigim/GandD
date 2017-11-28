#ifndef _MERGE_FILES_H_
#define _MERGE_FILES_H_

#include <vector>
#include <string>
#include <boost/filesystem.hpp>

using std::size_t;
using std::vector;
using std::string;

void merge_files(
	const boost::filesystem::path filename,
	const vector<string> header_file_str,
	const size_t header_file_n,
	const vector<string> header_local_str,
	const size_t header_local_n_global,
	const size_t header_local_n_local,
	const int ISLocalToGlobalFile[],
	const int ISLocalToGlobalLocal[]
	);

#endif