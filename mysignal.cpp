#include <unistd.h>
#include <string>
#include <sstream>
#include "mysignal.hpp"

namespace wtd {

    MySignal::MySignal(const int &sig,const int &flags):m_sig_{sig}{
        m_act_.sa_sigaction = signal_handler;
        m_act_.sa_flags = SA_SIGINFO | flags;
        sigaction(sig, &m_act_,nullptr);
    }

    int MySignal::sig(const int &sig){
        const auto res{sm_map_.find(sig)};
        return res != sm_map_.end() ? 
            res->second->m_sig_ : -1;
    }

    siginfo_t MySignal::siginfo(const int &sig){
        const auto res{sm_map_.find(sig)};
        return res != sm_map_.end() ? res->second->m_info_ : siginfo_t{};
    }

    void MySignal::Unregister(){
        m_act_.sa_handler = SIG_DFL;
        m_act_.sa_flags = 0;
        sigaction(m_sig_, &m_act_,nullptr);
        sm_map_.erase(m_sig_);
    }

    void MySignal::signal_handler(const int sig,siginfo_t* const info,void*ctx) {
        
        const auto res{sm_map_.find(sig)};

        if (sm_map_.end() == res || !res->second || !res->second->m_hander_){
            std::stringstream msg;
            msg << "This signal(id: " << sig << ")" << "is not registered\n";
            write(STDERR_FILENO, msg.str().c_str(),msg.str().length());
            return;
        }

        res->second->m_context_ = ctx;
        res->second->m_sig_ = sig;
        res->second->m_info_ = *info;
        res->second->m_hander_->func();
    }
}
