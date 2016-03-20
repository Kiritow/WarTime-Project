/** WarTime-Project On GitHub
*   https://github.com/Kiritow/WarTime-Project.git
*/

/** Kernel APIs (C++)
*   Provider: Game Harbor (HC TECH)
*/

#ifndef WARTIME_HEAD_KERNEL_API
#define WARTIME_HEAD_KERNEL_API

///User Info Structure
struct userinfo
{
    int id;
    char* username;
    char* uuid;
    userinfo();
    ~userinfo();
};

///Login Checker and fetch user info.
userinfo* kernel_getuserinfo(const char* EncodedUsername,const char* EncodedPassword);

///Encode secret information
char* kernel_encoder(const char* RawString,char* TargetString,int buffsize);

///Send message to the server
int kernel_sendmsg(const char* Message);

///Check unread message on server
int kernel_checkmessage();

///Receive message from server
int kernel_recvmsg(char* targetbuff,int buffsize);

#endif /*End of WARTIME_HEAD_KERNEL_API */
