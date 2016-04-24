[Frame](#frame) | [Internationalization Frame](#internationalization-frame) | [Asynchronous Call Frame](#asynchronous-call-frame) | [Any Class Frame](#any-class-frame)  
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

#Asynchronous Call Frame  
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
can not cast int to std::string
Cannot cast.
String:Hello
```

