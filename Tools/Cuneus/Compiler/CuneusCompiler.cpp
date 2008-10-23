// CuneusCompiler.cpp : Defines the entry point for the console application.
//

#include <iostream>

#include <String8.h>
#include <FileInputStream.h>
#include <FileOutputStream.h>
#include <CnsCuneusCompiler.h>

using namespace Meson::Common;
using namespace Meson::Common::IO;
using namespace Meson::Cuneus::Compiler;


int main(int argc, char* argv[])
{
	FileInputStream  inStream( "Example6.cns" );
	FileOutputStream outStream( "Example6.cnx" );

	CuneusCompiler compiler;

	compiler.Compile( &inStream, &outStream );

	outStream.Close();
	inStream.Close();

	String str;

	compiler.GetSymbolTable()->ToString( str );

	std::cout << str << std::endl;

	std::cin.get();

	return 0;
}

