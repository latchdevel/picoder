/*
    picoder.h - header

    Simple standalone command line tool to manage OOK protocols 
    supported by "pilight" project.
    
    Based on PiCode Library, a large number of wireless protocols 
    of RC switches and weather stations using the 433/315Mhz radio 
    frequency band via ASK/OOK pulse modulation are implemented by 
    the excellent work of the "pilight" community. 

    https://github.com/latchdevel/picoder
    
    Copyright (c) 2021 Jorge Rivera. All right reserved.
    License GNU Lesser General Public License v3.0.
*/

#ifndef PICODER_H
#define PICODER_H

#define MAYOR_VERSION 1
#define MINOR_VERSION 2

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#ifdef _MSC_VER 
#include <getsubopt.h>
#endif

#include <PiCode.h>
#include "picoder-list.h"
#include "picoder-show.h"
#include "picoder-encode.h"
#include "picoder-decode.h"
#include "picoder-convert.h"


#define STRINGIFY2(X) #X
#define STRINGIFY(X) STRINGIFY2(X)

#ifndef BUILD_VERSION
#define BUILD_VERSION no git info
#endif

#ifndef BUILD_COMPILER
#define BUILD_COMPILER 
#endif

#ifdef DEBUG
#define BUILD_TYPE "Debug"
#else
#define BUILD_TYPE "Release"
#endif

void show_version(){

    /* Get PiCode library version (new from v1.2) */
    char* picode_version = PiCode.getPiCodeVersion();

    printf("picoder v%d.%d (%s)\n",MAYOR_VERSION, MINOR_VERSION, STRINGIFY(BUILD_VERSION));
    printf("Compiled at " __DATE__ " " __TIME__ " %s (%s)\n",STRINGIFY(BUILD_COMPILER), BUILD_TYPE );
    printf("PiCode library version: %s\n", picode_version ? picode_version : "unknow");
    printf("Copyright (c) 2021 Jorge Rivera. All right reserved.\n");
    printf("See https://github.com/latchdevel/picoder\n\n");

    printf("License GNU Lesser General Public License v3.0.\n");
    printf("This is free software: you are free to change and redistribute it.\n");
    printf("There is NO WARRANTY, to the extent permitted by law.\n");

    if (picode_version!=nullptr){
        free(picode_version);
    }
}

#endif
