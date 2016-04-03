/** WarTime-Project On GitHub
*   https://github.com/Kiritow/WarTime-Project.git
*/

#ifndef WARTIME_FRAME
#error /// This file can only be included by "frame.hpp"
#endif

#ifndef WARTIME_FRAME_CPP
#define WARTIME_FRAME_CPP

#include <support/filesystem_support>

namespace _wartime_buildin_namespace
{
    /// implement
    c_s_class::c_s_class() throw (runtime_error)
    {
        if(_singleton_cnt!=0)
        {
            throw runtime_error("Cannot create more than 1 c_s_class object.");
        }
        _singleton_cnt=1;
    }
    c_s_class::~c_s_class()
    {
        _singleton_cnt=0;
    }
    int c_s_class::regist(const string Topic,function<void(const char*,long)> CallbackFunc)
    {
        for(auto& i:vec)
        {
            if(i.str==Topic)
            {
                return -1;
            }
        }
        pack p;
        p.str=Topic;
        p.func=CallbackFunc;
        vec.push_back(p);
        return 0;
    }
    int c_s_class::cancel(const string Topic)
    {
        for(decltype(vec.size()) i=0;i<vec.size();i++)
        {
            if(vec.at(i).str==Topic)
            {
                vec.erase(vec.begin()+i);
                return 0;
            }
        }
        return -1;
    }
    int c_s_class::exec(const string Topic,const char* Data,long Datasize)
    {
        for(auto& i:vec)
        {
            if(i.str==Topic)
            {
                i.func(Data,Datasize);
                return 0;
            }
        }
        return -1;
    }
    
    /// implement
    clientmessagebus_class::clientmessagebus_class() throw (runtime_error)
    {
        if(_singleton!=0)
            throw runtime_error("Cannot create more than 1 clientmessagebus");
        _singleton=1;
    }
    clientmessagebus_class::~clientmessagebus_class()
    {
        _singleton=0;
    }
    int clientmessagebus_class::regist(const string MessageDesID,function<void(const char*,long)> CallbackFunc)
    {
        return cscenter.regist((string)"MessageBus-"+MessageDesID,CallbackFunc);
    }
    int clientmessagebus_class::cancel(const string MessageDesID)
    {
        return cscenter.cancel((string)"MessageBus-"+MessageDesID);
    }
    int clientmessagebus_class::dealmessage_basic(const char* MessageFileADDR)
    {
        FILE* fp=fopen(MessageFileADDR,"r");
        if(fp==nullptr)
            return -1;
        fseek(fp,0L,SEEK_END);
        long length=ftell(fp);
        rewind(fp);
        char* p=new char[length];
        if(p==nullptr)
        {
            fclose(fp);
            return -2;
        }
        memset(p,0,length);
        char messageid[128];
        memset(messageid,0,128);
        fgets(messageid,128,fp);
        int _l=strlen(messageid);
        if(messageid[_l-1]=='\n')
        {
            messageid[_l-1]=0;
        }
        if(strstr(messageid,"MessageID=")==nullptr)
        {
            fclose(fp);
            delete[] p;
            return -3;
        }
        char* _start_of_messageid=strstr(messageid,"MessageID=");
        string DesID=(string)"MessageBus-"+_start_of_messageid;
        memset(messageid,0,128);
        fgets(messageid,128,fp);
        if(strstr(messageid,"MessageType=")==nullptr)
        {
            fclose(fp);
            delete[] p;
            return -4;
        }
        _start_of_messageid=strstr(messageid,"MessageType=");
        int callret;
        if(strstr(_start_of_messageid,"text")!=nullptr)
        {
            /// MessageType=text
            memset(messageid,0,128);
            while(!feof(fp))
            {
                fgets(messageid,127,fp);
                strcat(p,messageid);
                memset(messageid,0,128);
            }
            fclose(fp);
            callret=cscenter.exec(DesID,p,strlen(p));
        }
        else
        {
            /// MessageType=bin
            int __length=fread(p,length,1,fp);
            fclose(fp);
            callret=cscenter.exec(DesID,p,__length);
        }
        if(callret!=0) return -5;
        else return 0;
    }
    int clientmessagebus_class::dealmessage(const char* MessageFileADDR,action ActWhenFailed) throw (runtime_error)
    {
        int callret=dealmessage_basic(MessageFileADDR);
        if(callret!=-5)
        {
            return callret;
        }
        switch(ActWhenFailed)
        {
        case action::ignore:
            deletelocalfile(MessageFileADDR);
            return -5;
        case action::reserve:
            return -6;
        case action::throw_error:
            throw runtime_error((string)"Cannot found action when resolving "+MessageFileADDR);
        default:
            return -7;
        }
    }
}/// End of namespace _wartime_buildin_namespace

#endif /// End of WARTIME_FRAME_CPP
