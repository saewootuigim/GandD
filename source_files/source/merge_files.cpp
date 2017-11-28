#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

#include "string_operations.h"
#include "vector_operations.h"
#include "merge_files.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::size_t;
using boost::trim;

void merge_files(
	const boost::filesystem::path filename,
	const vector<string> header_file_str,
	const size_t header_file_n,
	const vector<string> header_local_str,
	const size_t header_local_n_global,
	const size_t header_local_n_local,
	const int ISLocalToGlobalFile[],
	const int ISLocalToGlobalLocal[]
	)
{
	// Open files.
	boost::filesystem::ifstream fin( filename );
	boost::filesystem::ofstream fout("./BIG_RC_Test.csv",boost::filesystem::ofstream::app);
	// FILE* fout = fopen("./BIG_RC_Test.csv","at");

	// Write the header.
	static bool header_wriiten=false;
	if( !header_wriiten )
	{
		for( unsigned int i=0; i<header_file_n; i++ )
			// fprintf(fout,"%32s, ",header_file_str[i].c_str());
			fout << header_file_str[i] << ",";
		for( unsigned int i=0; i<header_local_n_global; i++ )
		{
			// fprintf(fout,"%23s",header_local_str[i].c_str());
			fout << header_local_str[i];
			if( i!=header_local_n_global-1 )
				// fprintf(fout,", ");
				fout << ",";
		}
		// fprintf(fout,"\n");
		fout << endl;
		header_wriiten = true;
	}

	// Initialize the array.
	double header_file_val[header_file_n];
	for( unsigned int i=0; i<header_file_n; i++ )
		header_file_val[i] = 0.;
	// double local_val_read[header_local_n_local];
	// for( unsigned int i=0; i<header_local_n_local; i++ )
	// 	local_val_read[i] = 0.;
	double local_val_write[header_local_n_global];
	for( unsigned int i=0; i<header_local_n_global; i++ )
		local_val_write[i] = 0.;

	// Read file header and write.
	string line;
	for( unsigned int k=0; k<5; k++ )
		getline(fin,line,'\n');

	// sample name
	string sample_name;
	getline(fin,sample_name,'\n');
	trim(sample_name);

	// and others
	size_t j = 1; // starting from 1 because we found the sample name from above.
	while( ISLocalToGlobalFile[j]!=-1 )
	{
		getline(fin,line,'\n');
		trim(line);
		size_t pos1 = line.find_first_of('=');
		if( pos1!=string::npos )
		{
			if( !check_whether_to_skip(line.substr(0,pos1-1)) )
			{
				size_t pos2 = line.find_last_of(' ');
				double value = std::stod(line.substr(pos1+1,pos2));
				int k = ISLocalToGlobalFile[j++];
				if( k>=0 )
					header_file_val[k] = value;
			}
		}
	}

	// Read and write until EOF.
	while( getline(fin,line,'\n') )
	{
		// Look for confining stress Pc
		trim(line);
		auto pos = line.find_first_of('=');
		if( pos!=string::npos )
		{
			line = line.substr(pos+1);
			line.erase(line.find("psi"));
			local_val_write[0] = std::stod(line);

			while( true )
			{
				getline(fin,line,'\n');
				boost::trim(line);
				if( line.find_first_of('-')==0 )
					break;
			}

			// This is where the first line is.
// cout << "number of local header features = " << header_local_n << endl;
// for( int k=0; ISLocalToGlobalLocal[k]!=-1; k++ )
// {
// 	cout << "[" << k << "][" << ISLocalToGlobalLocal[k] << "]";
// 	if( ISLocalToGlobalLocal[k]>=0 )
// 		cout << "[" << header_local_str[ISLocalToGlobalLocal[k]] << "]" << endl;
// }
			while( true )
			{
				getline(fin,line,'\n');
				boost::trim(line);

				if( line.size()==0 )
					break;

				std::istringstream ss(line);
				for( unsigned int k=1; k<header_local_n_local; k++ )
				{
					string name;
					getline(ss,name,'\t');

					// There are empty cells. Skip.
					if( name.size()==0 ) continue;

					// There are some redundant columns that I did not count in feature_count.cpp. Skip.
					if( ISLocalToGlobalLocal[k]==-1 )
						continue;

					local_val_write[ISLocalToGlobalLocal[k]] = std::stod(name);
					if( std::isnan(local_val_write[ISLocalToGlobalLocal[k]]) )
						local_val_write[ISLocalToGlobalLocal[k]] = 0.;
					else if( std::isinf(local_val_write[ISLocalToGlobalLocal[k]]) )
						local_val_write[ISLocalToGlobalLocal[k]] = 0.;
					// if( std::isnan(local_val_write[ISLocalToGlobalLocal[k]]) )
					// {
					// 	cout << line << endl;
					// 	cout << name << endl;
					// 	cout << k << endl;
					// 	cout << ISLocalToGlobalLocal[k] << endl;
					// 	exit(1);
					// }
				}
				
				// Write down on file.
				// fprintf(fout,"%32s, ",sample_name.c_str());
				fout << sample_name << ",";
				// cout << sample_name << ",";
				for( unsigned int k=1; k<header_file_n; k++ )
				{
					// fprintf(fout,"%32f, ",header_file_val[k]);
					fout << header_file_val[k] << ",";
					// cout << header_file_val[k] << ",";
				}
				for( unsigned int k=0; k<header_local_n_global; k++ )
				{
					// fprintf(fout,"%23f",local_val_write[k]);
					fout << local_val_write[k];
					// cout << local_val_read[k];
					if( k!=header_local_n_global-1 )
					{
						// fprintf(fout,", ");
						fout << ",";
						// cout << ",";
					}
				}
				// fprintf(fout,"\n");
				fout << endl;
				// cout << endl;
			}
		}
	}

	// cout << "   # columns = " <<column_count << endl;

	fin.close();
	// fclose(fout);
	fout.close();
}