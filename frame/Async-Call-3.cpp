/** WarTime-Project On GitHub
*   https://github.com/Kiritow/WarTime-Project.git
*/

/** [Part of] CPPLIB TRX : Asynchronous Call ver 3
*   This file (Async-Call-3.cpp) is part of CppLib Technical Report X.
*    Static Data Field Used. The program would be larger but stable. (Because Pointers are always avaliable)
*/

#ifdef CPPLIB_TRX_ASYNC_CALL
#error "Async Call ver 3 is conflict with ver 1."
#endif

#ifdef CPPLIB_TRX_ASYNC_CALL_2ND
#error "Async Call ver 3 is conflict with ver 2."
#endif

#ifndef CPPLIB_TRX_ASYNC_CALL_3RD
#define CPPLIB_TRX_ASYNC_CALL_3RD

#include <thread>
using namespace std;

#define THREAD_FUNC(return_type,func_name,args...) void func_name(bool* pover,bool* pcancel,return_type* pans,##args)
#define FUNC_BEGIN {if(pover!=nullptr) *pover=false;if(pcancel!=nullptr) *pcancel=false;
#define CANCEL_POINT {if(pcancel!=nullptr&&*pcancel) return;}
#define SETANS(data) {if(pans!=nullptr) *pans=data;}
#define FUNC_END {if(pover!=nullptr) *pover=true;}}

#define NEW_ASYNC_THREAD(thread_id,return_type,thread_func,args...) static bool thread_id##_over;thread_id##_over=false;static bool thread_id##_cancel;thread_id##_cancel=false;static return_type thread_id##_ans;thread thread_id##_TD(thread_func,&thread_id##_over,&thread_id##_cancel,&thread_id##_ans,##args);thread_id##_TD.detach();

#define NEW_BACKGROUND_THREAD(thread_id,thread_func,args...) static bool thread_id##_over;thread_id##_over=false;static bool thread_id##_cancel;thread_id##_cancel=false;thread thread_id##_TD(thread_func,&thread_id##_over,&thread_id##cancel,nullptr,##args);thread_id##_TD.detach();

#define NEW_DETACHED_THREAD(thread_id,thread_func,args...) thread thread_id##_TD(thread_func,nullptr,nullptr,nullptr,##args);thread_id##_TD.detach();

#define CANCEL_THREAD(thread_id) thread_id##_cancel=true;
#define ISOVER(thread_id) (thread_id##_over)
#define WAIT_FOR(thread_id) {while(!thread_id##_over);}
#define GETANS(thread_id) [&](){if(ISOVER(thread_id)) {return thread_id##_ans;}else{WAIT_FOR(thread_id);return thread_id##_ans;}}();

#endif /// End of CPPLIB_TRX_ASYNC_CALL_3RD
