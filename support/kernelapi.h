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
    public:
    userinfo();
    ~userinfo();
    int getid();
    const char* getusername();
    const char* getuuid();
    private:
    int id;
    char* username;
    char* uuid;
};

///Login Checker and fetch user info.
userinfo* kernel_getuserinfo(const char* EncodedUsername,const char* EncodedPassword);

///Encode secret information
char* kernel_encoder(const char* RawString,char* TargetString,int buffsize);

///Send message to the server
int kernel_sendmsg(const char* Message);

///Check unread message on server
int kernel_checkmsg();

///Receive message from server
int kernel_recvmsg(char* targetbuff,int buffsize);

///Update main module
int kernel_updateself(const char* UpdateFile);

///Push an external module into internal memory
int kernel_pushfile(const char* FileToPush,const char* TargetFilePosition);

#endif /*End of WARTIME_HEAD_KERNEL_API */
