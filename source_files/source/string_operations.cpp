#include "string_operations.h"

std::string erase_trailing_whitespaces( std::string str )
{
	std::string whitespaces (" \t\f\v\n\r");
	std::size_t found_whitespace;

	found_whitespace = str.find_last_not_of(whitespaces);
	if( found_whitespace!=std::string::npos )
		str.erase(found_whitespace+1);
	else
		str.clear();                    // str is all whitespace

	return str;
}

std::string replace_special_characters_by_( std::string str )
{
	// Scan the string and replace special characters.
	for( std::size_t i=0; i<str.length(); i++ )
	{
		if( str[i]=='^' || str[i]=='.' || str[i]==' ' || str[i]==',' || str[i]=='-' || str[i]=='"' )
		{
			str.erase(i,1);
			i--;
		}
		else if( str[i]=='#' || str[i]==':' || str[i]=='/' || str[i]=='(' || str[i]==')' )
			str.replace(i,1,"_");
		else if( str[i]=='%' )
			str.replace(i,1,"percent");
	}

	return str;
}