#include <wartime/frame/MapBox.h>
#include <wartime/frame/RawBox.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

using namespace std;

string toString(int size)
{
    ostringstream os;
    os<<size;
    return os.str();
}
int toInt(string s)
{
    int size;
    istringstream is(s);
    is>>size;
    return size;
}

typedef void (*FILEBOX_SOCK_CALLBACK_FUNC)(int pDone,int pTotal);

class FileBox
{
public:
    FileBox(){_callback_func=nullptr;}
    FileBox(string incname,FILEBOX_SOCK_CALLBACK_FUNC CallbackFunc=nullptr)
    {
        setname(incname);
        setcallback(CallbackFunc);
    }
    void setname(string incname)
    {
        name=incname;
    }
    string getname()
    {
        return name;
    }
    void setcallback(FILEBOX_SOCK_CALLBACK_FUNC CallbackFunc)
    {
        _callback_func=CallbackFunc;
    }
    FILEBOX_SOCK_CALLBACK_FUNC getcallback()
    {
        return _callback_func;
    }
private:
    string name;
    FILEBOX_SOCK_CALLBACK_FUNC _callback_func;
};

template<>
int sock::send(FileBox& idx)
{
    int tlcsz=1024*1024+256;
    unsigned char* uubuff=(unsigned char*)malloc(tlcsz);
    if(uubuff==NULL) return -1;
    memset(uubuff,0,tlcsz);

    MapBox box;
    int ret;
    string fn=idx.getname();
    const char* fns=fn.c_str();
    FILE* fp=fopen(fns,"rb");
    if(fp==NULL)
    {
        /// Send Cancel Message
        box.clear();
        box.add("_MAPBOX_PROTOCOL","FILE_TRANSFER");
        box.add("ACTION","TRANSFER_CANCEL");
        ret=send(box);
        /// Free UUBUFF
        free(uubuff);
        if(ret<0) return -2;
        return -3;
    }
    fseek(fp,0L,SEEK_END);
    int fsz=ftell(fp);
    rewind(fp);
    box.add("_MAPBOX_PROTOCOL","FILE_TRANSFER");
    box.add("FILE_SIZE",toString(fsz));
    box.add("FILE_NAME",fn);
    ret=send(box);
    if(ret<=0)
    {
        /// Free UUBUFF
        free(uubuff);
        /// Close File
        fclose(fp);
        return -4;
    }
    ret=recv(box);
    if(ret<=0)
    {
        /// Free UUBUFF
        free(uubuff);
        /// Close File
        fclose(fp);
        return -5;
    }
    if(!box.haskey("_MAPBOX_PROTOCOL")||!box.haskey("RESULT"))
    {
        /// Free UUBUFF
        free(uubuff);
        /// Close File
        fclose(fp);
        return -6;
    }
    if(box["_MAPBOX_PROTOCOL"]!="FILE_TRANSFER"||box["RESULT"]!="OK")
    {
        /// Free UUBUFF
        free(uubuff);
        /// Close File
        fclose(fp);
        /// Transfer Request Denied.
        return -7;
    }

    int total=fsz;
    int sent=0;
    FILEBOX_SOCK_CALLBACK_FUNC _call=idx.getcallback();

    while(sent<total)
    {
        if((total-sent)>=1024*1024)
        {
            int read_done=0;
            int read_need=1024*1024;
            memset(uubuff,0,1024*1024+256);
            while(read_done<read_need)
            {
                int ret=fread(uubuff+read_done,1,read_need-read_done,fp);
                read_done+=ret;
            }
            string ans=base64_encode(uubuff,1024*1024);
            box.clear();
            box.add("DATA",ans);
            ret=send(box);
            if(ret<=0)
            {
                /// Free UUBUFF
                free(uubuff);
                /// Close File
                fclose(fp);
                return -8;
            }
            sent+=read_need;
        }
        else
        {
            int read_done=0;
            int read_need=total-sent;
            memset(uubuff,0,1024*1024+256);
            while(read_done<read_need)
            {
                int ret=fread(uubuff+read_done,1,read_need-read_done,fp);
                read_done+=ret;
            }
            string ans=base64_encode(uubuff,read_need);
            box.clear();
            box.add("DATA",ans);
            box.add("FIN","YES");
            ret=send(box);
            if(ret<=0)
            {
                /// Free UUBUFF
                free(uubuff);
                /// Close File
                fclose(fp);
                return -9;
            }
            sent+=read_need;
        }

        if(_call!=nullptr) _call(sent,total);
    }
    /// Free UUBUFF
    free(uubuff);
    /// Close File
    fclose(fp);

    ret=recv(box);
    if(ret<0)
    {
        return -10;
    }
    if(!box.haskey("RESULT")||box["RESULT"]!="OK")
    {
        return -11;
    }
    return sent;
}

