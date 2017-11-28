#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <boost/filesystem.hpp>

#include "merge_files.h"
#include "feature_count.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::array;
using namespace boost::filesystem;

int main(int argc, char* argv[])
{
	path p;

	if (argc < 2 )
	{
		cout << "Current path is used." << endl;
		p = current_path();
		cout << "path = " << p << endl;
	}
	else
		p = argv[1];

	// Delete existing output file: "./BIG_RC_Test.csv"
	path output_file("./BIG_RC_Test.csv");
	remove(output_file);

	// Search inside the directory tree and look for "RC Test.txt" files.
	// If found, open it and read them and write the contents into
	// a big gigantic RC Test file.
	//
	// This work is done in two steps.
	// Since not all files are sharing the same feature columns,
	// we have to first loop over all files and see what features are there.
	// Once the count is done, we open each file and write down the data on the big file
	// in accordance to the features we collected from the first step.

	// Step 1
	// Collect the features.
	recursive_directory_iterator d_end;
	
	vector<string> header_file_str;
	int header_file_n=0;

	vector<string> header_local_str;
	int header_local_n_global=0;
	
	int header_local_n_local[150];
	int ISLocalToGlobalFile[150][20]; // The size 150 is the expected number of files to read.
	int ISLocalToGlobalLocal[150][20]; // The size 150 is the expected number of files to read.
	
	unsigned int file_count=0;

	for( recursive_directory_iterator d_iter(p); d_iter!=d_end; d_iter++ )
	{
		if( is_regular_file(d_iter->status()) )
		{
			if( (d_iter->path().extension()).compare(".txt")==0 )
			{
				cout << "collecting headers... " << d_iter->path() << endl;
				
				// This arrays maps local feature location to global feature location.
				feature_count(
					d_iter->path(), // file name
					header_file_str,        // file header feature names
					header_file_n,            // how many?
					header_local_str,       // local header feature names
					header_local_n_global,           // how many?
					header_local_n_local[file_count],
					ISLocalToGlobalFile[file_count],
					ISLocalToGlobalLocal[file_count]
				);

				file_count++;
			}
		}
	}

	// Step 2
	// Read each file and append on a big file.
	file_count = 0;
	for( recursive_directory_iterator d_iter(p); d_iter!=d_end; d_iter++ )
	{
		if( is_regular_file(d_iter->status()) )
		{
			if( (d_iter->path().extension()).compare(".txt")==0 )
			{
				cout << "merging files... " << d_iter->path() << endl;
				
				merge_files( d_iter->path(),
					header_file_str,
					header_file_n,
					header_local_str,
					header_local_n_global,
					header_local_n_local[file_count],
					ISLocalToGlobalFile[file_count],
					ISLocalToGlobalLocal[file_count]
				);

				file_count++;
			}
		}
	}
// for( int i=0; i<Nfeatures; i++ )
// 	cout << "[" << i << " ]" << features[i] << endl;
	cout << endl << "total " << file_count << " files." << endl;

	return 0;
}