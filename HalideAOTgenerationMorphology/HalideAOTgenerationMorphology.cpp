// HalideAOTgenerationMorphology.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
//#include "HalideRuntime.h"
//#include "Halide.h"
int main()
{




	char* path;
	std::cout << "1: erosion, 2: dilation, 3: opening, 4: closing, 5: tophat, 6: blackhat, 7: gradient, 8: runtime" << std::endl;
	int choice = -1;
	std::cin >> choice;
	switch (choice) {
	case 1:
		path = "erosion";
		createAOTE();
		break;
	case 2:
		path = "dilation";
		createAOTD(path);
		break;
	case 3:
		path = "opening";
		createAOTO(path);
		break;
	case 4:
		path = "closing";
		createAOTC(path);
		break;
	case 5:
		path = "tophat";
		createAOTT(path);
		break;
	case 6:
		path = "blackhat";
		createAOTB(path);
		break;
	case 7:
		path = "gradient";
		createAOTG(path);
		break;
	case 8:
		Halide::Target t(Halide::Target::Windows, Halide::Target::Arch::X86, 64);

		Halide::compile_standalone_runtime("HalideRuntime", t);
		break;
		/*path = "C:\\Users\\Admin\\Documents\\Visual Studio 2015\\Projects\\HalideMorphology2AOT\\HalideMorphology2AOT\\erosion";
		createAOTE(path);
		path = "C:\\Users\\Admin\\Documents\\Visual Studio 2015\\Projects\\HalideMorphology2AOT\\HalideMorphology2AOT\\dilation";
		createAOTD(path);
		path = "C:\\Users\\Admin\\Documents\\Visual Studio 2015\\Projects\\HalideMorphology2AOT\\HalideMorphology2AOT\\erosion";
		createAOTO(path);
		path = "C:\\Users\\Admin\\Documents\\Visual Studio 2015\\Projects\\HalideMorphology2AOT\\HalideMorphology2AOT\\closing";
		createAOTC(path);*/
	}
	
    return 0;
}

