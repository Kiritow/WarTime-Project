#ifndef _wartime_frame_memfile
#define _wartime_frame_memfile

#include <cstdio>
#include <cstdlib>
#include <cstring>
using namespace std;


int GetFileSize(const char* FileAddress)
{
    FILE* fp=fopen(FileAddress,"rb");
    if(fp==NULL) return -1;
    fseek(fp,0L,SEEK_END);
    int totalsz=ftell(fp);
    fclose(fp);
    return totalsz;
}

int LoadFileToMem(const char* FileAddress,unsigned char* buff)
{
    FILE* fp=fopen(FileAddress,"rb");
    if(fp==NULL) return -1;
    fseek(fp,0L,SEEK_END);
    int totalsz=ftell(fp);
    fseek(fp,0L,SEEK_SET);
    int done=0;
    while(done<totalsz)
    {
        int ret=fread(buff+done,1,totalsz-done,fp);
        done+=ret;
    }
    fclose(fp);
    return done;
}
int DumpMemToDisk(const unsigned char* buff,int buff_size,const char* TargetFile)
{
    FILE* fp=fopen(TargetFile,"wb");
    if(fp==NULL) return -1;
    int totalsz=buff_size;
    int done=0;
    while(done<totalsz)
    {
        int ret=fwrite(buff+done,1,totalsz-done,fp);
        done+=ret;
    }
    fclose(fp);
    return done;
}

#endif /// End of _wartime_frame_memfile
