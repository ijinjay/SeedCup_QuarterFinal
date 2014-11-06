/****************************************************
 * Author: Liu Benxi
 * Created On: 2014-11-3
 * Last-Modified: 2014-11-3
 */

#ifndef _IO_H
#define _IO_H 
#include <stdio.h>
#include <stdlib.h>
#include <direct.h>
#include <string.h>

#define GETDIRSCMD "DIR /AD /D /B >>Dirs.lis"
//#define GETHTMLCMD "DIR /B *.html >> html.lis"
//#define GETCSSSCMD "DIR /B *.css >> css.lis"
#define DIRSFILE   "Dirs.lis"
#define MAXDATA 8096
typedef struct directory {
    char dir[100];
    struct directory* next;
}directory;
extern directory* getDirs();
extern void freeDir(directory *dirlist);
extern int readHtml(char* buffer, const char* dir);
extern int readCss(char* buffer, const char* dir, const char* fileName);
int writeFile(char* data, const char* fileName);

#endif // _IO_H
