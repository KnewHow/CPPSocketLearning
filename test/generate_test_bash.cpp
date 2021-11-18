#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) {
    FILE *fp = fopen("./test.sh", "w+");
    char buffer[1024];
    for(int i = 0; i < 2000; ++i) {
        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, "./client 123.60.46.246 5005 &\n");
        fwrite(buffer, strlen(buffer), 1, fp);
    }
    fclose(fp);
}