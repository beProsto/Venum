#ifndef FILELOADER_HPP_INCLUDED
#define FILELOADER_HPP_INCLUDED

#include <iostream>
#include <string>
#include <fstream>

namespace ven
{

std::string LoadFromFile( const std::string& _filename )
{
    std::string finalized;
    std::fstream filestream;

    filestream.open( _filename );
    if( filestream.good(  ) )
    {
        std::string line;
        while( !filestream.eof(  ) )
        {
            std::getline( filestream, line );
            finalized += line + '\n';
        }
    }
    else
    {
        std::cerr << "Cannot load a file : " << _filename << '\n';
        return 0;
    }

    return finalized;
}


}
#endif // FILELOADER_HPP_INCLUDED
