#ifndef MY_SIGNAL_H
#define MY_SIGNAL_H

#include <tuple>
#include <unordered_map>
#include <memory>
#include <type_traits>
#include <signal.h>
#include <utility>

namespace wtd {

class MySignal
{
    MySignal(const MySignal&) = delete;
    MySignal& operator=(const MySignal&) = delete;

    struct _Base{
        virtual void func() = 0;
        virtual ~_Base() = default;
    };

    using _sp_base_type = std::shared_ptr<_Base>;

    template<typename _Callable>
    struct _Base_impl:public _Base
    {
        _Callable _S_handler;
        _Base_impl(_Callable&& __f):_S_handler(std::forward<_Callable>(__f)){}
        void func() override{
            _S_handler();
        }
    };

    template<typename _Callable>
    static _sp_base_type _S_make_state(_Callable&& __f) {
        using _Impl = _Base_impl<_Callable>;
        return _sp_base_type{new _Impl{std::forward<_Callable>(__f)}};
    }

    static void signal_handler(const int sig,siginfo_t* info,void*) {

        const auto f{sm_map_[sig]};

        if (f){
            f->func();
        }
    }

    template<typename Fn,typename... Args>
    void reg_signal(const int sig,const int flags,
                    Fn&& fn,Args&& ...args) {

        m_act_.sa_sigaction = signal_handler;
        m_act_.sa_flags = flags;

        sigaction (sig, &m_act_,nullptr);

        auto t {__make_invoker(std::forward<Fn>(fn),std::forward<Args>(args)...)};
        auto _sp_t {_S_make_state(t)};
        sm_map_.insert({sig,std::forward<decltype(_sp_t)>(_sp_t)});
    }

public:

    MySignal() = default;

    template<typename Fn,typename... Args>
    explicit MySignal(const int sig,const int flags,
                    Fn&& fn,Args&& ...args):m_sig_(sig){
        reg_signal(sig,flags,std::forward<Fn>(fn),
        std::forward<Args>(args)...);
    }

    ~MySignal(){
        sm_map_.erase(m_sig_);
    }

private:

    template<typename _Tuple>
    struct _Invoker
    {
	    _Tuple _M_t;

        template<typename>
        struct __result;

        template<typename _Fn, typename... _Args>
        struct __result<std::tuple<_Fn, _Args...>> : std::__invoke_result<_Fn, _Args...>{ };

        template<size_t... _Ind>
        typename __result<_Tuple>::type _M_invoke(std::_Index_tuple<_Ind...>) { 
            return std::__invoke(std::get<_Ind>(std::move(_M_t))...); 
        }

        typename __result<_Tuple>::type operator()() {
            using _Indices = typename std::_Build_index_tuple<std::tuple_size<_Tuple>::value>::__type;
            return _M_invoke(_Indices());
        }

    };

    template<typename... _Tp>
    using __decayed_tuple = std::tuple<typename std::decay<_Tp>::type...>;

    template<typename _Callable, typename... _Args>
    static _Invoker<__decayed_tuple<_Callable, _Args...>> __make_invoker(_Callable&& __callable, _Args&&... __args) {
        return { __decayed_tuple<_Callable, _Args...>{std::forward<_Callable>(__callable), std::forward<_Args>(__args)...} };
    }

private:
    const int m_sig_;
    struct sigaction m_act_{};
    static std::unordered_map<int,_sp_base_type> sm_map_;
};

std::unordered_map<int,MySignal::_sp_base_type> MySignal::sm_map_;

}

#endif
