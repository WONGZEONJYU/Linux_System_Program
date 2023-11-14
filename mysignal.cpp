#include "mysignal.hpp"
#include <memory>
#include <cstring>
#include <iostream>

namespace wtd {

    std::map<int,MySignal::_sp_MySignal_type> MySignal::sm_map_;

    void MySignal::reg_signal(const int sig,const int flags) {

        m_act_.sa_sigaction = signal_handler;
        m_act_.sa_flags = SA_SIGINFO | flags;
        sigaction (sig, &m_act_,nullptr);
    }

    void MySignal::SWap(MySignal&& obj){
        m_sig_ = obj.m_sig_;
        m_act_ = obj.m_act_;
        m_info_ = obj.m_info_;
        m_hander_ = std::move(obj.m_hander_);
        obj.m_sig_ = 0;
        memset(&obj.m_act_,0,sizeof(obj.m_act_));
        memset(&obj.m_info_,0,sizeof(obj.m_info_));
    }

    MySignal::~MySignal(){
        if (m_sig_){
            std::cout << m_hander_.get() << "\n";
            sm_map_.erase(m_sig_);
        }
    }

    MySignal::MySignal(MySignal&& obj){
        SWap(std::move(obj));
    }

    MySignal& MySignal::operator=(MySignal&& obj) {
        if (std::addressof(obj) != this) {
            SWap(std::move(obj));
        }
        return *this;
    }

    int MySignal::sig(int index){

        if (sm_map_.find(index) != sm_map_.end()){
            return sm_map_[index]->m_sig_;
        }
        return -1;
    }

    int MySignal::sig(){
        return m_sig_;
    }

    siginfo_t MySignal::siginfo(int index){

        if (sm_map_.find(index) != sm_map_.end()){
            return sm_map_[index]->m_info_;
        }
        return {};
    }

    siginfo_t MySignal::siginfo(){
        return m_info_;
    }

    void MySignal::signal_handler(const int sig,siginfo_t* info,void*) {

        if (sm_map_.find(sig) != sm_map_.end()){

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

    void MySignal::signal_proxy(void* ){
        
    }
}
