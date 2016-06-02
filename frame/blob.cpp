/** Blob : Binary Large Object.*/

#ifdef __C4DROID__
#include <cpplib/cpplib#busybox>
#include <cpplib/cpplib#exception>
#endif

#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdio>
using namespace std;

#define BUILDIN_NAMESPACE(name) _cpplib_buildin_namespace_##name

class blob
{
private:
    struct _t
    {
        string str;
    };
    struct _b
    {
    public:
        /// Data Fields
        void* data;
        int size;
        /// Default Constructor
        _b()
        {
            data=nullptr;
            size=0;
        }
        /// Destructor
        ~_b()
        {
            free(data);
        }
        /// Copy Operator and Copy Constructor
        _b(const _b& inc)
        {
            size=inc.size;
            data=malloc(size);
            if(data==nullptr)
            {
                size=0;
            }
            else
            {
                memcpy(data,inc.data,size);
            }
        }
        _b& operator = (const _b& inc)
        {
            if(this==&inc)
            {
                return *this;
            }
            free(data);
            data=nullptr;
            size=inc.size;
            data=malloc(size);
            if(data==nullptr)
            {
                size=0;
            }
            else
            {
                memcpy(data,inc.data,size);
            }
            return *this;
        }
        /// Move Operator and Move Constructor
        _b& operator = (_b&& inc)
        {
            free(data);
            size=inc.size;
            data=inc.data;
            inc.data=nullptr;
            return *this;
        }
        _b(_b&& inc)
        {
            size=inc.size;
            data=inc.data;
            inc.data=nullptr;
        }
    };
public:
    enum class blobtype { null,bin,text };
    blobtype type;
    _b bin;
    _t text;
    blob()
    {
        type=blobtype::null;
    }
    blob(blobtype inctype) : type(inctype) { }
};

/** Exception For Blob */

class curl_error : public basic_exception
{
public:
    explicit curl_error(const string& inc):basic_exception(string("Error Occur while fetching page: ")+inc) {}
};

/** Execute std::string shell command */
int strexec(const string& cmdstr)
{
    int ret=system(cmdstr.c_str());
    return ret;
}

/** Get temp file */
string gettmpfile()
{
    thread_local char buff[256];
    gettmpfile(buff,256);
    return string(buff);
}

class atmp
{
public:
    atmp()
    {
        file=gettmpfile();
    }
    ~atmp()
    {
        unlink(file.c_str());
    }
    string name()
    {
        return file;
    }
private:
    string file;
};

blob getwebpage(const string& webpage) throw (curl_error)
{
    string cmd;
    blob b;
    b.type=blob::blobtype::text;
    atmp file,efile,ecfile;
    thread_local char buffer[1024];
    bzero(buffer,1024);
    cmd=(string)"curl "+webpage+" > "+file.name()+" 2> "+efile.name();
    strexec(cmd);
    cmd=(string)"grep 'curl' "+efile.name()+" > "+ecfile.name();
    strexec(cmd);
    if(getfilesize(ecfile.name().c_str())>3)
    {
        /// Error occur when curl...
        throw curl_error(webpage);
    }
    FILE* fp=fopen(file.name().c_str(),"r");
    b.text.str.clear();
    while(!feof(fp))
    {
        fgets(buffer,1024,fp);
        b.text.str.append(string(buffer));
        bzero(buffer,1024);
    }
    fclose(fp);
    return b;
}

blob loadbinfile(const string& binaryfile) throw(file_error,mem_error)
{
    int sz=getfilesize(binaryfile.c_str());
    if(sz<0) throw file_error("File not exist");
    void* dx=malloc(sz+8);
    if(dx==nullptr) throw mem_error(sz+8);
    memset(dx,0,sz+8);
    blob b(blob::blobtype::bin);
    FILE* fp=fopen(binaryfile.c_str(),"rb");
    fread(dx,sz+8,1,fp);
    fclose(fp);
    b.bin.data=dx;
    b.bin.size=sz+8;
    return b;
}
