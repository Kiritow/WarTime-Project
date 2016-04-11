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


#endif // WARTIME_TINY_BROWSER
