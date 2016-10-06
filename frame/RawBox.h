/** CPPLIB RawBox
*    RawBox is a base64 based raw data container.
*    RawBox provides toString() method. It can be stored into a MapBox.
*    RawBox provides toRawBox() method. Strings can be parsed to RawBox.
*    Created By Kiritow.
*    Created On: 20161006
*/

#ifndef _KIRITOW_RAWBOX
#define _KIRITOW_RAWBOX

#include <string>
#include <sstream>
#include "base64_raw.h"
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
	RawBox(const void* pdata,int datasz)
	{
		setdata(pdata,datasz);
	}
	~RawBox()
	{
		if(data) free(data);
		data=nullptr;
	}
	int setdata(const void* pdata,int datasz)
	{
		if(data) free(data);
		data=nullptr;
		sz=datasz;
		data=(RAWBYTE*)malloc(sz);
		if(data==nullptr)
		{
			sz=0;
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
	RawBox(RawBox& incBox)
	{
		setdata(incBox.data,incBox.sz);
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
	char* psz=strstr(tp,"dtsz#")+5;
	char* pszend=strstr(psz,"#");
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
	int gnx=base64_decode_raw(rawbox_encoded_str,nem);
	box.setdata_nocopy(nem,gnx);
	/// Success.
	return box;
}

#endif /// End of _KIRITOW_RAWBOX(rawbox.h)

