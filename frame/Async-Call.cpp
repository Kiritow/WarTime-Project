/** WarTime-Project On GitHub
*   https://github.com/Kiritow/WarTime-Project.git
*/

/** [Part of] CPPLIB TRX : Asynchronous Call
*   This file (Async-Call.cpp) is part of CppLib Technical Report X.
*   Unstable implement of async-call.
*/

#ifndef CPPLIB_TRX_ASYNC_CALL
#define CPPLIB_TRX_ASYNC_CALL

#include <thread>
using namespace std;

#define FUNCTION_STD(FuncName,args...) void FuncName(int* _buildin_trg,##args)
#define FUNCTION_START {*_buildin_trg=1;
#define FUNCTION_END *_buildin_trg=2;}

#define NEWTHREAD(LaunchType,ThreadName,ThreadFunc,args...) _buildin_threadpack _buildin_threadpack_pack_##ThreadName(LaunchType,new thread(ThreadFunc,_buildin_threadpack_pack_##ThreadName.getpint(),##args))
#define ISOVER(ThreadName) (_buildin_threadpack_pack_##ThreadName.getint()==2)

#define LAUNCH_JOIN 0
#define LAUNCH_DETACH 1
class _buildin_threadpack
{
private:
    thread* s;
    int status;
    int launch_type;
public:
    _buildin_threadpack(int inclaunch_type,thread* ins)
    {
        s=ins;
        launch_type=inclaunch_type;
    }
    int getint()
    {
        return status;
    }
    int* getpint()
    {
        return &status;
    }
    ~_buildin_threadpack()
    {
        if(s!=nullptr)
        {
        	if(launch_type==LAUNCH_JOIN)
            {
            	s->join();
            }
            else
            {
            	s->detach();
            }
        }
        delete s;
    }
};

#endif /// End of CPPLIB_TRX_ASYNC_CALL
