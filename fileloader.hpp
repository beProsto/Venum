#ifndef FILELOADER_HPP_INCLUDED
#define FILELOADER_HPP_INCLUDED

#include <iostream>
#include <string>
#include <fstream>

namespace ven
{

/* Simple function for loading files to std::string's */
std::string LoadFromFile( const std::string& _filename )
{
    /* std::string that will be returned at the end */
    std::string finalized;
    /* std::fstream as a filestream for loading the file */
    std::fstream filestream;

    /* opening the file */
    filestream.open( _filename );
    /* checking if file exists */
    if( filestream.good(  ) )
    {
        /* std::string containing loaded line of a file */
        std::string line;
        /* loop while the readed line isn't at the end of the file */
        while( !filestream.eof(  ) )
        {
            /* reading the line */
            std::getline( filestream, line );
            /* adding the line to the string, that will be returned */
            finalized += line + '\n';
        }
    }
    else
    {
        /* if the file wasn't existing, output the error */
        std::cerr << "Cannot load a file : " << _filename << '\n';
        /* return nothing */
        return 0;
    }

    /* returning the finalized string */
    return finalized;
}


}
#endif // FILELOADER_HPP_INCLUDED
