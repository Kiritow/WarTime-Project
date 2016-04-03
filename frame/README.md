#Frame  
This directory contains the main frame of WarTime.  
Get access to frame by `#include <frame/frame.hpp>`  
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
