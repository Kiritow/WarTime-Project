#include "MapBox.h"
#include <cpplib/cpplib#gsock>
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
using namespace std;

class MapBoxPack
{
public:
    MapBox box;
    int rxid;
    int txid;
};

/** MapBoxBus基于MapBox,完全兼容MapBox.
*   这意味着你可以使用MapBox接受由MapBoxBus发来的数据.
*/
class MapBoxBus
{
public:
	MapBoxBus();
	MapBoxBus(sock* psock,bool need_delete);
	~MapBoxBus();
	///提交数据包. 若设置replyto则表明本数据包回复ID为replyto的数据包.
	int submit(MapBox& mpbox, int* replyto=nullptr);
	///获取对responseid回复的数据包. 状态存储在pret中,远端数据包ID存储在remoteid
	MapBox get(int responseid, int* pret, int* remoteid=nullptr);
	///停止运行
	int stop();
	///开始运行
	int beginwith(sock* psock,bool need_delete);
	bool isRunning();
private:
    queue<MapBox> SendBus;
    vector<MapBoxPack> RecvBus;
    sock* ps;
    bool _internal_sock_need_delete;

    mutex SendBusLock;
    unique_lock<mutex>* pSendLock;
    condition_variable SendCond;

    mutex RecvBusLock;

    mutex QueryLock;

    mutex StatusLock;
    bool stop_lock;

    thread* SendThread;
    thread* RecvThread;

    friend void procSendThread(MapBoxBus*);
    friend void procRecvThread(MapBoxBus*);
};

MapBoxBus::MapBoxBus()
{
    ps=nullptr;
    _internal_sock_need_delete=false;
    stop_lock=false;
    pSendLock=nullptr;
    SendThread=nullptr;
    RecvThread=nullptr;
}

MapBoxBus::MapBoxBus(sock* psock,bool need_delete) : MapBoxBus()
{
    ps=psock;
    _internal_sock_need_delete=need_delete;
    stop_lock=false;
    pSendLock=new unique_lock<mutex>(SendBusLock);
}

MapBoxBus::~MapBoxBus()
{

}

void procSendThread(MapBoxBus* pBus)
{

}

void procRecvThread(MapBoxBus* pBus)
{

}
