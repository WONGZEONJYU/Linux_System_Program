#ifndef _SEM_H_
#define _SEM_H_

#include <sys/sem.h>
#include <string>

union semun 
{
    int val;	
    semid_ds *buf;	
    unsigned short int*array;
    seminfo *__buf;
};

class Sem
{
    void create(const semun&,int,const std::string&,int) noexcept(false);
    void sem_del();
    void _Move(Sem&&);  
    Sem(const Sem&) = delete;
    Sem& operator=(const Sem&) = delete;

public:
    Sem() = default;
    Sem(const semun& ,int = 1,const std::string& = ".",int = 100) noexcept(false);
    Sem(Sem&&) noexcept;
    Sem& operator=(Sem&&) noexcept;
    virtual ~Sem();

    int getval(const int&);
    int getall(semun&) const;
    int setval(const int&,const semun&);
    int setall(const semun&) const;

    int sem_p(const uint16_t&);
    int sem_v(const uint16_t&);

private:
    int m_sem_id_{-1};
    semun m_semun_{};
    static constexpr auto PATHNAME{"."};
    static constexpr auto PROJ_ID{100};
};

#endif
