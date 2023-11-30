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
    struct _Base_impl:public _Base {
        _Callable _S_handler;
        _Base_impl(_Callable&& __f):_S_handler(std::forward<_Callable>(__f)){}
        void func() override {
            _S_handler();
        }
    };

    template<typename _Callable>
    static _sp_base_type _S_make_state(_Callable&& __f) {
        using _Impl = _Base_impl<_Callable>;
        return _sp_base_type{new _Impl{std::forward<_Callable>(__f)}};
    }

    template<typename _Tuple>
    struct _Invoker {
	    _Tuple _M_t;

        template<typename> struct __result;
        
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

    static void signal_handler(int,siginfo_t* ,void*);

    template<typename Fn,typename... Args>
    void init(Fn&& fn,Args&& ...args){
        auto _invoker {__make_invoker(std::forward<Fn>(fn),std::forward<Args>(args)...)};
        auto _sp_t {_S_make_state(std::forward<decltype(_invoker)>(_invoker))};
        m_hander_ = std::move(_sp_t);
    }

    template<typename Fn,typename... Args>
    explicit MySignal(const int sig,const int flags,Fn&& fn,Args&& ...args):
            MySignal(sig,flags){
        init(std::forward<Fn>(fn),std::forward<Args>(args)...);
    }

    explicit MySignal(const int sig,const int flags);

public:
    using _sp_MySignal_type = std::shared_ptr<MySignal>;

    template<typename Fn,typename... Args>
    [[nodiscard]] static const _sp_MySignal_type Create(const int sig,const int flags,
                                                Fn&& fn,Args&& ...args){
        
        const _sp_MySignal_type ret (new MySignal(sig,flags,
                    std::forward<Fn>(fn),std::forward<Args>(args)...));
        sm_map_.insert({sig,ret});
        return ret;
    }

    ~MySignal() = default;

    static int sig(int) ;
    int sig()const ;
    static siginfo_t siginfo(int);
    siginfo_t siginfo() const;

private:
    int m_sig_{};
    struct sigaction m_act_{};
    siginfo_t m_info_{};
    _sp_base_type m_hander_{};
    static inline std::unordered_map<int,_sp_MySignal_type> sm_map_;
};

}

#endif
