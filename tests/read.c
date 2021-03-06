#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#define MAX_PATH 255

void wing_get_tmp_dir(char *buffer)
{
    const char* tmp = "/tmp";
    if(0 != access(tmp, W_OK)) {
        buffer = NULL;
        return;
    }

    const char* tmp_wing = "/tmp/wing_process";
    if (0 == access(tmp_wing, F_OK)) {
        strcpy(buffer, tmp_wing);
        return;
    }

    if (0 == mkdir(tmp_wing, 0777)) {
        strcpy(buffer, tmp_wing);
        return;
    }
    buffer = NULL;
    return;
}
void wing_get_cmdline(int process_id, char **buffer)
{
    *buffer = NULL;
    char file[MAX_PATH] = {0};
    sprintf(file, "/proc/%d/cmdline", process_id);
    if (access(file, R_OK) != 0) {
        return;
    }


    FILE *handle = fopen((const char*)file, "r");
    if (!handle) {
        *buffer = NULL;
        return;
    }

   // fseek(handle, 0L, SEEK_END);
    int filesize = 0;//ftell(handle);
    while(!feof(handle)){
    getc(handle);filesize++;
    }
//    if (filesize <= 0) {
//        fclose(handle);
//        return;
//    }
    rewind(handle);

    *buffer = (char*)malloc(filesize+1);
    if (*buffer == NULL) {
        fclose(handle);
        return;
    }
    memset(*buffer,0,filesize+1);

    int c;
    char *cs;
    cs = *buffer;

    while(!feof(handle)) {
        c = getc(handle);
        if (!c || c == NULL || c < 32) {
            c = ' ';
        }
        *cs++ = c;
    }
    *cs='\0';
    fclose(handle);

}

int wing_file_is_php(const char *file)
{

    char find_str[]     = " ";
    char path[MAX_PATH] = {0};

    char *find          = strstr(file, find_str);
    if (find != NULL)
    strncpy((char*)path, file, (size_t)(find-file));
    else
    strcpy((char*)path, file);

    FILE *handle = fopen(path, "r");
    if (!handle) {
        return 0;
    }
    //char *find = NULL;
    char line[8] = { 0 };
    memset(line, 0, 8);
    fgets(line, 7, handle);

    find = strstr(line, "<?php");
    if (find == line) {
        fclose(handle);
        return 1;
    }

    memset(line, 0, 8);
    fgets(line, 7, handle);
    find = strstr(line, "<?php");
    if (find == line) {
        fclose(handle);
        return 1;
    }
    fclose(handle);

    return 0;
}


int main() {
    char *buffer = NULL;//[256];
    wing_get_cmdline(17009, &buffer);
    printf("%s", buffer);
    if (buffer)
    free(buffer);


const char *file = "/proc/17009/cmdline";
FILE *fp = fopen(file, "rb");
fseek(fp,0L,SEEK_END); /* 定位到文件末尾 */
int flen = ftell(fp); /* 得到文件大小 */
printf("\r\n%s文件大小%d\r\n", file, flen);
fclose(fp);

     struct stat file_status = { 0 };

        int status = stat( file, &file_status );

        if ( 0 != status )
        {
            perror("Failed to read file status");
            return EXIT_FAILURE;
        }

        printf( "File size: %li\n", file_status.st_size );





    const char *sfile = "/proc/17009/status";
    FILE *sp = fopen(sfile, "r");
    	char sbuffer[256] = { 0 };
    	char mem[32] = { 0 };
    	char *cs = NULL;
    	char *end = NULL;
    	int count = 0;
    	while (!feof(sp)) {
    		memset(sbuffer, 0, 256);
    		fgets(sbuffer, 256, sp);
    		printf("%s", sbuffer);
    		if (strstr(sbuffer, "VmSize") != NULL) {
    			printf("发现VmSize\r\n");
    			cs = (char*)(sbuffer + 7);
    			end = (char*)(sbuffer + strlen(sbuffer));
    			while (cs[0] != '\n') {
    				if (cs[0] == NULL || cs[0] < 32 || cs[0] == ' ') {
    					cs++;
    					continue;
    				}
    				mem[count++] = cs[0];
    				cs++;
    			}

    			break;
    		}
    	}

    	printf("使用内存：%s\r\n", mem);


        return EXIT_SUCCESS;

    //printf("\r\n==%d\r\n", wing_file_is_php("/home/tools/wing-process/tests/wing_process_test.php"));
    return 0;
}