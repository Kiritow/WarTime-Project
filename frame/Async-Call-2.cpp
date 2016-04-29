/** WarTime-Project On GitHub
*   https://github.com/Kiritow/WarTime-Project.git
*/

/** [Part of] CPPLIB TRX : Asynchronous Call ver 2
*   This file (Async-Call-2.cpp) is part of CppLib Technical Report X.
*   Unstable implement of async-call.
*/
#ifdef CPPLIB_TRX_ASYNC_CALL
#error "Async Call ver 2 is conflict with ver 1."
#endif

#ifndef CPPLIB_TRX_ASYNC_CALL_2ND
#define CPPLIB_TRX_ASYNC_CALL_2ND

#include <thread>
using namespace std;

#define THREAD_FUNC(Type,name,args...) void name(bool* pover,bool* pcancle,Type* panswer,##args)
#define FUNC_BEGIN {*pover=false;*pcancle=false;
#define SETANS(data) *panswer=data;
#define CANCEL_POINT {if(*pcancle) return;}
#define FUNC_END *pover=true;}

#ifdef DEBUG
#include <cstdio>
#undef FUNC_BEGIN
#define FUNC_BEGIN  {*pover=false;*pcancle=false;printf("Begin of %s\n",__func__);
#undef FUNC_END
#define FUNC_END *pover=true;printf("End of %s\n",__func__);}
#endif

#define NEW_ASYNC_THREAD(id,Type,thread_func,args...)  bool id##_OVER=false;bool id##_CANCLE=false;Type id##_ANSWER;thread id##_TD(thread_func,&id##_OVER,&id##_CANCLE,&id##_ANSWER,##args);id##_TD.detach();

#define CANCEL_THREAD(id) id##_CANCLE=true;
#define ISOVER(id) (id##_OVER)
#define WAIT_FOR(id) {while(!id##_OVER);}
#define GETANS(id) [&](){if(ISOVER(id)) {return id##_ANSWER;}else{WAIT_FOR(id);return id##_ANSWER;}}();

#endif /// End of CPPLIB_TRX_ASYNC_CALL_2ND
