[Frame](#frame) | [Internationalization Frame](#internationalization-frame) | [Asynchronous Call Frame 3](#asynchronous-call-frame-3) | [Asynchronous Call Frame 2](#asynchronous-call-frame-2) | [Asynchronous Call Frame (Deprecated)](#asynchronous-call-frame-deprecated) | [Any Class Frame](#any-class-frame) | [Blob Frame](#blob-frame)  
#Frame  
This directory contains the main frame of WarTime.  
>Get access to frame by `#include <frame/frame.hpp>`.  

##Using Namespace  
Classes are defined in namespace **_wartime_buildin_namespace** and objects are created in global namespace.  
Using objects directly is recommended.  

##Available Objects  
**MessageBus** (messagebus)  
`_wartime_buildin_namespace::clientmessagebus_class messagebus`  
>MessageBus is designed to solve messages. Register a "topic" together with callback function. 
Functions will be executed when message with exact topic reachs the messagebus.  

##Other Objects  
**Client-Server Callback Center**(cscenter)  
`_wartime_buildin_namespace::c_s_class cscenter`  
>**Internal Object**. Most of its functions are not `public`. 
It is the main callback center and supports most of callback objects. 
For example, MessageBus's work is based on it.  


#Internationalization Frame  
>Get access to this frame by `#include <frame/Internationalization.cpp>`.  

This frame holds lots of packages.  
A package (type *std::pair<int,std::string>*) contains a key (type *int*) and a string (type *std::string**).  
The frame will load packages from files. Use `GetStringByID` to get string by the key.  
Results may be different after language setting being changed.  

#Asynchronous Call Frame 3  
>Get access to this frame by `#include <frame/Async-Call-3.cpp>`.

**Warning**: This frame (ver 3) is incompatible with ver 2 and ver 1.  
####Function Declarations  
Use `THREAD_FUNC` to declare a function following CPPLIB-TRX-ASYNC-CALL(Ver. 2) standard.  
Use `FUNC_BEGIN` at the beginning of the function.  
Use `FUNC_END` at the end of the function.  
Use `CANCEL_POINT` where your procedure can be canceled.  
Use `SETANS` to set the return value.  
####Thread Declarations  
Use `NEW_ASYNC_THREAD` to run a new asynchronous thread.  
**New** Use `NEW_BACKGROUND_THREAD` to start a background thread. This thread cannot set return value.  
**New** Use `NEW_DETACHED_THREAD` to start a detached thread. This thread cannot set return value and cannot be canceled.  
Use `CANCEL_THREAD` to try to stop a running thread.  
Use `ISOVER` to test if the thread is over.  
Use `WAIT_FOR` to wait for the end of the processing thread.  
Use `GETANS` to get answer. This will call `WAIT_FOR` to wait for the answer.  
####Example Code
```
#include <cstdio>

THREAD_FUNC(int,func,int a)
FUNC_BEGIN
{
    printf("Args is %d\n",a);
    SETANS(3);
}
FUNC_END

int main()
{
    NEW_ASYNC_THREAD(tid,int,func,99);
    WAIT_FOR(tid);
}
```
Result would be
```
Args is 99
```

#Asynchronous Call Frame 2  
>Get access to this frame by `#include <frame/Async-Call-2.cpp>`.

**Warning**: This frame (ver 2) is incompatible with ver 1.  
####Function Declarations
Use `THREAD_FUNC` to declare a function following CPPLIB-TRX-ASYNC-CALL(Ver. 2) standard.  
Use `FUNC_BEGIN` at the beginning of the function.  
Use `FUNC_END` at the end of the function.  
Use `CANCEL_POINT` where your procedure can be canceled.  
Use `SETANS` to set the return value.  
####Thread Declarations
Use `NEW_ASYNC_THREAD` to run a new asynchronous thread.  
Use `CANCEL_THREAD` to try to stop a running thread.  
Use `ISOVER` to test if the thread is over.  
Use `WAIT_FOR` to wait for the end of the processing thread.  
Use `GETANS` to get answer. This will call `WAIT_FOR` to wait for the answer.  
####Example Code
```
#include <cstdio>
struct pod_data
{
	int a;
	int b;
};

THREAD_FUNC(pod_data,func)
FUNC_BEGIN
{
	printf("Start in thread\n");
	CANCEL_POINT;
	this_thread::sleep_for(chrono::seconds(5));
	CANCEL_POINT;
	pod_data k;
	k.a=99;
	k.b=88;
	SETANS(k);
	printf("End of thread\n");
}
FUNC_END

int main()
{
	pod_data s;
	NEW_ASYNC_THREAD(myid,pod_data,func);
	s=GETANS(myid);
	printf("Answer is %d %d\n",s.a,s.b);
	printf("END OF MAIN\n");
	return 0;
}
```
Result would be:
```
Start in thread
End of thread
Answer is 99 88
END OF MAIN
```

#Asynchronous Call Frame (Deprecated)
>Get access to this frame by `#include <frame/Async-Call.cpp>`.  

####Function Declaration  
Use `FUNCTION_STD` to declare a function following CPPLIB-TRX-ASYNC-CALL standard.  
Use `FUNCTION_START` to start a function.  
Use `FUNCTION_END` at the end of a function.  
####Thread Declaration  
Use `NEWTHREAD` to create an asynchronous thread.  
Use `ISOVER` to test if the thread is over.  
####Example Code  
```
#include <iostream>
using namespace std;

FUNCTION_STD(func)
FUNCTION_START
cout<<"In func"<<endl;
FUNCTION_END

int main()
{
    NEWTHREAD(LAUNCH_JOIN,td,func);
    cout<<ISOVER(td)<<endl;
    this_thread::sleep_for(chrono::seconds(2));
    cout<<ISOVER(td)<<endl;
    return 0;
}
```

#Any Class Frame  
>Get access to this frame by `#include <frame/AnyClass.cpp>`  

This source file is from [qicosmos/cosmos](https://github.com/qicosmos/cosmos/blob/master/Any.hpp "C++11 Example Codes").  
Rewritten for [WarTime-Project](https://github.com/Kiritow/WarTime-Project "WarTime-Project On GitHub Main Page")  
**Any Class** provides a simple way of managing different types of data.  

####Example Code  
```
#include <string>
using namespace std;

int main()
{
    Any n;
    string s="Hello";
    n=s;
    try
    {
        int ss=n.AnyCast<int>(true);
        cout<<"Int:"<<ss<<endl;
    }
    catch(...)
    {
        cout<<"Cannot cast."<<endl;
    }
    try
    {
        string kk=n.AnyCast<string>();
        cout<<"String:"<<kk<<endl;
    }
    catch(...)
    {
        cout<<"Cannot Cast."<<endl;
    }
    return 0;
}
```
Result would be:
```
can not cast std::string to int
Cannot cast.
String:Hello
```

#Blob Frame  
>Get access to this frame by `#include <frame/blob.cpp>`.  

Blob Frame is designed for managing **Binary Large Object**.  
A few functions work with `class Blob`. Such as `getwebpage()` and `loadbinfile()`.  

##Example Code  
```
blob s=getwebpage("www.google.com");
printf("%s\n",s.text.str.c_str());
```

