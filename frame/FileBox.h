#include <wartime/frame/MapBox.h>
#include <wartime/frame/RawBox.h>
#include <memory>
using namespace std;

string toString(int size)
{
	ostringstream os;
	os<<size;
	return os.str();
}
int toInt(string s)
{
	int size;
	istringstream is(s);
	is>>size;
	return size;
}

class FileBox
{
	public:
	FileBox(){}
	FileBox(string incname){setname(incname);}
	
	void setname(string incname){name=incname;}
	string getname(){return name;}
	private:
	string name;
};

template<>
int sock::send(FileBox& idx)
{
	MapBox box;
	string fn=idx.getname();
	const char* fns=fn.c_str();
	FILE* fp=fopen(fns,"rb");
	if(fp==NULL)
	{
		return -1;
	}
	fseek(fp,0L,SEEK_END);
	int fsz=ftell(fp);
	rewind(fp);
	box.add("_MAPBOX_PROTOCOL","FILE_TRANSFER");
	box.add("FILE_SIZE",toString(fsz));
	box.add("FILE_NAME",fn);
	int ret=send(box);
	if(ret<=0)
	{
		fclose(fp);
		return -2;
	}
	ret=recv(box);
	if(ret<=0)
	{
		fclose(fp);
		return -3;
	}
	if(!box.haskey("_MAPBOX_PROTOCOL")||!box.haskey("RESULT"))
	{
		fclose(fp);
		return -4;
	}
	if(box["_MAPBOX_PROTOCOL"]!="FILE_TRANSFER"||box["RESULT"]!="OK")
	{
		fclose(fp);
		/// Transfer Request Denied.
		return -5;
	}
	thread_local char buffer[2048];
	int total=fsz;
	int sent=0;
	int blocksz=1024*1024;
	int singleblocksz=1024;
	while(sent<total)
	{
		if((total-sent)>=blocksz)
		{
			string ans;
			for(int i=0;i<1024;i++)
			{
				memset(buffer,0,2048);
				fread(buffer,1,1024,fp);
				ans+=base64_encode((unsigned char*)buffer,1024);
			}
			box.clear();
			box.add("DATA",ans);
			ret=send(box);
			if(ret<=0)
			{
				fclose(fp);
				return -6;
			}
			sent+=blocksz;
		}
		else
		{
			int groupsz=(total-sent)/singleblocksz;
			string ans;
			for(int i=0;i<groupsz;i++)
			{
				memset(buffer,0,2048);
				fread(buffer,1,1024,fp);
				ans+=base64_encode((unsigned char*)buffer,1024);
			}
			if((total-sent)%groupsz!=0)
			{
				memset(buffer,0,2048);
				int cret=fread(buffer,1,1024,fp);
				ans+=base64_encode((unsigned char*)buffer,cret);
			}
			box.clear();
			box.add("DATA",ans);
			box.add("FIN","YES");
			ret=send(box);
			if(ret<=0)
			{
				fclose(fp);
				return -7;
			}
			sent=total;
		}
	}
	fclose(fp);
	ret=recv(box);
	if(ret<0)
	{
		return -8;
	}
	if(!box.haskey("RESULT")||box["RESULT"]!="OK")
	{
		return -9;
	}
	return sent;
}

/// Receive File To idx.name
template<>
int sock::recv(FileBox& idx)
{
	int tlcsz=1024*1024+256;
	unsigned char* uubuff=(unsigned char*)malloc(tlcsz);
	if(uubuff==NULL)
	{
		return -1;
	}
	memset(uubuff,0,tlcsz);
	
	MapBox box;
	int ret=recv(box);
	if(ret<=0) return ret;
	if(box["_MAPBOX_PROTOCOL"]!="FILE_TRANSFER"||!box.haskey("FILE_NAME")||!box.haskey("FILE_SIZE"))
	{
		return -4;
	}
	int sz=toInt(box["FILE_SIZE"]);
	/// REMOTE FILE NAME IGNORED.
	string localname=idx.getname();
	const char* localfile=localname.c_str();
	FILE* fp=fopen(localfile,"wb");
	if(fp==NULL) return -5;
	int zerototal=sz;
	int zerodone=0;
	thread_local char buff[2048];
	memset(buff,0,2048);
	int error_flag=0;
	while(zerodone<zerototal)
	{
		int ret=fwrite(buff,1,zerototal-zerodone>=1024?1024:zerototal-zerodone,fp);
		if(ret<0)
		{
			error_flag=1;break;
		}
		zerodone+=ret;
	}
	if(error_flag)
	{
		fclose(fp);
		unlink(localfile);
		box.clear();
		box.add("_MAPBOX_PROTOCOL","FILE_TRANSFER");
		box.add("RESULT","ERR_NO_MORE_DISK");
		ret=send(box);
		if(ret<=0) return -8;
		return -9;
	}
	rewind(fp);
	box.clear();
	box.add("_MAPBOX_PROTOCOL","FILE_TRANSFER");
	box.add("RESULT","OK");
	ret=send(box);
	if(ret<=0) return -10;
	while(1)
	{
		ret=recv(box);
		if(ret<=0) return -11;
		if(!box.haskey("DATA"))
		{
			/// Broken Box
			return -12;
		}
		string ens=box["DATA"];
		int cdssz=base64_decode_raw(ens,uubuff);
		fwrite(uubuff,1,cdssz,fp);
		if(box["FIN"]=="YES") break;
	}
	fclose(fp);
	box.clear();
	box.add("RESULT","OK");
	ret=send(box);
	if(ret<0) return -15;
	
	return zerototal;
}
