#include <cstring>
#include "mysignal.hpp"

namespace wtd {

    MySignal::MySignal(const int sig,const int flags):m_sig_{sig}{

        m_act_.sa_sigaction = signal_handler;
        m_act_.sa_flags = SA_SIGINFO | flags;
        sigaction (sig, &m_act_,nullptr);
    }

    int MySignal::sig(const int index){

        if (sm_map_.find(index) != sm_map_.end()){
            return sm_map_[index]->m_sig_;
        }
        return -1;
    }

    int MySignal::sig() const{
        return m_sig_;
    }

    siginfo_t MySignal::siginfo(const int index){

        if (sm_map_.find(index) != sm_map_.end()){
            return sm_map_[index]->m_info_;
        }
        return {};
    }

    siginfo_t MySignal::siginfo() const{
        return m_info_;
    }

    void MySignal::signal_handler(const int sig,siginfo_t* info,void*) {

        if (sm_map_.find(sig) != sm_map_.end()) {

            auto _t {sm_map_[sig]};

            if (_t) {
                _t->m_sig_ = sig;
                _t->m_info_ = *info;

                auto __t {_t->m_hander_};

                if (__t){
                    __t->func();
                }
            }
        }
    }
}

