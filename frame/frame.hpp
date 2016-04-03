/** WarTime-Project On GitHub
*   https://github.com/Kiritow/WarTime-Project.git
*/

#ifndef WARTIME_FRAME
#define WARTIME_FRAME

#include <frame/frame.cpp>

namespace _wartime_buildin_namespace
{
    class noncopyable
    {
    protected:
        noncopyable()=default;
        virtual ~noncopyable()=default;
        noncopyable(const noncopyable&)=delete;
        noncopyable& operator = (const noncopyable&)=delete;
    };
    /// pre-declaration
    class clientmessagebus_class;
    /// declaration
    class c_s_class : public noncopyable
    {
    public:
        c_s_class() throw(runtime_error);
        ~c_s_class();
    private:
        int regist(const string Topic,function<void(const char*,long)> CallbackFunc);
        int cancel(const string Topic);
        int exec(const string Topic,const char* Data,long Datasize);
        static int _singleton_cnt;
        struct pack
        {
            string str;
            function<void(const char*,long)> func;
        };
        vector<pack> vec;
        friend clientmessagebus_class;
    };
    int c_s_class::_singleton_cnt=0;
    
    /// declaration
    class clientmessagebus_class : public noncopyable
    {
    public:
        clientmessagebus_class() throw (runtime_error);
        ~clientmessagebus_class();
        int regist(const string MessageDesID,function<void(const char*,long)> CallbackFunc);
        int cancel(const string MessageDesID);
        /// Action on dealmessage Failed.
        /// action::ignore : Ignore This Message. ( Delete the message file )
        /// action::reserve : No action and leave the message file existing.
        /// action::throw_error : Throw an 'runtime_error' exception with file address.
        enum class action {ignore=1,reserve,throw_error};
        int dealmessage(const char* MessageFileADDR,action ActWhenFailed) throw (runtime_error);
    private:
        int dealmessage_basic(const char* MessageFileADDR);
        static int _singleton;
    };
    int clientmessagebus_class::_singleton=0;
}/// End of namespace _wartime_buildin_namespace

_wartime_buildin_namespace::c_s_class cscenter;
_wartime_buildin_namespace::clientmessagebus_class messagebus;

#endif /// End of WARTIME_FRAME
