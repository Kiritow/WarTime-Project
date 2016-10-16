#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <cpplib/cpplib#file>
using namespace std;
struct SMTPAuthPack
{
    string uname,upass;
    SMTPAuthPack(string incuname,string incupass) : uname(incuname), upass(incupass){}
};

void _sendmail_fill(char* buffer,const char* SendMailBIN,
                   const char* SenderAddress,const char* ReceiverAddress,
                   const char* SmtpDomain,const char* Username,const char* Password,
                   const char* Subject,const char* FilledContextFile)
{
    sprintf(buffer,"%s -f %s -t %s -s %s -xu %s -xp %s -u \"%s\" < %s",SendMailBIN,
            SenderAddress,ReceiverAddress,SmtpDomain,Username,Password,Subject,
            FilledContextFile);
}
int _sendmail_real(const char* CommandBuffer)
{
    return system(CommandBuffer);
}
int sendmail(string SenderMail,string ReceiverMail,string SMTPServer,
             SMTPAuthPack pack,string Subject,string Context)
{
    char* commandstr=(char*)malloc(4096);
    if(commandstr==NULL) return -1;
    memset(commandstr,0,4096);

    autotmp tmp;
    string tfile=tmp.name();
    FILE* fp=fopen(tfile.c_str(),"w");
    if(fp==NULL)
    {
        free(commandstr);
        return -2;
    }
    fprintf(fp,"%s\n",Context.c_str());
    fclose(fp);
    _sendmail_fill(commandstr,"C:\\mysendmail.exe",SenderMail.c_str(),ReceiverMail.c_str(),
                   SMTPServer.c_str(),pack.uname.c_str(),pack.upass.c_str(),Subject.c_str(),tfile.c_str());
    int callret=_sendmail_real(commandstr);
    free(commandstr);
    return callret;
}
