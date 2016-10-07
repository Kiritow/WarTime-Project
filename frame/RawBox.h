/** CPPLIB RawBox
*    RawBox is a base64 based raw data container.
*    RawBox provides toString() method. It can be stored into a MapBox.
*    RawBox provides toRawBox() method. Strings can be parsed to RawBox.
*    Created By Kiritow.
*    Created On: 20161006
*/

#ifndef _KIRITOW_RAWBOX
#define _KIRITOW_RAWBOX

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <string>
#include <sstream>
#include "base64.h"
using namespace std;
using RAWBYTE = unsigned char;

class RawBox
{
	public:
	RawBox()
	{
		data=nullptr;
		sz=0;
	}
	RawBox(const RAWBYTE* pdata,int datasz) : RawBox()
	{
		setdata(pdata,datasz);
	}
	RawBox(RawBox& incBox) : RawBox()
	{
		setdata(incBox.data,incBox.sz);
	}
	~RawBox()
	{
		if(data) free(data);
		data=nullptr;
	}
	int setdata(const RAWBYTE* pdata,int datasz)
	{
		if(data) free(data);
		data=nullptr;
		sz=datasz;
		if(sz<1) return 0;
		data=(RAWBYTE*)malloc(sz);
		if(data==nullptr)
		{
			sz=0;
			/// Out of Memory
			return -1;
		}
		memset(data,0,sz);
		memcpy(data,pdata,sz);
		return 0;
	}
	/// Warning: If you use nocopy, you must asure the memory can be freed by free() because we use 'free()' inside.
	int setdata_nocopy(RAWBYTE* pdata,int datasz)
	{
		free(data);
		data=pdata;
		sz=datasz;
		return 0;
	}
	const RAWBYTE* getdata() const
	{
		return data;
	}
	const int getsize() const
	{
		return sz;
	}
	string toString()
	{
		ostringstream os;
		os<<"RawBox#dtsz#"<<sz<<"#dta#";
		if(data)
		{
			os<<base64_encode(data,sz);
		}
		return os.str();
	}
	RawBox& operator = (RawBox& incBox)
	{
		setdata(incBox.data,incBox.sz);
		return *this;
	}
	RawBox(RawBox&& tinc)
	{
		data=tinc.data;
		sz=tinc.sz;
		tinc.data=nullptr;
		tinc.sz=0;
	}
	RawBox& operator = (RawBox&& tinc)
	{
		if(data)
		{
			free(data);
			data=nullptr;
			sz=0;
		}
		data=tinc.data;
		sz=tinc.sz;
		tinc.data=nullptr;
		tinc.sz=0;
		return *this;
	}
	private:
	RAWBYTE* data;
	int sz;
};

RawBox toRawBox(string rawbox_encoded_str)
{
	RawBox box;
	if(rawbox_encoded_str.find("RawBox#dtsz#")==string::npos)
	{
		/// Head Not Found. Cannot Parse.
		return box;
	}
	char* tp=(char*)(rawbox_encoded_str.c_str());
	char* psz=strstr(tp,"#dtsz#")+6;
	char* pszend=strstr(psz,"#");
	char* tq=strstr(tp,"#dta#")+5;
	thread_local char buff[128];
	memset(buff,0,128);
	strncpy(buff,psz,pszend-psz);
	int tsz;
	sscanf(buff,"%d",&tsz);
	tsz+=32;
	RAWBYTE* nem=(RAWBYTE*)malloc(tsz);
	/// Out of Memory.
	if(nem==nullptr) return box;
	memset(nem,0,tsz);
	int gnx=base64_decode_raw(tq,nem);
	box.setdata_nocopy(nem,gnx);
	/// Success: Return Converted Bytes.
	return box;
}

#endif /// End of _KIRITOW_RAWBOX(rawbox.h)

/**
/// Example Code.
RAWBYTE uu[]={0x20,0x30,0x40,0x50,0x60};

int main()
{
    int sz=sizeof(uu);
    RawBox box(uu,sz);
    printf("%s\n",box.toString().c_str());
    RawBox tbox=toRawBox(box.toString());
    printf("Size %d\n",tbox.getsize());
    for(int i=0;i<tbox.getsize();i++)
    {
        printf("%x ",tbox.getdata()[i]);
    }
}
//*/
