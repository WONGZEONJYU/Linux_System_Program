#include <sys/types.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <iostream>
#include <algorithm>
#include "sem_.hpp"
#include <mutex>

using namespace std;

void Sem::_Move(Sem&& obj)
{
    m_sem_id_ = obj.m_sem_id_;
    m_semun_ = obj.m_semun_;
    obj.m_sem_id_ = -1;
    fill_n(reinterpret_cast<char*>(addressof(obj.m_semun_)),sizeof(obj.m_semun_),0);
}

Sem::Sem(const semun& _s,const int nsems,const std::string& pathname,const int _id) noexcept(false)
{
    create(_s, nsems, pathname,_id);
}

Sem::Sem(Sem&& obj) noexcept
{
    _Move(forward<decltype(obj)>(obj));
}

Sem& Sem::operator=(Sem&& obj) noexcept
{
    if (this != addressof(obj)){
        _Move(forward<decltype(obj)>(obj));
    }
    return *this;
}

Sem::~Sem()
{
    sem_del();
}

int Sem::getval(const int& semnum)
{
    return semctl(m_sem_id_,semnum,GETVAL);
}

int Sem::getall(semun& _s) const
{
    return semctl(m_sem_id_,0,GETALL,_s);
}

int Sem::setval(const int& semnum,const semun& _s)
{
    return semctl(m_sem_id_,semnum,SETVAL,_s);
}

int Sem::setall(const semun& _s) const
{
    return semctl(m_sem_id_,0,SETALL,_s);
}

int Sem::sem_p(const uint16_t& nsems)
{
    sembuf sops{.sem_num = nsems,.sem_op = -1,.sem_flg = SEM_UNDO};
    const auto ret{ semop(m_sem_id_,&sops,1)};
    if (ret < 0){
        cerr << "[ERROR]" << __FUNCTION__ << "\n" << 
            "errno : " << errno << "\n"; 
    }
    return ret;
}

int Sem::sem_v(const uint16_t& nsems)
{
    sembuf sops{.sem_num = nsems,.sem_op = 1,.sem_flg = SEM_UNDO};
    const auto ret{ semop(m_sem_id_,&sops,1)};
    if (ret < 0){
        cerr << "[ERROR]" << __FUNCTION__ << "\n" << 
                "errno : " << errno << "\n"; 
    }
    return ret;
}

void Sem::create(const semun& _s,int nsems,const string& pathname,int proj_id) noexcept(false)
{
    proj_id = (proj_id <= 0)? PROJ_ID : proj_id;
    nsems = (nsems <= 0) ? 1 : nsems;
    const auto key {pathname.empty()? IPC_PRIVATE : ftok(pathname.c_str(),proj_id)};
    if (key < 0){
        cerr << errno << "\n";
        throw string("[ERROR] ftok(...)\n");
    }

    m_sem_id_ = semget(key,nsems,IPC_CREAT | 0666 );
    if (m_sem_id_ < 0){
        cerr << errno << "\n";
        throw string("[ERROR] semget(...)\n");
    }

    if (semctl(m_sem_id_,0,SETALL,_s) < 0){
        cerr << errno << "\n";
        throw string("[ERROR] semctl(...)\n");
    }
}

void Sem::sem_del()
{
    if (m_sem_id_ < 0){
        return;
    }

    if (semctl(m_sem_id_,0,IPC_RMID,nullptr) < 0){
        cerr << "[ERROR] semctl(...)\n";
    }
}
