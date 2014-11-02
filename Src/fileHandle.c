/****************************************************
 * Author: Jin Jay
 * Created On: 2014-10-23
 * Last-Modified: 2014-11-2
 */

#include "fileHandle.h"

int readCurrentDir() {
	struct dirent *ptr;
	DIR *dir = opendir(".");
	while((ptr = readdir(dir)) != NULL) {
		#ifdef _WIN32
            printf("d_name: %s\n", ptr->d_name);
        #endif
        #ifdef __linux
            printf("d_type:%d d_name: %s\n", ptr->d_type,ptr->d_name);
        #endif
	}
	return 1;
}

int writeFile(int i, const char *data) {
	// 产生待写入的文件名
	char *filename = (char *)malloc(sizeof(char));
	sprintf(filename , "web%d.txt", i);
	FILE *output = fopen(filename, "w");
	// 释放内存
	free(filename);
	// 文件不能打开
	if (output == NULL)
		return 0;
	fprintf(output, "%s", data);
	fclose(output);
	return 1;
}
