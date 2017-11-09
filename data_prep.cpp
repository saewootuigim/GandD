//===========================================================================80
// This code is to read the RC Test data
// and manipulate it into a readable format
// of known machine learning algorithm.
// I chose the comma separated values (csv) format
// by having pandas in mind.
//
// author: Seungbum Koo
// Date: Tue 11/07/2017 initial release
//===========================================================================80

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<cstdlib>

typedef std::vector< std::vector<std::string> > Rows;

int main(int argc, char *argv[] )
{

   std::ifstream file_to_read(argv[1],std::ifstream::in);
   Rows rows;
   unsigned int i, j, k;

   if( argc<2 )
   {
      std::cerr << "Usage: a.out [filename]" << std::endl;
      exit(1);
   }

   // Check if the file exists.
   if( !file_to_read )
   {
      std::cout << argv[1] << " does not exist." << std::endl;
      std::cerr << "File open failed." << std::endl;
      exit(2);
   }
   else
   {
      std::cout << "Reading " << argv[1] << std::endl;
   }

   // Read RC Test.txt file.
   for( std::string row; getline(file_to_read,row,'\n'); )
   {
      rows.push_back(Rows::value_type());
      std::istringstream ss(row);
      for( std::string field; getline(ss,field,'\t'); )
      {
         rows.back().push_back(field);
      }
   }
   file_to_read.close();
// // FOR DEBUGGING: PRINT ALL INFORMATION
   // for( i=0; i<rows.size(); i++ )
   // {
   //    for( j=0; j<rows[i].size(); j++ )
   //       std::cout<<rows[i][j]<<'\t';
   //    std::cout<<std::endl;
   // }
// // FOR DEBUGGING: PRINT HEADER
   // for( i=0; i<rows[43].size(); i++ )
   //    std::cout<< rows[43][i] << std::endl;
   // exit(1);

   // Search the position in file where the confining stress is written.
   // std::cout << rows.size() << std::endl;
   std::cout<<"There are total "<<rows.size()<<" rows in this file."<<std::endl;

   i = 0; j = 0;
   while( i<rows.size() )
   {
      // std::cout << rows[i].size() << std::endl;
      j=0;
      while( j<rows[i].size() )
      {
         // std::cout << rows[i][j].size() << std::endl;
         // When found
         if( !rows[i][j].compare("Loading") )
         {
            // Erase up to '='
            rows[i][j+1].erase(0,rows[i][j+1].find_first_of('=')+1);
            // Erase leading spaces
            rows[i][j+1].erase(0,std::min(rows[i][j+1].find_first_not_of(' '),rows[i][j+1].size()-1));
            // Erase after the number
            rows[i][j+1].erase(rows[i][j+1].find_last_of(' '),rows[i][j+1].size());
            // When found, open a file.
            std::string filename("Pc");
            filename.append(rows[i][j+1]);
            filename.append("psi.csv");
            std::ofstream file_to_write(filename.data(),std::ofstream::out);
            std::cout << "Writing file " << filename << std::endl;
            // Move on to the next line.
            i+=2; // \n
            j=0; // \r
            // Write header.
            for( j=0; j<rows[i].size(); j++ )
            {
               // Erase commas in the string.
               while( true )
               {
                  k = rows[i][j].find_first_of(',');
                  if( k>=rows[i][j].size() ) break;
                  rows[i][j].erase(k,1);
               }
               // Erase whitespaces in the string.
               while( true )
               {
                  k = rows[i][j].find_first_of(" ");
                  if( k>=rows[i][j].size() ) break;
                  // std::cout<< "k=" << k << std::endl;
                  // std::cout << "str["<<k<<"]="<<rows[i][j][k]<<std::endl;
                  rows[i][j].erase(k,1);
                  // std::cout << rows[i][j] << std::endl;
               }
               // Replace symbol '%' by word 'percent'.
               while( true )
               {
                  k = rows[i][j].find_first_of('%');
                  if( k>=rows[i][j].size() ) break;
                  rows[i][j].replace(k,1,"percent");
               }
               // Replace symbol '()' by '[]'.
               while( true )
               {
                  k = rows[i][j].find_first_of('(');
                  if( k>=rows[i][j].size() ) break;
                  rows[i][j].replace(k,1,"[");
               }
               while( true )
               {
                  k = rows[i][j].find_first_of(')');
                  if( k>=rows[i][j].size() ) break;
                  rows[i][j].replace(k,1,"]");
               }
               // Write.
               // std::cout<<rows[i][j];
               file_to_write << rows[i][j];
               if( j!=rows[i].size()-1 )
               {
                  file_to_write << ',';
                  // std::cout<<',';
               }
            }
            // std::cout<<std::endl;
            file_to_write << std::endl;
            // Write contents.
            i+=3;
            j=0;
            // std::cout<<rows[i].size()<<std::endl;
            // Write rows in file.
            // Tabbed space in file is not read by vector.
            // So if the size of rows[i][j]==0, that means that line is empty.
            while( rows[i][j].size()>0 ) // If that is the case, skip.
            {
               // Write columns in file.
               while( j<rows[i].size() )
               {
                  // std::cout<<rows[i][j];
                  file_to_write << rows[i][j++];
                  if( j!=rows[i].size() )
                  {
                     file_to_write << ',';
                     // std::cout<<',';
                  }
               }
               // std::cout<<std::endl;
               file_to_write << std::endl;
               // Move to the next line.
               // If we reached the end of file, movig to the next line will cause SEGV.
               // So we should escape from the loop.
               if( i+1>=rows.size() ) break;
               i++;
               j=0;
            }
            file_to_write.close();
            std::cout<<"File close status "<<file_to_write<<std::endl;
         }
         // std::cout<<i<<','<<j<<','<<rows.size()<<','<<rows[rows.size()-1].size()<<std::endl;
         j++;
      }
      // Move to the next line.
      // If we reached the end of file, movig to the next line will cause SEGV.
      // So we should escape from the loop.
      if( i+1>=rows.size() ) break;
      i++;
   }

   return 0;
}
