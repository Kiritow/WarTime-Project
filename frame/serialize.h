#include <sstream>
#include <cpplib/cpplib#gsock>
using namespace std;

/** Templates */
template<typename T>
string serialize(const T& a)
{
    return T::serialize(a);
}
template<typename T>
int deserialize(string str,T& a)
{
    return T::deserialize(str,a);
}

/** Special Version
* For...
*   int, double, float
*/
/// int
template<>
string serialize(const int& a)
{
    string ans;
    int c=htonl(a);
    ans.append((const char*)&c,sizeof(c));
    return ans;
}
template<>
int deserialize(string str,int& c)
{
    memcpy(&c,str.data(),sizeof(c));
    c=ntohl(c);
    return sizeof(c);
}
/// string
template<>
string serialize(const string& a)
{
    int len=a.size();
    string ans;
    ans.append(::serialize(len));
    ans.append(a);
    return ans;
}
template<>
int deserialize(string str,string& a)
{
    int len;
    ::deserialize(str,len);
    a=str.substr(sizeof(len),len);
    return sizeof(int)+len;
}

/// Marco definition
#define NORMAL_DATA_SERIALIZE(Type) template<> \
    string serialize(const Type& a) \
    { \
        string ans; \
        ans.append((const char*)&a,sizeof(a)); \
        return ans; \
    }
#define NORMAL_DATA_DESERIALIZE(Type) template<> \
    int deserialize(string str,Type& a)\
    { \
        memcpy(&a,str.data(),sizeof(a)); \
        return sizeof(a); \
    }

/// double
NORMAL_DATA_SERIALIZE(double);
NORMAL_DATA_DESERIALIZE(double);
NORMAL_DATA_SERIALIZE(float);
NORMAL_DATA_DESERIALIZE(float);
NORMAL_DATA_SERIALIZE(char);
NORMAL_DATA_DESERIALIZE(char);


template<typename SerializableType>
class Serializable
{
public:
    static SerializableType deserialize(string);
    static string serialize(const SerializableType& a);
};

class OutEngine
{
public:
    template<typename SerializableType>
    OutEngine& operator << (SerializableType& a)
    {
        string x=::serialize(a);
        os.write(x.data(),x.size());
        return *this;
    }
    string str()
    {
        return os.str();
    }
    void set_empty()
    {
        os.str("");
    }
    OutEngine():os(std::ios::binary){}
public:
    ostringstream os;
};

class InEngine
{
public:
    InEngine(string s) : is(s){n_size=leftsize();}
    template<typename SerializableType>
    InEngine& operator >> (SerializableType& a)
    {
        int ret=::deserialize(is,a);
        is=is.substr(ret);
        return *this;
    }
    void set_str(string s)
    {
        is=s;
        n_size=leftsize();
    }

    int leftsize()
    {
        return is.size();
    }
    int donesize()
    {
        return n_size-leftsize();
    }

protected:
    string is;
    int n_size;
};
