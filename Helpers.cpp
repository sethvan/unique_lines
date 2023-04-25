#include "Helpers.h"
#include <algorithm>
#include <ranges>
#include <set>
#include <vector>
#include <sstream>
#include <QDir>
#include <QFileInfo>

std::ifstream open_file( const std::string& file_path ) {
   std::ifstream file_stream( file_path );
   if ( !file_stream.is_open() ) {
      throw std::runtime_error( "Could not open file: " + file_path );
   }
   return file_stream;
}

auto uniqueLines( std::istream& inStream ) {
   const char* spaces = " \t\n\v\f\r";

   std::string line;
   std::set<std::string> lSet;
   while ( std::getline( inStream, line ) ) {
      auto beginIndex = line.begin() + line.find_first_not_of( spaces );
      auto endIndex = line.begin() + line.find_last_not_of( spaces ) + 1;
      if ( ( endIndex - beginIndex ) > 1 ) {
         lSet.insert( { beginIndex, endIndex } );
      }
   }

   return lSet;
}

void writeMissingLinesToFile( const std::string& fileName1, const std::string& filename2,
                              const std::string& outputFileName, QString& uniques ) {
   auto inFile1 = open_file( fileName1 );
   auto inFile2 = open_file( filename2 );
   std::ostringstream os;

   std::vector<std::string> result;
   std::ofstream outFile( outputFileName );
   std::ranges::set_symmetric_difference( uniqueLines( inFile1 ), uniqueLines( inFile2 ),
                                          std::back_inserter( result ) );
   std::string str = std::string("This file prints the unique lines from comparing files \n\'") +
           fileName1 + "\'\n and \n\'" + filename2 + "\'\n\nUnique lines are printed below this line:\n";
   outFile << str;
   os << str;
   for ( auto const& s : result ) {
      outFile << s << '\n';
      os << s << '\n';
   }
   uniques = QString::fromStdString(std::move(os.str()));
   outFile.close();
}

QString getOutFilePath(QString path, const QString& file1, const QString& file2) {
    if (!path.endsWith(QDir::separator())) {
        path.append(QDir::separator());
    }
    QDir f1(file1);
    QDir f2(file2);

    path += "unique_lines_" + QFileInfo(f1.dirName()).baseName() + "_" + QFileInfo(f2.dirName()).baseName() + ".txt";

    return path;
}
