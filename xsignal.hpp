#ifndef MY_SIGNAL_H
#define MY_SIGNAL_H

#include <tuple>
#include <unordered_map>
#include <memory>
#include <type_traits>
#include <utility>
#include <csignal>

namespace xtd {

class XSignal final{

    static void signal_handler(int,siginfo_t* ,void*);

    class Base_{
        public:
        virtual void func() = 0;
        virtual ~Base_() = default;
    protected:
        Base_() = default;
    };

    using _sp_base_type = std::shared_ptr<Base_>;
    
    template<typename Callable_>
    class Base_impl_:public Base_ {
        Callable_ m_S_handler_{};
        public:
        explicit Base_impl_(Callable_&& f):Callable_(std::forward<Callable_>(f)){}
        void func() override {
            m_S_handler_();
        }
    };

    template<typename Callable_>
    static inline auto S_make_state_(Callable_&& f){
        using Impl_ = Base_impl_<Callable_>;
        return std::make_shared<Impl_>(std::forward<Callable_>(f));
    }

    template<typename Tuple_>
    class Invoker {
        Tuple_ m_M_t{};
        template<typename> struct result_{};

        template<typename Fn_, typename... Args_>
        struct result_<std::tuple<Fn_, Args_...>> : std::__invoke_result<Fn_, Args_...>{ };

        template<size_t... Ind_>
        typename result_<Tuple_>::type M_invoke_(std::_Index_tuple<Ind_...>) {
            return std::__invoke(std::get<Ind_>(std::move(m_M_t))...);
        }

        public:
        explicit Invoker(Tuple_ &&t):m_M_t{std::forward<Tuple_>(t)}{}
        typename result_<Tuple_>::type operator()() {
            using Indices_ = typename std::_Build_index_tuple<std::tuple_size_v<Tuple_>>::__type;
            return M_invoke_(Indices_{});
        }
    };

    template<typename... Tp_>
    using decayed_tuple_ = std::tuple<std::decay_t<Tp_>...>;

    template<typename Callable_, typename... Args_>
    using Invoker_ = Invoker<decayed_tuple_<Callable_, Args_...>>;

    template<typename Callable_, typename... Args_>
    static inline Invoker_<Callable_, Args_...> make_invoker_(Callable_&& callable_, Args_&&... args_) {
        return { decayed_tuple_<Callable_, Args_...>{
           std::forward<Callable_>(callable_), std::forward<Args_>(args_)...} };
    }

    template<typename Fn,typename... Args>
    inline void init(Fn&& fn,Args&& ...args){
        auto _invoker {__make_invoker(std::forward<Fn>(fn),std::forward<Args>(args)...)};
        m_call_ = std::move(_S_make_state(std::forward<decltype(_invoker)>(_invoker)));
    }

    template<typename Fn,typename... Args>
    explicit XSignal(const int &sig,const int &flags,Fn&& fn,Args&& ...args):
            XSignal(sig,flags){
        init(std::forward<Fn>(fn),std::forward<Args>(args)...);
    }

    explicit XSignal(const int &,const int &);

public:
    using _sp_MySignal_type = std::shared_ptr<XSignal>;

    template<typename Fn,typename... Args>
    [[nodiscard]] static auto Register(const int &sig,const int &flags,
                                                Fn&& fn,Args&& ...args){
        const auto &res{sm_map_.try_emplace(sig,new XSignal(sig,flags,
            std::forward<Fn>(fn),std::forward<Args>(args)...))};
        return res.second ? res.first->second : _sp_MySignal_type{};
    }

    void Unregister();
    static int sig(const int&);
    [[nodiscard]] inline auto sig() const{return m_sig_;}
    static siginfo_t siginfo(const int&);
    [[nodiscard]] inline auto siginfo() const{return m_info_;}
    static auto Send_signal(const __pid_t &pid_,const int &sig_,const sigval &val_){
        return !sigqueue(pid_,sig_,val_);
    }

    [[nodiscard]] auto context() const{
        return static_cast<ucontext_t *>(m_context_);
    }

private:
    int m_sig_{};
    struct sigaction m_act_{};
    siginfo_t m_info_{};
    _sp_base_type m_call_{};
    void * m_context_{};

    using call_map_t = std::unordered_map<int,_sp_MySignal_type>;
    static inline call_map_t sm_map_{};

public:
    XSignal(const XSignal&) = delete;
    XSignal(XSignal&&) = delete;
    XSignal& operator=(const XSignal&) = delete;
    XSignal& operator=(XSignal&&) = delete;
    ~XSignal();
};

using _sp_MySignal_type = std::shared_ptr<XSignal>;

template<typename Fn,typename... Args>
static inline auto Register(const int &sig,const int &flags,
    Fn&& fn,Args&& ...args){
    return XSignal::Register(sig,flags,std::forward<Fn>(fn),std::forward<Args>(args)...);
}

}

#endif
