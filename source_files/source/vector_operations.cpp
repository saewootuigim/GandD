#include "vector_operations.h"

string standardize( string str )
{
	if(str.compare("beta")==0 )
		return string("approxbeta");
	return str;
}

bool check_whether_to_skip( string name )
{
	if( name.compare("Drive Plate Number")==0 ||
		name.compare("Top Cap")==0 ||
		name.compare("Conf Pressure (psi)")==0 ||
		name.compare("Time Stamp")==0 ||
		name.compare("Start Time")==0 )
		return true;
	return false;
}

bool check_inside_vector( vector<string> vec, string name, size_t *pos )
{
	for( size_t i=0; i<vec.size(); i++ )
	{
		if( vec[i].compare(name)==0 )
		{
			*pos = i;
			return true;
		}
	}
	*pos = -1;
	return false;
}