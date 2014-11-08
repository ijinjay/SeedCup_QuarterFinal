#include "IO.h"
directory* getDirs() {
    system(GETDIRSCMD);
    FILE *dirFp = fopen(DIRSFILE,"r+");
    char dirBuffer[1024];
    memset(dirBuffer, '\0', 1024);
    directory* head = malloc(sizeof(directory));
    directory* currentDir;
    currentDir = head;
    while(fgets(dirBuffer, 1024, dirFp)) {
        int len = strlen(dirBuffer);
        dirBuffer[len - 1] = '\0';
        directory *newDir = (directory*)malloc(sizeof(directory));
        strcpy(newDir->dir, dirBuffer);
        currentDir->next = newDir;
        newDir->next = NULL;
        currentDir = newDir;
    }
    fclose(dirFp);
    remove(DIRSFILE);
    return head;
}
int readHtml(char* buffer, const char* dir) {
    printf("current dir is:%s\n",dir);
    char filename[30];
    sprintf(filename, "%s/index.html",dir);
    FILE *htmlFp = fopen(filename,"r+");
    if(htmlFp == NULL)
        return -1;
    int num = 0;
    int lenth = 0;
    while((num = fread(buffer+lenth,sizeof(char),1024,htmlFp))) {
        lenth += num;
    }
    buffer[lenth] = '\0';
    fclose(htmlFp);
    return 0;
}
int readCss(char* buffer, const char* dir, const char* fileName) {
    char fullName[30];
    sprintf(fullName, "%s/%s", dir, fileName);
    FILE *cssFp = fopen(fullName, "r+");
    if(cssFp == NULL)
        return -1;
    int lenth = 0;
    int num = 0;
    while((num = fread(buffer+lenth,sizeof(char),1024,cssFp))) {
        lenth += num;
    }
    buffer[lenth] = '\0';
    fclose(cssFp);
    return 0;
}
int writeFile(char* data, const char* dirName)
{
    char fileName[20];
    // strcat(fileName, dirName + 4);
    strcpy(fileName, dirName);
    strcat(fileName, ".txt");
    FILE *outPut = fopen(fileName, "w+");
    if(outPut == NULL)
        return -1;
    fprintf(outPut, "%s", data);
    fclose(outPut);
    return 0;
}
void freeDir(directory *dirlist) {
    if (dirlist->next != NULL)
        freeDir(dirlist->next);
    free(dirlist);
}
