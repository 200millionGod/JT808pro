#!/bin/bash
#user modify SOFT_VERSION
SOFT_VERSION=v1.0.0.6
#Specifies the path to the generated header file
CURDIR=$(cd `dirname $0`; pwd)
VERSION_PATCH=${CURDIR}/version.h

#The following automatically generated, please do not modify
BUILD_TIME=`env LC_ALL='C' date`
GIT_VERSION=$(git log -1 | grep 'commit' | sed 's/^.*commit //g')
AUTHOR=`whoami`
echo '/*The head file automatically generated, please do not modify*/' > ${VERSION_PATCH}
echo "#ifndef VERSION_H" >> ${VERSION_PATCH}
echo "#define VERSION_H" >> ${VERSION_PATCH}
echo "#include <stdio.h>" >> ${VERSION_PATCH}
echo "#include <stdlib.h>" >> ${VERSION_PATCH}
echo "#include <string.h>" >> ${VERSION_PATCH}
echo "" >> ${VERSION_PATCH}
echo "#define SOFT_VERSION \"SOF_VER: ${SOFT_VERSION}\"" >> ${VERSION_PATCH}
echo "#define GIT_VERSION \"GIT_VER: ${GIT_VERSION}\"" >> ${VERSION_PATCH}
echo "#define BUILD_TIME \"DATE   : ${BUILD_TIME}\"" >> ${VERSION_PATCH}
echo "#define BUILD_AUTHOR \"AUTHOR : ${AUTHOR}\"" >> ${VERSION_PATCH}
echo "" >> ${VERSION_PATCH}
echo '#define PRINT_VERSION(a, b)\' >> ${VERSION_PATCH}
echo 'printf("*****************************************************\n");\' >> ${VERSION_PATCH}
echo 'printf("* %-49s *\n",SOFT_VERSION);\' >> ${VERSION_PATCH}
echo 'printf("* %-49s *\n",GIT_VERSION);\' >> ${VERSION_PATCH}
echo 'printf("* %-49s *\n",BUILD_TIME);\' >> ${VERSION_PATCH}
echo 'printf("* %-49s *\n",BUILD_AUTHOR);\' >> ${VERSION_PATCH}
echo 'printf("*****************************************************\n");\' >> ${VERSION_PATCH}
echo 'if(a == 2)\' >> ${VERSION_PATCH}
echo '{\' >> ${VERSION_PATCH}
echo '	if(strcmp(b[1], "--version") == 0)\' >> ${VERSION_PATCH}
echo '	{\' >> ${VERSION_PATCH}
echo '		exit(1);\' >> ${VERSION_PATCH}
echo '	}\' >> ${VERSION_PATCH}
echo '}' >> ${VERSION_PATCH}
echo "#endif" >> ${VERSION_PATCH}
