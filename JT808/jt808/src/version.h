/*The head file automatically generated, please do not modify*/
#ifndef VERSION_H
#define VERSION_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SOFT_VERSION "SOF_VER: v1.0.0.6"
#define GIT_VERSION "GIT_VER: 6af612a2c946fc81dfa5de52061b3332275419de"
#define BUILD_TIME "DATE   : Tue Oct 19 10:48:47 CST 2021"
#define BUILD_AUTHOR "AUTHOR : elvis"

#define PRINT_VERSION(a, b)\
printf("*****************************************************\n");\
printf("* %-49s *\n",SOFT_VERSION);\
printf("* %-49s *\n",GIT_VERSION);\
printf("* %-49s *\n",BUILD_TIME);\
printf("* %-49s *\n",BUILD_AUTHOR);\
printf("*****************************************************\n");\
if(a == 2)\
{\
	if(strcmp(b[1], "--version") == 0)\
	{\
		exit(1);\
	}\
}
#endif
