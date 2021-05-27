/*
    picoder.cpp - main

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

#include "picoder.h"

enum {
    LIST = 0,
    SHOW,
    ENCODE,
    DECODE,
    CONVERT,
    VERSION,
    VERSION_v,
    VERSION__v,
    HELP,
    HELP_h,
    HELP__h
};

char *const cmds[] = {
    (char*) "list",
    (char*) "show",
    (char*) "encode",
    (char*) "decode",
    (char*) "convert",
    (char*) "version",  
    (char*) "-v",  
    (char*) "--version",  
    (char*) "help",
    (char*) "-h",
    (char*) "--help",
    NULL
};

int main(int argc, char** argv){

    int result = 0;

    if ( argc > 1){

        char*  cmd;
        int    n_args =  argc-1;
        char*  saved  =  argv[1];
        char** params = &argv[1];

        switch (getsubopt(params, cmds, &cmd)){
            case LIST:
              result = list_cmd(n_args,params);
              break;
            case SHOW:
              result = show_cmd(n_args,params);
              break;
            case ENCODE:
              result = encode_cmd(n_args,params);
              break;
            case DECODE:
              result = decode_cmd(n_args,params);
              break;
            case CONVERT:
              result = convert_cmd(n_args,params);
              break;
            case VERSION:
            case VERSION_v:
            case VERSION__v:
              show_version();
              break;
            case HELP:
            case HELP_h:
            case HELP__h:
              printf("usage: %s <command> [options]\n",argv[0]);
              printf("commands:\n");
              printf("         help | -h | --help                           --> show help\n");
              list_help();
              show_help();
              encode_help();
              decode_help();
              convert_help();
              printf("         version | -v | --version                     --> show version details\n");
              break;
            default:
              /* unknown command as suboption */
              fprintf(stderr,"error: unknown command: %s\n", saved);
              result--;
              break;
        }
    }else{
      // no arguments
      show_version();
      printf("\nusage: %s <command> [options]\n",argv[0]);
      printf("  try: \"%s -h\" for commands help\n",argv[0]);
    }

    return result;
}