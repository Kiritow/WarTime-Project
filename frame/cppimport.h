/**
CPPLIB Extends - CPP Import
A Import Syntax in C++ like C#.
Usage:
import(Library,Label) - Import a library. *Important*:Label should be the only one of its name.
getfunc(ReturnType,FunctionName,Args...) - Declare A Function. If the function cannot be found, it will be set NULL.
getfuncin(Label,ReturnType,FunctionName,Args...) - Declare A Function. This function will be found in loaded library labled Label.
*/

#ifndef _CPPIMPORT_H
#define _CPPIMPORT_H

#include <cpplib/cpplib#dlibctrl>

#include <stdexcept>
#include <memory>
#include <vector>
#include <mutex>

#include <cstdio>
using namespace std;

namespace _cpplib_buildin_namespace
{
    /// DECL
    class dlibctrl_manager
    {
    public:
        dlibctrl* dbptr;
        ~dlibctrl_manager();
    };

    class _dlibctrl_center_class
    {
    public:
        void add(dlibctrl_manager* ptr)
        {
            m.lock();
            vec.push_back(ptr);
            m.unlock();
        }
        int del(dlibctrl_manager* ptr)
        {
            m.lock();
            int sz=vec.size();
            for(int i=0;i<sz;i++)
            {
                if(vec.at(i)==ptr)
                {
                    vec.erase(vec.begin()+i);
                    m.unlock();
                    return 0;
                }
            }
            m.unlock();
            return -1;
        }
        void* find(const char* name)
        {
            m.lock();
            for(auto& ptr:vec)
            {
                void* p;
                if((p=ptr->dbptr->getproc(name))!=NULL)
                {
                    m.unlock();
                    return p;
                }
            }
            m.unlock();
            return NULL;
        }
        void* findin(const char* name,dlibctrl_manager* incptr)
        {
            m.lock();
            bool found=false;
            for(auto& ptr:vec)
            {
                if(ptr==incptr)
                {
                    found=true;
                    break;
                }
            }
            if(!found)
            {
                m.unlock();
                return NULL;
            }
            void* p=incptr->dbptr->getproc(name);
            m.unlock();
            return p;
        }
    private:
        vector<dlibctrl_manager*> vec;
        mutex m;
    }dlibctrl_center;

    ///IMPL
    dlibctrl_manager::~dlibctrl_manager()
    {
        dlibctrl_center.del(this);
        delete dbptr;
    }
}



#define import(DynamicLibraryName,Label) \
unique_ptr<_cpplib_buildin_namespace::dlibctrl_manager> Label=[]()->unique_ptr<_cpplib_buildin_namespace::dlibctrl_manager>\
{\
dlibctrl* p=new dlibctrl;\
int ret=p->open(DynamicLibraryName);\
if(ret<0)\
{\
delete p;\
return NULL;\
}\
_cpplib_buildin_namespace::dlibctrl_manager* manager=new _cpplib_buildin_namespace::dlibctrl_manager;\
manager->dbptr=p;\
_cpplib_buildin_namespace::dlibctrl_center.add(manager);\
return unique_ptr<_cpplib_buildin_namespace::dlibctrl_manager>(manager);\
}()

#define getfunc(ReturnType,FunctionName,Args...) ReturnType(*FunctionName)(##Args)=[]()->ReturnType(*)(##Args)\
{\
void* p=NULL;\
if((p=_cpplib_buildin_namespace::dlibctrl_center.find(#FunctionName))==NULL)\
{\
    return NULL;\
}\
return (ReturnType(*)(##Args))p;\
}()


#define getfuncin(Label,ReturnType,FunctionName,Args...) ReturnType(*FunctionName)(##Args)=[]()->ReturnType(*)(##Args)\
{\
void* p=NULL;\
if((p=_cpplib_buildin_namespace::dlibctrl_center.findin(#FunctionName,Label.get()))==NULL)\
{\
    return NULL;\
}\
return (ReturnType(*)(##Args))p;\
}()

#endif /// _CPPIMPORT_H
