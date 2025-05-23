#ifndef X_SIGNAL_HPP
#define X_SIGNAL_HPP 1

#include "xabstractsignal.hpp"

namespace xtd {

    class XSignal;
    using Signal_Ptr = std::shared_ptr<XSignal>;

    class XSignal : public XAbstractSignal {
        SIG_DISABLE_COPY_MOVE(XSignal)

        static Signal_Ptr create(const int &,const int &);

    protected:
        explicit XSignal() = default;

    public:
        template<typename Fn,typename... Args>
        inline static auto Register(const int &sig,const int &flags,
                            Fn&& fn,Args&& ...args){
            const auto obj{create(sig,flags)};
            if (obj){
                obj->Callable_join(std::forward<Fn>(fn),std::forward<Args>(args)...);
            }
            return obj;
        }

        static bool Send_signal(const int &pid_,const int &sig_,const sigval &val_);
        static siginfo_t siginfo(const int&sig);
        static void Unregister(const int &sig);
        ~XSignal() override = default;
    };

    template<typename Fn,typename... Args>
    static inline auto Signal_Register(const int &sig,const int &flags,
        Fn&& fn,Args&& ...args){
        return XSignal::Register(sig,flags,std::forward<Fn>(fn),std::forward<Args>(args)...);
    }

    static inline void Signal_Unregister(const int &sig){
        XSignal::Unregister(sig);
    }

}

#endif
