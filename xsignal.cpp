#include <unistd.h>
#include <string>
#include <sstream>
#include "xsignal.hpp"

namespace xtd {

    XSignal::XSignal(const int &sig,const int &flags):m_sig_{sig}{
        m_act_.sa_sigaction = signal_handler;
        m_act_.sa_flags = SA_SIGINFO | flags;
        sigaction(sig, &m_act_,nullptr);
    }

    int XSignal::sig(const int &sig){
        const auto &res{sm_map_.find(sig)};
        return res != sm_map_.end() ?
            res->second->m_sig_ : -1;
    }

    siginfo_t XSignal::siginfo(const int &sig){
        const auto &res{sm_map_.find(sig)};
        return res != sm_map_.end() ? res->second->m_info_ : siginfo_t{};
    }

    void XSignal::signal_handler(const int sig,siginfo_t* const info,void* const ctx) {
        
        const auto &res{sm_map_.find(sig)};
        if (sm_map_.end() == res || !res->second || !res->second->m_call_){
            std::stringstream msg;
            msg << "This signal(id: " << sig << ")" << "is not registered\n";
            write(STDERR_FILENO,msg.str().c_str(),msg.str().length());
            return;
        }
        res->second->m_context_ = ctx;
        res->second->m_sig_ = sig;
        res->second->m_info_ = *info;
        res->second->m_call_->func();
    }

    XSignal::~XSignal(){
        if (sm_map_.contains(m_sig_)){
            sm_map_.erase(m_sig_);
        }
    }
}
