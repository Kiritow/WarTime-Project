#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>

using namespace std;

/** Declarations */
char* support_getmacaddress(char* TargetBuffer,int BufferSize);
int kernel_sendmsg(const char* Message);


namespace _wartime_buildin_namespace
{
    const unsigned int _default_device_macx_size = 32 ;
    const char _default_macdecoder_char = ':';
    struct device
    {
        string macx;
    };
    string _basic_MacAddressEncoder(string OriginalMacAddress)
    {
        thread_local char buff[32];
        memset(buff,0,32);
        buff[0]=OriginalMacAddress.at(0);
        int lastValue=(buff[0]>='0'&&buff[0]<='9')?(buff[0]-'0'):(buff[0]>='A'&&buff[0]<='F')?(buff[0]-'A'+10):(buff[0]-'a'+10);
        for(size_t i=1;i<OriginalMacAddress.size();i++)
        {
            char incBit=OriginalMacAddress.at(i);
            int incValue=(incBit>='0'&&incBit<='9')?(incBit-'0'):(incBit>='A'&&incBit<='F')?(incBit-'A'+10):(incBit-'a'+10);
            int newValue=incValue+lastValue;
            if(newValue>15) newValue-=16;
            buff[i]=(newValue<10)?(newValue+'0'):(newValue-10+'A');
            lastValue=incValue;
        }
        return string(buff);
    }
    string _basic_MacAddressDecoder(string EncodedMacAddress)
    {
        thread_local char buff[32];
        memset(buff,0,32);
        buff[0]=EncodedMacAddress.at(0);
        int lastValue=(buff[0]>='0'&&buff[0]<='9')?(buff[0]-'0'):(buff[0]>='A'&&buff[0]<='F')?(buff[0]-'A'+10):(buff[0]-'a'+10);
        for(size_t i=1;i<EncodedMacAddress.size();i++)
        {
            char incBit=EncodedMacAddress.at(i);
            int incValue=(incBit>='0'&&incBit<='9')?(incBit-'0'):(incBit>='A'&&incBit<='F')?(incBit-'A'+10):(incBit-'a'+10);
            int newValue=incValue-lastValue;
            if(newValue<0) newValue+=16;
            buff[i]=(newValue<10)?(newValue+'0'):(newValue-10+'A');
            lastValue=newValue;
        }
        return string(buff);
    }

    string MacAddressEncoder(string MacAddress)
    {
        string mac;
        for(auto i:MacAddress)
        {
            if((i>='0'&&i<='9')||(i>='A'&&i<='F')||(i>='a'&&i<='f'))
            {
                mac.push_back(i);
            }
        }
        mac=_basic_MacAddressEncoder(mac);
        string newmac;

        int cnt=0;
        for(auto i:mac)
        {
            if(cnt==2)
            {
                cnt=0;
                newmac.push_back(_default_macdecoder_char);
                newmac.push_back(i);
                cnt++;
            }
            else
            {
                cnt++;
                newmac.push_back(i);
            }
        }
        return newmac;
    }
    string MacAddressDecoder(string MacAddressX)
    {
        string mac;
        for(auto i:MacAddressX)
        {
            if((i>='0'&&i<='9')||(i>='A'&&i<='F')||(i>='a'&&i<='f'))
            {
                mac.push_back(i);
            }
        }
        mac=_basic_MacAddressDecoder(mac);
        string newmac;
        int cnt=0;
        for(auto i:mac)
        {
            if(cnt==2)
            {
                cnt=0;
                newmac.push_back(_default_macdecoder_char);
                newmac.push_back(i);
                cnt++;
            }
            else
            {
                cnt++;
                newmac.push_back(i);
            }
        }
        return newmac;
    }

    device getLocalDevice()
    {
        device dev;
        thread_local char buffer[32];
        dev.macx=MacAddressEncoder(string(support_getmacaddress(buffer,32)));
        return dev;
    }
}

using DEVICEINFO = _wartime_buildin_namespace::device;

int SendMessage(DEVICEINFO Target,string Message)
{
    char* message=new char[Message.size()+512];
    if(message==nullptr) return -1; /// Out of Memory
    memset(message,0,Message.size()+512);
    snprintf(message,Message.size()+512,"TYPE=TEXT;FROM=%s;MSG=%s;OVER;;",Target.macx.c_str(),Message.c_str());
    kernel_sendmsg(message);
    delete[] message;
    return 0;
}

int SendRawMessage(DEVICEINFO Target,const void* RawData,size_t Size)
{
    char* message=new char[Size+512];
    if(message==nullptr) return -1; /// Out of Memory
    memset(message,0,Size+512);
    sprintf(message,"TYPE=BIN;FROM=%s;MSG=",Target.macx.c_str());
    int cache=strlen(message);
    memcpy(message+cache,RawData,Size);
    memcpy(message+cache+Size,";OVER;;",strlen(";OVER;;"));
    kernel_sendmsg(message);
    delete[] message;
    return 0;
}
