/** WarTime-Project On GitHub
*   https://github.com/Kiritow/WarTime-Project.git
*/

/** Tiny Browser
*   HTTP Request Parser and HTML File Converter
*   Open-Source.
*   Created By HC TECH.
*   Powered By CPPLIB ( HC TECH )
*/

#ifndef WARTIME_TINY_BROWSER
#define WARTIME_TINY_BROWSER

#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <string>
using namespace std;
string linepraser(string text)
{
    string ans;
    for(auto& i:text)
    {
        if(i=='\r') continue;
        if(i=='\n')
        {
            ans.push_back('\r');
            ans.push_back('\n');
        }
        else
        {
            ans.push_back(i);
        }
    }
    return ans;
}

string fileconvert(string LocalFile) throw (runtime_error)
{
    FILE* fp=fopen(LocalFile.c_str(),"r");
    if(fp==nullptr)
    {
        throw runtime_error(string("file convert: Cannot found request file"));
    }
    thread_local char pool[1024];
    memset(pool,0,1024);
    string ans;
    while(fgets(pool,1023,fp)>0)
    {
        ans=ans+linepraser(pool);
        memset(pool,0,1024);
    }
    fclose(fp);
    return ans;
}

string urldecode(string url)
{
    string ans;
    bool flag=false;
    int cnt=0;
    char bita,bitb;
    int ansx;
    for(auto& i:url)
    {
        if(flag)
        {
            if(cnt==0)
            {
                bita=i;
                cnt=1;
                continue;
            }
            else
            {
                flag=false;
                cnt=0;
                bitb=i;
                ansx=0;
                if(bita>='0'&&bita<='9')
                {
                    ansx+=bita-'0';
                }
                else if(bita>='A'&&bita<='F')
                {
                    ansx+=bita-'A'+10;
                }
                else if(bita>='a'&&bita<='f')
                {
                    ansx+=bita-'a'+10;
                }
                ansx=ansx*16;
                if(bitb>='0'&&bitb<='9')
                {
                    ansx+=bitb-'0';
                }
                else if(bitb>='A'&&bitb<='F')
                {
                    ansx+=bita-'A'+10;
                }
                else if(bitb>='a'&&bitb<='f')
                {
                    ansx+=bitb-'a'+10;
                }
                ans.push_back((char)ansx);
                continue;
            }
        }
        if(i!='%')
        {
            ans.push_back(i);
        }
        else
        {
            flag=true;
        }
    }
    return ans;
}
#endif // WARTIME_TINY_BROWSER
