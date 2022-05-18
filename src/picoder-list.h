/*
    Simple standalone command line tool to manage OOK protocols 
    supported by "pilight" project, PiCode library based.
    
    Copyright (c) 2021 Jorge Rivera. All right reserved.
    License GNU Lesser General Public License v3.0.
*/

#ifndef PICODER_LIST_H
#define PICODER_LIST_H

#include <cPiCode.h>
#include <stdio.h>

void list_help(FILE* out);

int list_cmd(int argc, char** argv);

#endif