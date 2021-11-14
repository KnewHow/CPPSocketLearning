#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** argv) {
    
    if(argc != 3) {
        printf("Using : ./filecopy source dest\n");
        printf("Example: ./filecopy s.txt d.txt\n");
        printf("this program is copy a source file to destinaton file\n");
        return -1;
    }

    FILE *srcfp, *destfp;
    if((srcfp = fopen(argv[1], "r")) == 0) {
        printf("open source file: %s fail\n", argv[1]);
        return -1;
    }
    
    if((destfp = fopen(argv[2], "w+")) == 0) {
        printf("open destination file %s failure\n", argv[2]);
        return -1;
    }

    int nread = 0;
    int *buf = (int*)malloc(100);
    while(1) {
        if((nread = fread(buf, 1, 100, srcfp)) == 0)
            break;
        fwrite(buf, 1, nread, destfp);
    }
    fclose(srcfp);
    fclose(destfp);
    free(buf);
    return 0;

}