/// Receive File To idx.name
template<>
int sock::recv(FileBox& idx)
{
    int tlcsz=1024*1024+256;
    unsigned char* uubuff=(unsigned char*)malloc(tlcsz);
    if(uubuff==NULL)
    {
        return -1;
    }
    memset(uubuff,0,tlcsz);

    MapBox box;
    int ret=recv(box);
    if(ret<=0)
    {
        free(uubuff);
        return -2;
    }
    if(box["_MAPBOX_PROTOCOL"]=="FILE_TRANSFER"&&box["ACTION"]=="TRANSFER_CANCEL")
    {
        free(uubuff);
        return -3;
    }
    if(box["_MAPBOX_PROTOCOL"]!="FILE_TRANSFER"||!box.haskey("FILE_NAME")||!box.haskey("FILE_SIZE"))
    {
        free(uubuff);
        return -4;
    }
    int sz=toInt(box["FILE_SIZE"]);
    /// REMOTE FILE NAME IGNORED.
    string localname=idx.getname();
    const char* localfile=localname.c_str();
    FILE* fp=fopen(localfile,"wb");
    if(fp==NULL)
    {
        box.clear();
        box.add("_MAPBOX_PROTOCOL","FILE_TRANSFER");
        box.add("RESULT","ERR_NO_MORE_DISK");
        ret=send(box);
        free(uubuff);
        if(ret<=0) return -5;
        return -6;
    }
    int zerototal=sz;
    int zerodone=0;
    thread_local char buff[2048];
    memset(buff,0,2048);
    int error_flag=0;
    while(zerodone<zerototal)
    {
        int ret=fwrite(buff,1,zerototal-zerodone>=1024?1024:zerototal-zerodone,fp);
        if(ret<0)
        {
            error_flag=1;
            break;
        }
        zerodone+=ret;
    }
    if(error_flag)
    {
        fclose(fp);
        unlink(localfile);
        box.clear();
        box.add("_MAPBOX_PROTOCOL","FILE_TRANSFER");
        box.add("RESULT","ERR_NO_MORE_DISK");
        ret=send(box);
        free(uubuff);
        if(ret<=0) return -7;
        return -8;
    }
    rewind(fp);
    box.clear();
    box.add("_MAPBOX_PROTOCOL","FILE_TRANSFER");
    box.add("RESULT","OK");
    ret=send(box);
    if(ret<=0)
    {
        fclose(fp);
        free(uubuff);
        return -9;
    }
    FILEBOX_SOCK_CALLBACK_FUNC _call=idx.getcallback();
    int r_done=0;
    int r_total=zerototal;

    while(1)
    {
        ret=recv(box);
        if(ret<=0)
        {
            fclose(fp);
            free(uubuff);
            return -10;
        }
        if(!box.haskey("DATA"))
        {
            /// Broken Box
            fclose(fp);
            free(uubuff);
            return -11;
        }
        string ens=box["DATA"];
        memset(uubuff,0,1024*1024+256);
        int cdssz=base64_decode_raw(ens,uubuff);
        r_done+=cdssz;
        fwrite(uubuff,1,cdssz,fp);
        if(_call!=nullptr) _call(r_done,r_total);
        if(box["FIN"]=="YES") break;
    }
    fclose(fp);
    box.clear();
    box.add("RESULT","OK");
    ret=send(box);
    if(ret<0)
    {
        fclose(fp);
        free(uubuff);
        return -12;
    }

    fclose(fp);
    free(uubuff);
    return r_total;
}
