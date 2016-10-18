// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>


// TODO: reference additional headers your program requires here
#include "Halide.h"
#include <iostream>

void createAOTE();
void createAOTD(char* path);
void createAOTO(char* path);
void createAOTC(char* path);
void createAOTT(char* path);
void createAOTB(char* path);
void createAOTG(char* path);