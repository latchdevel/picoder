/*
    Simple standalone command line tool to manage OOK protocols 
    supported by "pilight" project, PiCode library based.
    
    Copyright (c) 2021 Jorge Rivera. All right reserved.
    License GNU Lesser General Public License v3.0.
*/

#include "picoder-show.h"
#include <getopt.h>

extern const char* devtype[];

#ifndef MAX_ID
#define MAX_ID        4
#endif

#ifndef MAX_NAME
#define MAX_NAME     32
#endif

#ifndef MAX_MASK
#define MAX_MASK    255
#endif

static struct option list_options[] = {
  { "proto",      required_argument, NULL,      'p' },
  { "help",       no_argument,       NULL,      'h' },
  { NULL, 0, NULL, 0 }
 };

void show_help(FILE* out){
    fprintf(out,"         show [-h] -p protocol                        --> show protocol details\n");
    fprintf(out,"              [-h | --help]                           --> show command options\n");
    fprintf(out,"              [-p | --proto protocol]                 --> set protocol to show\n");

}

int show_cmd(int argc, char** argv){

    protocol_t*         protocol = nullptr; 
    protocol_devices_t* devices  = nullptr;

    int  error_flag = 0;
    bool help_flag  = false;
    int  ch         = 1;

    if (argc > 1){
        while ((ch = getopt_long(argc, argv, "p:h", list_options, NULL)) != -1) {

            switch (ch) {
                case 'p':
                    if ((error_flag == 0) and (protocol == nullptr) ){
                        protocol = PiCode.findProtocol(optarg);
                        if (protocol == nullptr){
                            fprintf(stderr, "error: protocol '%s' invalid\n", optarg);
                            error_flag--;
                        }
                    }else{
                        fprintf(stderr,"error: only one protocol is allowed\n");
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

        if (help_flag){
            printf("command:\n");
            show_help();
        }else{

            if (protocol == nullptr){
                fprintf(stderr,"error: -p protocol is required\n");
                error_flag--;
            }
            
            if (error_flag==0){ 

                printf("Protocol:    %s\n",protocol->id);   
                printf("Encode:      %s\n",protocol->createCode==nullptr ? "Unsupported":"Supported");   
                printf("Device type: %d (%s)\n",protocol->devtype,devtype[protocol->devtype]);  
                devices = protocol->devices;
                while (devices != nullptr){
                    printf("Devices:     %s\n",devices->desc);
                    if (devices->next != devices){
                        devices = devices->next;
                    }else{
                        devices = nullptr;
                    }
                } 

                printf("MinRawLen:   %5d uSecs\n",protocol->minrawlen);   
                printf("MaxRawLen:   %5d uSecs\n",protocol->maxrawlen);   
                printf("MinGapLen:   %5d uSecs\n",protocol->mingaplen);   
                printf("MaxGapLen:   %5d uSecs\n",protocol->maxgaplen); 

                int   option_index          =  0 ;
                char* option_id[MAX_ID]     = {0};
                char* option_name[MAX_NAME] = {0};
                int   option_argtype        =  0 ;
                int   option_conftype       =  0 ;
                char* option_mask[MAX_MASK] = {0};

                while (options_list(protocol->options, option_index, option_id)==0){
                    if (option_index==0){
                        printf("Option: (Id) Name          Arg Conf Regexp mask\n");
                    }
                    options_get_name_by_id(protocol->options, *option_id, option_name);
                    options_get_argtype(protocol->options, *option_id, 0, &option_argtype);
                    options_get_conftype(protocol->options, *option_id, 0, &option_conftype);
                    options_get_mask(protocol->options, *option_id, 0, option_mask);
                
                    printf("         (%s) %-12s  %2d  %2d   %s\n",*option_id,*option_name, option_argtype, option_conftype ,*option_mask==nullptr ? "":*option_mask);

                    option_index++;
                }
                
                if (protocol->createCode!=nullptr){
                    printf("pilight-send:\n");
                    protocol->printHelp();
                }
            }
        }

    }else{
        fprintf(stderr,"error: -p protocol is required\n");
        error_flag--;
    }

    return error_flag; 
}