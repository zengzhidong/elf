#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <assert.h>

NK_Int  NK_GetFileSize(NK_PChar pathFile)
{
#if defined(_WIN32)
    #error "error : no implemented!"
    return 0;
#else
    // get font file size
    struct stat stStatBuf;
    if(stat(pathFile, &stStatBuf) < 0){
        return 0;
    }
    return (NK_Int)stStatBuf.st_size;
#endif
}

NK_Int NK_ReadFile2Buffer(const NK_PChar file, NK_PChar *data)
{
    NK_EXPECT_RETURN_VAL(NK_Nil != file, -1);
    FILE *fp = NK_Nil;
    char *buf = NK_Nil;
    NK_Int filesize = (int)NK_GetFileSize(file);
    if (filesize >  0) {
        char *ptr = NK_Nil;
        int rdsize = 0;
        fp = fopen(file, "r");
        NK_EXPECT_RETURN_VAL( NK_Nil != fp, -1);
        buf = (char *)malloc(filesize+10);
        ptr = buf;
        for(;;) {
            int ret = (int)fread(ptr, 1, filesize - rdsize, fp);
            if (ret > 0) {
                rdsize += ret;
                ptr += ret;
                if (rdsize == filesize) break;
            } else {
                break;
            }   
        }
        
        //if (feof(fp) != 0) 
        if (rdsize == filesize)
        {
            fclose(fp);
            buf[rdsize] = 0;
            *data = buf;
            return rdsize;
        } else {
            goto _fail_exit;
        }
    }

_fail_exit:
    if (buf) free(buf);
    if (fp) fclose(fp);
    return -1;
}
