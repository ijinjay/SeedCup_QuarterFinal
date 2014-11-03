#include "IO.h"
int main() {
    readHtmlFiles();
    system("pause");
    return 0;
}
int readHtmlFiles() {
    system(GETDIRSCMD);
    FILE *dirFp = fopen(DIRSFILE,"r+");
    char dirBuffer[1024];
    memset(dirBuffer, '\0', 1024);
    while(fgets(dirBuffer, 1024, dirFp)) {
        int len = strlen(dirBuffer);
        dirBuffer[len - 1] = '\0';
        chdir(dirBuffer);
        printf("current dir is:%s\n",dirBuffer);
        FILE *htmlFp = fopen("index.html","r+");
        if(htmlFp == NULL)
            return -1;
        char data[MAXDATA];
        memset(data, '\0', MAXDATA);
        fread(data, sizeof(char), MAXDATA,htmlFp);
        fclose(htmlFp);
        //handle Html file
        //read and handle css file
        chdir("..");
        //write file
        char outPutName[20] = "web";
        strcat(outPutName, dirBuffer + 4);
        strcat(outPutName, ".txt");
        //a example
        //writeFile(dirBuffer, outPutName);
        memset(outPutName, '\0', 20);
    }
    fclose(dirFp);
    remove(DIRSFILE);
    return 1;
}
int readCssFile(char* data,const char* fileName) {
    FILE *cssFp = fopen(fileName, "r+");
    if(cssFp == NULL)
        return -1;
    int lenth = 0;
    int num = 0;
    while(fread(data+lenth,sizeof(char),1024,cssFp)) {
        lenth += num;
    }
    fclose(cssFp);
    return 1;
}
int writeFile(char* data, const char* fileName) {
    FILE *outPut = fopen(fileName, "w+");
    if(outPut == NULL)
        return -1;
    fprintf(outPut, "%s", data);
    fclose(outPut);
    return 0;
}
