/** MapBox - A Light Key-Value Container.
*   Based On STL(map) and Base64. GSOCK General interface Supported.
*   Created By Kiritow.
*   Created On: 20161002
*/
#ifndef _KIRITOW_MAPBOX
#define _KIRITOW_MAPBOX

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <memory>
#include <string>
#include <sstream>
#include "base64.h"
#include <cpplib/cpplib#gsock>
using namespace std;

class MapBox
{
public:
    string toString()
    {
        ostringstream os;
        for(auto& cc:pool)
        {
            string key=base64_encode((unsigned char*)cc.first.c_str(),cc.first.size());
            string val=base64_encode((unsigned char*)cc.second.c_str(),cc.second.size());
            os<<key<<":"<<val<<";";
        }
        return os.str();
    }
    string operator [] (string indexstr)
    {
        return pool[indexstr];
    }
    bool haskey(string key)
    {
        return pool.find(key)!=pool.end();
    }
    string* getvalp(string key)
    {
        auto iter=pool.find(key);
        if(iter==pool.end()) return nullptr;
        else return &(iter->second);
    }
    void clear()
    {
        pool.clear();
    }
    void add(string Key,string Value)
    {
        pool[Key]=Value;
    }
    void remove(string Key)
    {
        auto iter=pool.find(Key);
        if(iter==pool.end()) return;
        pool.erase(iter);
    }
private:
    map<string,string> pool;
};

template<>
int sock::send(MapBox& box)
{
    string str=box.toString();
    int sz=str.size();
    int nsz=htonl(sz);
    int ret=send((const char*)&nsz,sizeof(nsz));
    if(ret<=0) {return ret;} /// Failed
    int sent=0;
    while(sent<sz)
    {
        ret=send(str.c_str()+sent,sz-sent);
        if(ret<=0)
        {
            return ret;/// Failed
        }
        sent+=ret;
    }
    return sent+sizeof(nsz);
}

template<>
int sock::recv(MapBox& box)
{
    box.clear();
    thread_local char buff[1024];
    memset(buff,0,1024);
    int nsz;
    int ret=recv((char*)&nsz,sizeof(nsz));
    if(ret<0) return ret; /// Failed
    int sz=ntohl(nsz);
    int recd=0;
    string ans;
    while(recd<sz)
    {
        ret=recv(buff,sz-recd>1023?1023:sz-recd);
        if(ret<=0) return ret;
        ans+=buff;
        memset(buff,0,1024);
        recd+=ret;
    }
    /// Parse It.
    char* tp=(char*)ans.c_str();
    char* p=tp;
    while(p<tp+sz)
    {
        char* q=strstr(p,":");
        char* t=strstr(p,";");
        if(!q||!t) break;
        box.add(ans.substr(p-tp,q-p),ans.substr(q-tp+1,t-q+1));
    }
    return recd+sizeof(int);
}

/*
int main()
{
    MapBox mbox;
    mbox.add("Name","Kiritow");
    mbox.add("Sexual","Male");
    string s=mbox.toString();
    printf("%s\n",s.c_str());
}
//*/
#endif ///End of _KIRITOW_MAPBOX(mapbox.h)
