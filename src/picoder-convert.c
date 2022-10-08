/*
    Simple standalone command line tool to manage OOK protocols 
    supported by "pilight" project, PiCode library based.
    
    Copyright (c) 2021 Jorge Rivera. All right reserved.
    License GNU Lesser General Public License v3.0.
*/

#include "picoder-convert.h"
#include <getopt.h>

typedef size_t rsize_t;
#include <string.h>
#include <stdlib.h>

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

void convert_help(FILE* out){
    fprintf(out,"         convert [-h] [ -s string | -t train ]        --> coverts from/to pilight string to/from pulse train\n");
    fprintf(out,"                 [-h | --help]                        --> show command options\n");
    fprintf(out,"                 [-s | --string piligth-string]       --> pilight string to convert\n");
    fprintf(out,"                 [-t | --train pulse-train]           --> pulse train to convert\n");
}

int convert_cmd(int argc, char** argv){

    char*     pi_string         = NULL;
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
                        }else{
                            pi_string = optarg;
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
            convert_help(stdout);
        }else{

            if (error_flag == 0) {

                if (n_pulses > 0){
                    if (pi_string == NULL){
                        // Provide pulse train to convert to pilight string
                        pi_string = pulseTrainToString(pulses,(uint16_t)n_pulses,0);
                        if (pi_string != NULL){
                            printf("%s\n",pi_string);
                            free(pi_string);
                        }else{
                            fprintf(stderr,"error: unable to encode pulse train\n");
                            error_flag--;    
                        }
                    }else{
                        // Provide pilight string to convert to pulse train
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