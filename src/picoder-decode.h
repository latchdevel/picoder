/*
    Simple standalone command line tool to manage OOK protocols 
    supported by "pilight" project, PiCode library based.
    
    Copyright (c) 2021 Jorge Rivera. All right reserved.
    License GNU Lesser General Public License v3.0.
*/

#ifndef PICODER_DECODE_H
#define PICODER_DECODE_H

#include <PiCode.h>

void decode_help(FILE* out = stdout);

int decode_cmd(int argc, char** argv);

#endif