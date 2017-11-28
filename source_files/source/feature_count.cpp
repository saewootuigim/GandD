#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

#include "string_operations.h"
#include "vector_operations.h"
#include "feature_count.h"

using std::vector;
using std::string;
using std::cout;
using std::endl;
using std::istringstream;
using std::size_t;
using boost::trim;

void feature_count(
	const boost::filesystem::path filename, // file name
	vector<string>& header_file_str,        // file header feature names
	int& header_file_n,            // how many?
	vector<string>& header_local_str,       // local header feature names
	int& header_local_n_global,           // how many?
	int& header_local_n_local,
	int ISLocalToGlobalFile[],
	int ISLocalToGlobalLocal[]
)
{
	// Initialize;
	header_local_n_local = 0;
	for( int i=0; i<20; i++ )
	{
		ISLocalToGlobalFile[i] = -1;
		ISLocalToGlobalLocal[i] = -1;
	}

	// Open file.
	boost::filesystem::ifstream fin( filename );

	// Read until it meets ':'.
	string line;
	while( true )
	{
		getline(fin,line,'\n');
		if( line.find_first_of(':')!=string::npos )
			break;
	}

	// Add to header_file_str if this is the first function call.
	// Otherwise, keep move on.
	if( header_file_n==0 )
	{
		header_file_str.push_back("SampleName");
		header_file_n += 1;
	}
	ISLocalToGlobalFile[0] = 0;

	// Read until it meets ':'.
	// Start Time = 01/01/1979 00:00:00 AM
	while( true )
	{
		getline(fin,line,'\n');
		if( line.find_first_of(':')!=string::npos )
			break;
	}

/******************************************************************************
 Read the file header.
******************************************************************************/
	// Read until it meets ':'.
	// SAMPLE INFORMATION:
	while( true )
	{
		getline(fin,line,'\n');
		if( line.find_first_of(':')!=string::npos )
			break;
	}
	
	getline(fin,line,'\n');

	size_t i;
	size_t j = 1;

	while( true )
	{
		getline(fin,line,'\n');
		trim(line);
		if( line.size()==0 )
			break;
		size_t pos = line.find_first_of('=');
		if( pos==string::npos )
			continue;
	
		// feature name
		string name = line.substr(0,pos-1);
		trim(name);

		// There are two "string valued" information.
		// Let us skip those.
		if( check_whether_to_skip(name) )
			continue;

		name = replace_special_characters_by_(name);
		name = standardize(name);

		// Append unit to feature name.
		string value = line.substr(pos+1);
		trim(value);
		pos = value.find_last_of(' ');
		name.append("_");
		if( pos!=string::npos )
		{
			string unit = value.substr(pos);
			trim(unit);
			unit = replace_special_characters_by_(unit);
			name.append(unit);
			value.erase(pos);
		}

		// If the feature name is already in the features vec
		if( check_inside_vector(header_file_str,name,&i) )
		{
			// Mark its position in features vec on LocalFeatureValues.
			ISLocalToGlobalFile[j++] = i;
		}
		// If not,
		else
		{
			// add at the end of the features vec.
			ISLocalToGlobalFile[j++] = header_file_n;
			header_file_n++;
			header_file_str.push_back(name);
		}
	}

	// Read until it meets ':'.
	// TEST CELL INFORMATION:
	while( true )
	{
		getline(fin,line,'\n');
		if( line.find_first_of(':')!=string::npos )
			break;
	}

	while( true )
	{
		getline(fin,line,'\n');
		trim(line);
		if( line.size()==0 )
			break;
		size_t pos = line.find_first_of('=');
		if( pos==string::npos )
			continue;
	
		// feature name
		string name = line.substr(0,pos-1);
		trim(name);

		// There are two "string valued" information.
		// Let us skip those.
		if( check_whether_to_skip(name) )
			continue;

		name = replace_special_characters_by_(name);
		name = standardize(name);

		// Append unit to feature name.
		string value = line.substr(pos+1);
		trim(value);
		pos = value.find_last_of(' ');
		name.append("_");
		if( pos!=string::npos )
		{
			string unit = value.substr(pos);
			trim(unit);
			unit = replace_special_characters_by_(unit);
			name.append(unit);
			value.erase(pos);
		}

		// If the feature name is already in the features vec
		if( check_inside_vector(header_file_str,name,&i) )
		{
			// Mark its position in features vec on LocalFeatureValues.
			ISLocalToGlobalFile[j++] = i;
		}
		// If not,
		else
		{
			// add at the end of the features vec.
			ISLocalToGlobalFile[j++] = header_file_n;
			header_file_n++;
			header_file_str.push_back(name);
		}
	}

// FILE * fout = fopen("confirm.txt","wt");
// for( int m=0; m<20; m++ )
// {
// 	if( ISLocalToGlobalFile[m]>=0 )
// 		fprintf(fout,"%32i",ISLocalToGlobalFile[m]);
// }
// fprintf(fout,"\n");
// for( int m=0; m<20; m++ )
// {
// 	if( ISLocalToGlobalFile[m]>=0 )
// 		fprintf(fout,"%32s",header_file_str[m].c_str());
// }
// fprintf(fout,"\n");
// fclose(fout);
// exit(1);


/******************************************************************************
 Read the local header
******************************************************************************/
	// Read until it meets the confining stress.
	while( true )
	{
		getline(fin,line,'\n');
		trim(line);
		size_t pos = line.find_first_of('=');
		if( pos!=string::npos )
			break;
	}

	// Add to header_file_str if this is the first function call.
	// Otherwise, keep move on.
	if( header_local_n_global==0 )
	{
		header_local_str.push_back("Pc");
		header_local_n_global += 1;
	}
	header_local_n_local = 0;
	ISLocalToGlobalLocal[header_local_n_local++] = 0;

	// Read local header.
	getline(fin,line,'\n');
	getline(fin,line,'\n');
	trim(line);
	istringstream ss(line);

	string name;
	i = 0;
	while( getline(ss,name,'\t') )
	{	
		trim(name);

		// There is a column name called "Time Stamp" which is useless.
		// Let's not count that.
		if( check_whether_to_skip(name) )
		{
			header_local_n_local++;
			continue;
		}

		name = replace_special_characters_by_(name);

		// When found, see if the feature name is already in the vector
		if( check_inside_vector(header_local_str,name,&i) )
			ISLocalToGlobalLocal[header_local_n_local++] = i;
		// If there isn't, add to the vector and store the element number;
		else
		{
			ISLocalToGlobalLocal[header_local_n_local++] = header_local_n_global++;
			header_local_str.push_back(name);
		}
	}

	fin.close();
}