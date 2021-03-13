/*
    Simple standalone command line tool to manage OOK protocols 
    supported by "pilight" project, PiCode library based.
    
    Copyright (c) 2021 Jorge Rivera. All right reserved.
    License GNU Lesser General Public License v3.0.
*/

#include "picoder-encode.h"
#include <getopt.h>

#ifndef MAX_ENCODE_REPEATS
#define MAX_ENCODE_REPEATS     32
#endif

#ifndef MAX_ENCODE_PULSES
#define MAX_ENCODE_PULSES    255
#endif

static struct option list_options[] = {
  { "proto",      required_argument, NULL,      'p' },
  { "json",       required_argument, NULL,      'j' },
  { "full-json",  required_argument, NULL,      'f' },
  { "repeats",    required_argument, NULL,      'r' },
  { "train",      no_argument,       NULL,      't' },
  { "only-train", no_argument,       NULL,      'o' },
  { "help",       no_argument,       NULL,      'h' },
  { NULL, 0, NULL, 0 }
 };

void encode_help(FILE* out){
    fprintf(out,"         encode [-h] -p protocol -j json [-r repeats] --> encode from protocol and json data\n");
    fprintf(out,"                [-h | --help]                         --> show command options\n");
    fprintf(out,"                [-p | --proto protocol]               --> set protocol to encode\n");
    fprintf(out,"                [-j | --json json-data]               --> set json data to encode\n");
    fprintf(out,"                [-f | --full-json json]               --> set full json to encode\n");
    fprintf(out,"                [-r | --repeats repeats]              --> add repeats parameter from 1 to %d\n", MAX_ENCODE_REPEATS);
    fprintf(out,"                [-t | --train]                        --> show pulse train\n");
    fprintf(out,"                [-o | --only-train]                   --> show only pulse train\n");
}

int encode_cmd(int argc, char** argv){

    protocol_t* protocol                  = nullptr; 
    char*       json                      = nullptr; 
    char*       json_data                 = nullptr; 
    uint16_t    pulses[MAX_ENCODE_PULSES] = {0};
    char        repeats                   =  0 ;

    bool show_train      = false;
    bool show_only_train = false;

    int  error_flag = 0;
    bool help_flag  = false;
    int  ch         = 1;

    if (argc > 1){
        while ((ch = getopt_long(argc, argv, "p:j:f:r:hto", list_options, NULL)) != -1) {

            switch (ch) {
                case 'p':
                    if ( protocol == nullptr and json_data == nullptr ){
                        protocol = PiCode.findProtocol(optarg);
                        if (protocol == nullptr){
                            fprintf(stderr, "error: protocol '%s' invalid\n", optarg);
                            error_flag--;
                        }else{
                            if (protocol->createCode==nullptr){
                                fprintf(stderr, "error: protocol '%s' no encode support\n", optarg);
                                error_flag--;                                
                            }
                        }
                    }else{
                        fprintf(stderr,"error: only one protocol is allowed\n");
                        error_flag--;
                    }
                    break;
                case 'j':
                    if (json == nullptr and json_data == nullptr){
                        if (json_validate(optarg)){
                            json = optarg;
                        }else{
                            fprintf(stderr,"error: json '%s' invalid\n",optarg);
                            error_flag--;
                        }
                    }else{
                        fprintf(stderr,"error: only one json is allowed\n");
                        error_flag--;                        
                    }
                    break;
                case 'f':
                    if (json_data == nullptr and json == nullptr and protocol == nullptr){
                        if (json_validate(optarg)){

                            /* decode as root json */
                            JsonNode* root_json = json_decode(optarg);
                            if (root_json != nullptr){

                                /* check for child */
                                JsonNode* child_json = json_first_child(root_json);
                                if (child_json != nullptr){            
                                
                                    /* check for child key */
                                    if (child_json->key != nullptr){

                                        protocol = PiCode.findProtocol(child_json->key);

                                        if (protocol == nullptr){
                                            fprintf(stderr, "error: protocol '%s' invalid\n", child_json->key);
                                            error_flag--;
                                        }else{
                                            if (protocol->createCode==nullptr){
                                                fprintf(stderr, "error: protocol '%s' no encode support\n", child_json->key);
                                                error_flag--;                                
                                            }else{
                                                json_data  = json_encode(child_json);
                                                if (json_data != nullptr){
                                                    json = json_data;
                                                }else{
                                                    fprintf(stderr, "error: json data invalid\n");
                                                    error_flag--;   
                                                }
                                            }
                                        }
                                    }else{
                                        fprintf(stderr, "error: full json child no key\n");
                                        error_flag--;
                                    }
                                    json_delete(child_json);
                                }else{
                                    fprintf(stderr, "error: full json no child\n");
                                    error_flag--;
                                }
                                json_delete(root_json); 
                            }else{
                                fprintf(stderr, "error: full json decode fault\n");
                                error_flag--;     
                            }
                        }else{
                            fprintf(stderr,"error: full json '%s' invalid\n",optarg);
                            error_flag--;
                        }
                    }else{
                        fprintf(stderr,"error: only one full json is allowed\n");
                        error_flag--;                        
                    }
                    break;
                case 'r':
                    if (repeats == 0){
                        if ((atoi(optarg) > 0) and (atoi(optarg) < MAX_ENCODE_REPEATS )){
                            repeats = (char)atoi(optarg);
                        }else{
                            fprintf(stderr,"error: repeat must be > 0 and < %d \n",MAX_ENCODE_REPEATS );
                            repeats = -1;
                            error_flag--;                              
                        }
                    }else{
                        fprintf(stderr,"error: only one repeat param is allowed\n");
                        error_flag--;   
                    }
                    break;
                case 'h':
                    help_flag = true;
                    break;
                case 't':
                    show_train = true;
                    break;
                case 'o':
                    show_only_train = true;
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
            encode_help();
    
        }else{
 
            if (  protocol == nullptr and json == nullptr and json_data == nullptr ){
                fprintf(stderr,"error: -p protocol and -j json data or -f full json are required\n");
                error_flag--;
            }else{
                if ((protocol == nullptr or json == nullptr) and json_data == nullptr  ){
                    fprintf(stderr,"error: -p protocol and -j json data are required\n");
                    error_flag--;                    
                }
            }

            if (error_flag==0){ 

                int n_pulses = PiCode.encodeToPulseTrain(pulses, protocol, json);

                if (n_pulses >= 0 ){

                    if (show_train or show_only_train){
                        printf("pulses[%d]={",n_pulses);
                        for (int i = 0; i<n_pulses; i++){
                            printf("%d",pulses[i]);
                            if (i<n_pulses-1){
                                printf(",");
                            }else{
                                printf("};\n");
                            }
                        }
                    }
                    if (not show_only_train){
                        
                        char* picode_str = PiCode.pulseTrainToString(pulses,(unsigned int)n_pulses, (uint8_t)repeats);

                        if (picode_str != nullptr){
                            printf("%s\n",picode_str);
                            free(picode_str);
                        }else{
                            fprintf(stderr,"error: encoding pulse train");
                            error_flag--; 
                        }
                        
                    }
                }else{
                    fprintf(stderr,"error: unable to encode (%d)\n",n_pulses);
                    error_flag = n_pulses;
                }
            }
        }
    }else{
        fprintf(stderr,"error: -p protocol and -j json or -f full json are required\n");
        error_flag--;
    }
    if (json_data) free(json_data);
    return error_flag; 
}