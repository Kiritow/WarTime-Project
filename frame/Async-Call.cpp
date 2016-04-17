/** WarTime-Project On GitHub
*   https://github.com/Kiritow/WarTime-Project.git
*/

/** [Part of] CPPLIB TRX : Asynchronous Call
*   This file (Async-Call.cpp) is part of CppLib Technical Report X.
*   Unstable implement of async-call.
*/

#ifndef CPPLIB_TRX_ASYNC_CALL
#define CPPLIB_TRX_ASYNC_CALL

#define FUNCTION_STD(FuncName,args...) void FuncName(int* _buildin_trg,##args)
#define FUNCTION_START {
#define FUNCTION_END *_buildin_trg=1;}

#define NEWTHREAD(ThreadName,ThreadFunc,args...) _buildin_threadpack _buildin_threadpack_pack_##ThreadName(new thread(ThreadFunc,_buildin_threadpack_pack_##ThreadName.getpint(),##args))
#define ISOVER(ThreadName) (_buildin_threadpack_pack_##ThreadName.getint()==1)

class _buildin_threadpack
{
private:
    thread* s;
    int status;
public:
    _buildin_threadpack(thread* ins)
    {
        status=0;
        s=ins;
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
            s->join();
        }
        delete s;
    }
};

#endif /// End of CPPLIB_TRX_ASYNC_CALL
