#pragma once
#include <memory>
#include <functional>
#include <vector>
#include <string>
#include "Noncopyable.h"
#include "address.h"
#include "socket.h"
#include "iomanager.h"
namespace tw{
class TcpServer : public std::enable_shared_from_this<TcpServer>
                    , Noncopyable{
public:
    using ptr = std::shared_ptr<TcpServer>;
    TcpServer(tw::IOManager* worker = tw::IOManager::GetThis()
        , tw::IOManager* accept_worker = tw::IOManager::GetThis());
    virtual ~TcpServer();
    virtual bool bind(Address::ptr addr);
    virtual bool bind(const std::vector<Address::ptr>& addrs,
        std::vector<Address::ptr>& fails);
    virtual bool start();
    virtual void stop();
    uint64_t getRecvTimeout() const {return m_recvTimeout;}
    std::string getName() const {return m_name;}
    void setName(const std::string& v) {m_name = v;}
    bool isStop() const {return m_isStop;}
protected:
    virtual void handleClient(Socket::ptr client);
    virtual void startAccept(Socket::ptr sock);
private:
    std::vector<Socket::ptr> m_socks;
    IOManager* m_worker;
    IOManager* m_acceptWorker;
    uint64_t m_recvTimeout;
    std::string m_name;
    bool m_isStop;
};
}