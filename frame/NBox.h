/** NBox - A Light Key-Value Container without base64 en/decode
*   Based On STL(map) and binary container: string. GSOCK General interface Supported.
*   Created By Kiritow.
*   Created On: 20161108
*
*/
#ifndef _KIRITOW_NBOX
#define _KIRITOW_NBOX

#include <cpplib/cpplib#gsock>
#include <string>
#include <map>
using namespace std;

class NBox
{
public:
    string toBinaryString()
    {
        string ans;
        for(auto& cc:pool)
        {
            int len=cc.first.size();
            int nlen=ntohl(len);
            ans.append((const char*)&nlen,sizeof(nlen));
            ans.append(cc.first);

            len=cc.second.size();
            nlen=ntohl(len);
            ans.append((const char*)&nlen,sizeof(nlen));
            ans.append(cc.second.data(),len);
        }
        return ans;
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
    void add(string Key,const void* pData,int iDataSize)
    {
        pool[Key]=string((const char*)pData,iDataSize);
    }
    int remove(string Key)
    {
        auto iter=pool.find(Key);
        if(iter==pool.end()) return 0;
        pool.erase(iter);
        return 1;
    }
protected:
    map<string,string> pool;
};

#ifndef _GNR_toBinaryString
#define _GNR_toBinaryString
template<typename T>
string toBinaryString(T&);
#endif

template<>
string toBinaryString(NBox& a)
{
    return a.toBinaryString();
}

NBox toNBox(string incBinaryString)
{
    NBox box;
    int sz=incBinaryString.size();
    int done=0;
    while(done<sz)
    {
        int nlen;
        memcpy(&nlen,incBinaryString.data()+done,sizeof(nlen));
        int len=ntohl(nlen);
        string tag(incBinaryString.begin()+done+4,incBinaryString.begin()+done+4+len);
        done+=len+4;
        memcpy(&nlen,incBinaryString.data()+done,sizeof(nlen));
        len=ntohl(nlen);
        string data(incBinaryString.begin()+done+4,incBinaryString.begin()+done+4+len);
        box.add(tag,data);
        done+=len+4;
    }
    return box;
}

template<>
int sock::send(NBox& box)
{
	string ans=box.toBinaryString();
	const char* p=ans.data();
	int sz=ans.size();
	int nsz=htonl(sz);
	int ret=send((const char*)&nsz,sizeof(nsz));
	if(ret<=0) return ret;
	int done=0;
	while(done<sz)
	{
		ret=send(p+done,sz-done);
		if(ret<=0) return ret;
		done+=ret;
	}
	return done+sizeof(int);
}

template<>
int sock::recv(NBox& box)
{
	box.clear();
	thread_local char buff[1024];
	memset(buff,0,1024);
	int nsz;
	int ret=recv((char*)&nsz,sizeof(nsz));
	if(ret<=0) return ret;
	int sz=ntohl(nsz);
	int done=0;
	string ans;
	while(done<sz)
	{
		ret=recv(buff,nsz-done>1024?1024:nsz-done);
		if(ret<=0) return ret;
		ans+=string(buff,ret);
		done+=ret;
	}
	box=toNBox(ans);
	return done+sizeof(int);
}

/**
//Test Code
int main()
{
    NBox box;
    box.add("Name","Kiritow");
    box.add("Sex","Male");

    string ans=box.toBinaryString();
    printf("Size: %d\n",ans.size());
    for(int i=0;i<(int)ans.size();i++)
    {
        printf("%c ",*(ans.data()+i));
    }
    printf("\n");
    NBox box2=toNBox(ans);
    string ans2=toBinaryString(box2);
    printf("%d\n",ans==ans2);
    printf("%s\n",box2["Name"].c_str());
    printf("%s\n",box2["Sex"].c_str());
    return 0;
}
*/

#endif /// End of _KIRITOW_NBOX(NBox.h)
