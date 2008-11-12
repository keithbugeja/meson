// Common headers
#include "Directory.h"
#include "MesonException.h"
#include "Converter.h"

// Gravitas headers
#include "GrvGravitasEngine.h"

// Local headers
#include "GravitasSandboxApplication.h"


using namespace Meson::Common;
using namespace Meson::Common::IO;
using namespace Meson::Common::Text;

int main(int p_nArgumentCount, char* p_szArguments[])
{
	// get and process command-line parameters
	bool bFullscreen = false;
	bool bInstrumentationDriver = false;
	uint unWidth = 1024, unHeight = 768;
	String strSimulationFilename;
	for (size_t unIndex = 0; unIndex < (size_t) p_nArgumentCount; unIndex++)
	{
		String strParameter = p_szArguments[unIndex];
		if (strParameter == "-f")
			bFullscreen = true;
		else if (strParameter == "-i")
			bInstrumentationDriver = true;
		else if (strParameter == "-w")
		{
			if (unIndex < (size_t) (p_nArgumentCount - 1))
			{
				strParameter = p_szArguments[++unIndex];
				unWidth = Converter::ToShort(strParameter);
			}
		}
		else if (strParameter == "-h")
		{
			if (unIndex < (size_t) (p_nArgumentCount - 1))
			{
				strParameter = p_szArguments[++unIndex];
				unHeight = Converter::ToShort(strParameter);
			}
		}
		else if (strParameter == "-s")
		{
			if (unIndex < (size_t) (p_nArgumentCount - 1))
			{
				strSimulationFilename = p_szArguments[++unIndex];
			}
		}
	}

	// create and run application
	GravitasSandboxApplication application(unWidth, unHeight, bFullscreen, bInstrumentationDriver);

	if (strSimulationFilename.Length() > 0)
		application.SetSimulationFilename(strSimulationFilename);

	application.ApplicationPath = Directory::GetCurrent().GetPath() + Directory::Delimeter + "..\\..\\Test";
	application.MaterialCatalogue = "default.ms";
	application.BackgroundColour.Set(0,0,0,0);

		application.Run();

	/*
	try {
		application.Run();
	} 
	catch (Meson::Common::MesonException* e)
	{
		Meson::Gravitas::GravitasEngine::GetInstance()->Logger().Out << e->GetMessageTrace();
	}
	*/
}