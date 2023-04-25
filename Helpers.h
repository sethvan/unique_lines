#ifndef HELPERS_H
#define HELPERS_H

#include <fstream>
#include <string>
#include <QString>


std::ifstream open_file( const std::string& file_path );

auto uniqueLines( std::istream& inStream );

void writeMissingLinesToFile( const std::string& fileName1, const std::string& filename2,
                              const std::string& outputFileName, QString& uniques );

QString getOutFilePath(QString path, const QString& file1, const QString& file2);


#endif // HELPERS_H
