#ifndef MY_SIGNAL_H
#define MY_SIGNAL_H

#include <signal.h>
#include <tuple>
#include <unordered_map>
#include <memory>
#include <type_traits>
#include <utility>

namespace wtd {

class MySignal final{

    static void signal_handler(int,siginfo_t* ,void*);

    class _Base{
        public:
        virtual void func() = 0;
        virtual ~_Base() = default;
    };

    using _sp_base_type = std::shared_ptr<_Base>;

    template<typename _Callable>
    class _Base_impl:public _Base {
        _Callable _S_handler{};
        public:
        _Base_impl(_Callable&& __f):_S_handler(std::forward<_Callable>(__f)){}
        void func() override {
            _S_handler();
        }
    };

    template<typename _Callable>
    static inline auto _S_make_state(_Callable&& __f) {
        using _Impl = _Base_impl<_Callable>;
        return std::make_shared<_Impl>(std::forward<_Callable>(__f));
    }

    template<typename _Tuple>
    class _Invoker {
        _Tuple _M_t{};
        template<typename> struct __result;

        template<typename _Fn, typename... _Args>
        struct __result<std::tuple<_Fn, _Args...>> : std::__invoke_result<_Fn, _Args...>{ };

        template<size_t... _Ind>
        typename __result<_Tuple>::type _M_invoke(std::_Index_tuple<_Ind...>) {
            return std::__invoke(std::get<_Ind>(std::move(_M_t))...); 
        }

        public:
        _Invoker(_Tuple &&__t):_M_t{std::move(__t)}{}
        typename __result<_Tuple>::type operator()() {
            using _Indices = typename std::_Build_index_tuple<std::tuple_size<_Tuple>::value>::__type;
            return _M_invoke(_Indices{});
        }
    };

    template<typename... _Tp>
    using __decayed_tuple = std::tuple<typename std::decay<_Tp>::type...>;

    template<typename _Callable, typename... _Args>
    using __Invoker = _Invoker<__decayed_tuple<_Callable, _Args...>>;

    template<typename _Callable, typename... _Args>
    static inline __Invoker<_Callable, _Args...> __make_invoker(_Callable&& __callable, _Args&&... __args) {
        return { __decayed_tuple<_Callable, _Args...>{
           std::forward<_Callable>(__callable), std::forward<_Args>(__args)...} };
    }

    template<typename Fn,typename... Args>
    inline void init(Fn&& fn,Args&& ...args){
        auto _invoker {__make_invoker(std::forward<Fn>(fn),std::forward<Args>(args)...)};
        m_hander_ = std::move(_S_make_state(std::forward<decltype(_invoker)>(_invoker)));
    }

    template<typename Fn,typename... Args>
    explicit MySignal(const int &sig,const int &flags,Fn&& fn,Args&& ...args):
            MySignal(sig,flags){
        init(std::forward<Fn>(fn),std::forward<Args>(args)...);
    }

    explicit MySignal(const int &,const int &);

public:
    using _sp_MySignal_type = std::shared_ptr<MySignal>;

    template<typename Fn,typename... Args>
    [[nodiscard]] static const auto Register(const int &sig,const int &flags,
                                                Fn&& fn,Args&& ...args){
        const auto res{sm_map_.try_emplace(sig,new MySignal(sig,flags,
            std::forward<Fn>(fn),std::forward<Args>(args)...))};
        return res.second ? res.first->second : _sp_MySignal_type{};
    }

    void Unregister();
    static int sig(const int&);
    inline auto sig() const {return m_sig_;}
    static siginfo_t siginfo(const int&);
    inline auto siginfo() const{return m_info_;}
    static auto sendsig(const __pid_t &__pid,const int &__sig,const sigval &__val){
        return !sigqueue (__pid,  __sig, __val);
    }

    auto context() const{
        return reinterpret_cast<ucontext_t *>(m_context_);
    }

private:
    int m_sig_{};
    struct sigaction m_act_{};
    siginfo_t m_info_{};
    _sp_base_type m_hander_{};
    void * m_context_;
    static inline std::unordered_map<int,_sp_MySignal_type> sm_map_;

public:
    MySignal(const MySignal&) = delete;
    MySignal(MySignal&&) = delete;
    MySignal& operator=(const MySignal&) = delete;
    MySignal& operator=(MySignal&&) = delete;
    ~MySignal() = default;
};

using _sp_MySignal_type = std::shared_ptr<MySignal>;

template<typename Fn,typename... Args>
static inline const auto Register(const int &sig,const int &flags,
    Fn&& fn,Args&& ...args){
    return MySignal::Register(sig,flags,std::forward<Fn>(fn),std::forward<Args>(args)...);
}

}

#endif
