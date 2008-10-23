#include "DX9SandboxApplication.h"
#include "DX9PostProcessingApp.h"
#include "Directory.h"
#include "Converter.h"
#include "MesonException.h"

using namespace Meson::Common;
using namespace Meson::Common::IO;
using namespace Meson::Common::Maths;

int main(int p_nArgumentCount, char* p_szArguments[])
{
	bool bFullscreen = false,
		bInstrumentationDriver = false;

	uint uiWidth = 1024,
		uiHeight = 768;

	String strEnvironmentFilename("scene.vst");
	//String strEnvironmentFilename("grove.vst");

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
				uiWidth = Converter::ToShort(strParameter);
			}
		}
		else if (strParameter == "-h")
		{
			if (unIndex < (size_t) (p_nArgumentCount - 1))
			{
				strParameter = p_szArguments[++unIndex];
				uiHeight = Converter::ToShort(strParameter);
			}
		}
		else if (strParameter == "-e")
		{
			if (unIndex < (size_t) (p_nArgumentCount - 1))
			{
				strEnvironmentFilename = p_szArguments[++unIndex];
			}
		}
	}

	// create and run application
	DX9SandboxApplication application("Meson: Vistas", uiWidth, uiHeight, bFullscreen, bInstrumentationDriver);
	//DX9PostProcessingApp application("Meson: Vistas", uiWidth, uiHeight, bFullscreen, bInstrumentationDriver);

	application.ApplicationPath = Directory::GetCurrent().GetPath() + Directory::Delimeter + "..\\..\\Test";
	//application.MaterialCatalogue = "default.ms";
	application.MaterialCatalogue = "sandbox.ms";
	//application.MaterialCatalogue = "postprocess.ms";
	application.BackgroundColour.Set(1,1,1,1);

	if (strEnvironmentFilename.Length() > 0)
		application.EnvironmentFilename = strEnvironmentFilename;

	try {
		application.Run();
	} 
	catch (Meson::Common::MesonException* e) {
		MESON_ASSERT(false, e->GetMessageTrace());
	}
}
