/** WarTime-Project On GitHub
*   https://github.com/Kiritow/WarTime-Project.git
*/

/** WarTime Internationalization - String Getter
*/

#ifndef WARTIME_INTERNATIONLIZATION
#define WARTIME_INTERNATIONLIZATION

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
/** Type Define of MHANDLE
*   This will become a standard of CPPLIB Handle System.
*   Till now, name "MHANDLE", "CHANDLE", "KHANDLE" are candidates.
*   ( We can't use HANDLE directly. Because on Windows platform, HANDLE has been declared. )
*/
using MHANDLE = void*;

/** Namespace Definition
*   This will become a standard of WarTime Marco Definition Standard.
*/
#define _WARTIME_NAMESPACE_BEGIN namespace _wartime_buildin_namespace{
#define _WARTIME_NAMESPACE_END }

_WARTIME_NAMESPACE_BEGIN

/** String Getter ( Pure C++ File IO )
*   Read String From File with index.
*   Standard:
*   ID String
*   Example: (en_US.txt)
*   0 Hello!
*   1 Welcome to WarTime
*/
MHANDLE GetStringGetter(const std::string& FileName)
{
    std::ifstream ifs(FileName);
    if(!ifs.is_open())
    {
        return nullptr;
    }
    auto handle=new std::vector<std::pair<int,std::string>>;
    if(handle==nullptr)
    {
        return nullptr;
    }
    int id;
    while(ifs>>id)
    {
        ifs.ignore(1);
        std::string k;
        std::getline(ifs,k);
        handle->push_back(std::pair<int,std::string>(id,k));
    }
    return handle;
}

void DestoryStringGetter(MHANDLE handle)
{
    delete static_cast<std::vector<std::pair<int,std::string>>*>(handle);
}

std::string GetStringByID(MHANDLE handle,int id) throw(std::runtime_error)
{
    if(handle==nullptr)
    {
        throw std::runtime_error("GetStringByID: Cannot operate NULL handle.");
    }
    auto ihand=static_cast<std::vector<std::pair<int,std::string>>*>(handle);
    for(auto& i:*ihand)
    {
        if(i.first==id)
        {
            return i.second;
        }
    }
    throw std::runtime_error("GetStringByID: Cannot Find Target String.");
}

_WARTIME_NAMESPACE_END

#endif /// End of WARTIME_INTERNATIONLIZATION
