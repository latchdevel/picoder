/*
    Simple standalone command line tool to manage OOK protocols 
    supported by "pilight" project, PiCode library based.
    
    Copyright (c) 2021 Jorge Rivera. All right reserved.
    License GNU Lesser General Public License v3.0.
*/

#include "picoder-decode.h"
#include <getopt.h>

typedef size_t rsize_t;
#include <string.h>

#ifndef MAX_PULSES
#define MAX_PULSES    255
#endif

#ifndef MAX_PULSE_LENGTH
#define MAX_PULSE_LENGTH    100000UL
#endif

static struct option list_options[] = {
  { "string",     required_argument, NULL,      's' },
  { "train",      required_argument, NULL,      't' },
  { "help",       no_argument,       NULL,      'h' },
  { NULL, 0, NULL, 0 }
 };

void decode_help(FILE* out){
    fprintf(out,"         decode [-h] [ -s string | -t train ]         --> decode pilight string or pulse train\n");
    fprintf(out,"                [-h | --help]                         --> show command options\n");
    fprintf(out,"                [-s | --string piligth-string]        --> pilight string to decode\n");
    fprintf(out,"                [-t | --train pulse-train]            --> pulse train to decode\n");
}

int decode_cmd(int argc, char** argv){

    uint32_t  pulses[MAX_PULSES] = {0};
    int       n_pulses           =  0;

    int  error_flag = 0;
    bool help_flag  = false;
    int  ch         = 1;

    if (argc > 1){
        while ((ch = getopt_long(argc, argv, "s:t:h", list_options, NULL)) != -1) {

            switch (ch) {
                case 's':
                    if (n_pulses == 0){
                        n_pulses = stringToPulseTrain(optarg, pulses, MAX_PULSES);
                        if (n_pulses <= 0){
                            fprintf(stderr,"error: string to pulse train (%d)\n",n_pulses);
                            error_flag--;  
                        }
                    }else{
                        fprintf(stderr,"error: only one pilight string is allowed\n");
                        error_flag--;                        
                    }
                    break;
                case 't':  
                    if (n_pulses == 0){

                        char* pulse;
                        pulse = strtok(optarg,",");

                        while (pulse != NULL){

                            if ((atol(pulse) > 0 ) && ((uint32_t)atol(pulse) <= MAX_PULSE_LENGTH)) { 
                                pulses[n_pulses++] = (uint32_t)atol(pulse);
                                if (n_pulses >= MAX_PULSES ){
                                    pulse = NULL;
                                    n_pulses = -1;
                                    fprintf(stderr,"error: too many pulses (max %d)\n",MAX_PULSES);
                                    error_flag--;
                                }else{
                                    pulse = strtok (NULL, ",");
                                }
                            }else{
                                pulse = NULL;
                                n_pulses = -1;
                                fprintf(stderr,"error: pulses must be > 0 and <= %lu\n",MAX_PULSE_LENGTH);
                                error_flag--;
                            }
                        }
                    }else{
                        fprintf(stderr,"error: only one pulse train is allowed\n");
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
            decode_help(stdout);
        }else{

            if (error_flag == 0) {

                if (n_pulses > 0){

                    char *json = decodePulseTrain(pulses, (uint8_t)n_pulses, "  ");

                    if (json != NULL){
                        if (strlen(json) > 4){  
                            printf("%s\n",json);
                        }else{
                            // JSON emply '[]'
                            fprintf(stderr,"error: unable to decode pulse train\n"); 
                            error_flag--;                             
                        }
                        free(json);
                    }else{
                        fprintf(stderr,"error: decode pulse train fails\n");
                        error_flag--; 
                    }

                }else{
                    fprintf(stderr,"error: invalid pulse train (%d)\n",n_pulses);
                    error_flag--;    
                }
            }
        }
    }else{
        fprintf(stderr,"error: -s pilight-string or -t pulse-train are required\n");
        error_flag--;
    }

    return error_flag; 
}