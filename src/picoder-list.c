/*
    Simple standalone command line tool to manage OOK protocols 
    supported by "pilight" project, PiCode library based.
    
    Copyright (c) 2021 Jorge Rivera. All right reserved.
    License GNU Lesser General Public License v3.0.
*/

#include "picoder-list.h"
#include <getopt.h>

typedef size_t rsize_t;
#include <string.h>

#define DEVTYPES  17

const char* devtype[DEVTYPES] = {  "raw",
                                   "switch",
                                   "dimmer",
                                   "weather",
                                   "realy",
                                   "screen",
                                   "contact",
                                   "pendingsw",
                                   "datetime",
                                   "xmbc",
                                   "lirc",
                                   "webcam",
                                   "motion",
                                   "dusk",
                                   "ping",
                                   "label",
                                   "alarm"};

static struct option list_options[] = {
  { "encode",     no_argument,       NULL,      'e' },
  { "device",     required_argument, NULL,      'd' },
  { "help",       no_argument,       NULL,      'h' },
  { NULL, 0, NULL, 0 }
 };

void list_help(FILE* out){
    fprintf(out,"         list [-h] [-e] [-d devtype] [-d devtype] ... --> list supported protocols and devices\n");
    fprintf(out,"              [-h | --help]                           --> show command options\n");          
    fprintf(out,"              [-e | --encode]                         --> list encode support only protocols\n");
    fprintf(out,"              [-d | --device devtype ]                --> list device type only protocols\n"); 
}

int list_cmd(int argc, char** argv){

    protocol_t*         listener = NULL; 
    protocol_devices_t* devices  = NULL;
    protocols_t*        pnode    = pilight_protocols;


    bool devtypeflags[DEVTYPES] = {false};
    bool devtypeflags_set       =  false;
    bool encode_only            =  false;
    bool help_flag              =  false;
    int error_flag              =  0;
    int found                   = -1;
    int ch                      =  1;

    if (argc > 1){
        while ((ch = getopt_long(argc, argv, "ed:h", list_options, NULL)) != -1) {
            switch (ch) {
                case 'e':
                    encode_only = true;
                    break;
                case 'd':
                    for (int i = 0; i<DEVTYPES && found == -1; i++){
                        if (strcasecmp(devtype[i],optarg)==0){
                            found = i;
                        }
                    }
                    if (found!=-1){
                        devtypeflags[found] = true;
                        devtypeflags_set = true;
                        found = -1;
                    }else{
                        fprintf(stderr, "error: device type '%s' invalid\n", optarg);
                        error_flag--;
                    }
                    break;
                case 'h':
                    help_flag = true;
                    break;
                case 1:
                    /*
                    * Use this case if getopt_long() should go through all
                    * arguments. If so, add a leading '-' character to optstring.
                    * Actual code, if any, goes here.
                    */
                    break;
                case ':':   /* missing option argument */
                    //fprintf(stderr, "error: option '-%c' requires an argument\n", optopt);
                    error_flag--;
                    break;
                case '?':
                default:    /* invalid option */
                    //fprintf(stderr, "error: option '-%c' is invalid\n", optopt);
                    error_flag--;
                    break;
            }
        }

        if (optind < argc) {
            fprintf(stderr,"error: invalid parameters (%d)", argc - optind );
            while (optind < argc){
                fprintf(stderr," %s", argv[optind++]);
                error_flag--;
            }
            fprintf(stderr,"\n");
        }
    }else{
        //printf("no params\n");
    }

    if (help_flag){
        printf("command:\n");
        list_help(stdout);
    }else{
        if (error_flag==0){
            printf("Encode Protocol             Type      Devices\n");
            printf("-----------------------------------------------------------------------------------\n");
            while (pnode != NULL) {
                listener = pnode->listener;
                if (( encode_only == false ) || (listener->createCode!=NULL) ){ 
                    if ((devtypeflags_set == false) || ( devtypeflags[listener->devtype])){
                        printf(" [%c]   %-20s %-10s",listener->createCode==NULL ? ' ':'*',listener->id, devtype[listener->devtype]);
                        devices = listener->devices;
                        if (devices != NULL){
                            printf("%s\n",devices->desc);
                            devices = devices->next;
                            while (devices != NULL){
                                printf("%38s%s\n"," ",devices->desc);
                                if (devices->next != devices){
                                    devices = devices->next;
                                }else{
                                    devices = NULL;
                                    printf("\n");
                                }
                            } 
                        }else{
                            printf("\n");
                        }
                    }
                } 
                pnode = pnode->next;
            }
        }
    } 
    return error_flag;
}
