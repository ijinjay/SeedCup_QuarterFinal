/****************************************************
 * Author: Liu Benxi
 * Created On: 2014-11-3
 * Last-Modified: 2014-11-3
 */

#ifndef _IO_H
#define _IO_H 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

#define GETDIRSCMD "DIR /AD /D /B >>Dirs.lis"
//#define GETHTMLCMD "DIR /B *.html >> html.lis"
//#define GETCSSSCMD "DIR /B *.css >> css.lis"
#define DIRSFILE   "Dirs.lis"
#define MAXDATA 8096
//#define HTMLFILE   "html.lis"
//#define CSSSFILE   "css.lis"
int readHtmlFiles();
int readCssFile(char* data, const char* fileName);
int writeFile(char* data, const char* fileName);

#endif // _IO_H
