/*
    Simple standalone command line tool to manage OOK protocols 
    supported by "pilight" project, PiCode library based.
    
    Copyright (c) 2021 Jorge Rivera. All right reserved.
    License GNU Lesser General Public License v3.0.
*/

#ifndef PICODER_ENCODE_H
#define PICODER_ENCODE_H

#include <cPiCode.h>
#include <stdio.h>

void encode_help(FILE* out);

int encode_cmd(int argc, char** argv);

#endif