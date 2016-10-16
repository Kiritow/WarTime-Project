#include "miracl/big.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <sstream>

using namespace std;

Miracl precision(10000,2);
typedef struct
{
    string n,e;
} PublicKey;

typedef struct
{
    string n,d;
} PrivateKey;

void KeyGen(PublicKey& pub,PrivateKey& pri)
{
    irand(time(NULL));
    int bits;
    Big n,e,d;
    bits=1024;

    int half = bits/2;
    Big p,q,fn,tmp;
    tmp = rand(half,2);
    p = nextprime(tmp);
    tmp = rand(half,2);
    q = nextprime(tmp);
    n = p*q;
    fn = (p-1)*(q-1);
    do
    {
        e = rand(16,2);
        e = nextprime(e);
        if(gcd(e,fn)==1)
            break;
    }
    while(true);
    d = inverse(e,fn);

    {
        ostringstream os;
        os<<e;
        pub.e=os.str();
    }

    {
        ostringstream os;
        os<<n;
        pub.n=os.str();
        pri.n=os.str();
    }

    {
        ostringstream os;
        os<<d;
        pri.d=os.str();
    }
}

string RSA_Encode_basic(const PublicKey& pubkey,string num_code)
{
    Big tmp((char*)num_code.c_str());
    cout<<"You are going to encode: "<<tmp<<endl;
    Big e((char*)pubkey.e.c_str());
    Big n((char*)pubkey.n.c_str());
    tmp = pow(tmp,e,n);
    ostringstream ostr;
    ostr<<tmp;
    return ostr.str();
}
string RSA_Decode_basic(const PrivateKey& prikey,string encoded_num_code)
{
    Big tmp((char*)encoded_num_code.c_str());
    cout<<"You are going to decode: "<<tmp<<endl;
    Big d((char*)prikey.d.c_str());
    Big n((char*)prikey.n.c_str());
    tmp = pow(tmp,d,n);
    ostringstream ostr;
    ostr<<tmp;
    return ostr.str();
}
const char* input=
    "19201365997978672846726933993428540364131938371455436620802788819655375";

int main()
{
    PublicKey a;
    PrivateKey b;
    KeyGen(a,b);
    printf("E: %s\nN: %s\n",a.e.c_str(),a.n.c_str());
    printf("D: %s\nN: %s\n",b.d.c_str(),b.n.c_str());
    string source(input);
    string x=RSA_Encode_basic(a,source);
    printf("Encoded: %s\n",x.c_str());
    string tans=RSA_Decode_basic(b,x);
    printf("Decoded: %s\n",tans.c_str());
}
