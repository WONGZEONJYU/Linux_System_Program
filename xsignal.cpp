#include "xsignal.hpp"
#include <unistd.h>
#include <string>
#include <sstream>
#include <unordered_map>

namespace xtd {

    class XSignal_impl final: public XSignal {

        static void signal_handler(const int sig,siginfo_t* const info,void* const ctx) {

            const auto &res{sm_map_.find(sig)};
            if (sm_map_.end() == res || !res->second || !res->second->m_call_){
                std::stringstream msg{};
                msg << "This signal(id: " << sig << ")" << "is not registered\n";
                write(STDERR_FILENO,msg.str().c_str(),msg.str().length());
                return;
            }

            res->second->m_context_ = ctx;
            res->second->m_sig_ = sig;
            res->second->m_info_ = *info;
            res->second->m_call_->func();
        }

        void Unregister_helper() {
            if (m_sig_ > 0) {
                m_act_.sa_handler = SIG_DFL;
                m_act_.sa_flags = 0;
                sigaction(m_sig_, &m_act_,{});
                m_sig_ = -1;
            }
        }

    public:
        using XSignal_Impl_Ptr = std::shared_ptr<XSignal_impl>;

        int sig() const & override {
            return m_sig_;
        }

        const siginfo_t &siginfo() const & override{
            return m_info_;
        }

        static auto siginfo(const int &sig) {
            const auto it{sm_map_.find(sig)};
            return  sm_map_.end() != it ? it->second->m_info_ : siginfo_t{};
        }

        ucontext_t * context() const & override {
            return static_cast<ucontext_t*>(m_context_);
        }

        void Unregister() override {
            Unregister_helper();
            sm_map_.erase(m_sig_);
        }

        static void Unregister(const int &sig){
            if (const auto it{sm_map_.find(sig)};sm_map_.end() != it){
                it->second->Unregister();
            }
        }

    private:
        using call_map_t = std::unordered_map<int,XSignal_Impl_Ptr>;
        friend class XSignal;
        static inline call_map_t sm_map_{};
        int m_sig_{-1};
        struct sigaction m_act_{};
        siginfo_t m_info_{};
        void *m_context_{};

    public:
        explicit XSignal_impl(const int &sig,const int &flags):m_sig_(sig){
            m_act_.sa_sigaction = signal_handler;
            m_act_.sa_flags = SA_SIGINFO | flags;
            sigaction(sig, &m_act_,{});
        }

        XSignal_impl(const XSignal_impl &) = delete;
        XSignal_impl(XSignal_impl &&) = delete;
        XSignal_impl& operator=(const XSignal_impl &) = delete;
        XSignal_impl& operator=(XSignal_impl &&) = delete;

        ~XSignal_impl() override{
            Unregister_helper();
        }
    };

    siginfo_t XSignal::siginfo(const int &sig){
        return XSignal_impl::siginfo(sig);
    }

    void XSignal::Unregister(const int &sig){
        XSignal_impl::Unregister(sig);
    }

    bool XSignal::Send_signal(const int &pid_,const int &sig_,const sigval &val_){
        return !sigqueue(pid_,sig_,val_);
    }

    Signal_Ptr XSignal::create(const int &sig,const int &flags){
        try{
            const auto obj{std::make_shared<XSignal_impl>(sig,flags)};
            obj->sm_map_[sig] = obj;
            return obj;
        }catch (const std::exception &){
            return {};
        }
    }
}
