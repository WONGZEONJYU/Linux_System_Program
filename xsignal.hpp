#ifndef MY_SIGNAL_H
#define MY_SIGNAL_H

#include <tuple>
#include <memory>
#include <type_traits>
#include <utility>
#include <csignal>

namespace xtd {

#define SIG_DISABLE_COPY(Class)\
    Class(const Class &) = delete;\
    Class& operator=(const Class &) = delete;

#define SIG_DISABLE_COPY_MOVE(Class)\
    SIG_DISABLE_COPY(Class)\
    Class(Class &&) = delete;\
    Class& operator=(Class &&) = delete;

    class XSignal;
    using Signal_Ptr = std::shared_ptr<XSignal>;



    class XSignal {
        SIG_DISABLE_COPY_MOVE(XSignal)

        class Base_impl_Factory;

        class Base_{
            friend class Base_impl_Factory;
        public:
            virtual void func() = 0;
            virtual ~Base_() = default;
        protected:
            constexpr explicit Base_() = default;
            struct Private{explicit Private() = default;};
        };

        template<typename Callable_>
        class Base_impl_ final: public Base_ {
        public:
            explicit Base_impl_(Callable_&& f,Private):
            m_S_handler_{std::forward<Callable_>(f)}{}
        private:
            void func() override {
                m_S_handler_();
            }
            Callable_ m_S_handler_{};
        };

        class Base_impl_Factory final{
            Base_impl_Factory() = default;
        public:
            template<typename Callable_>
            static auto make_call_(Callable_&& f){
                using Impl = Base_impl_<Callable_>;
                return std::make_shared<Impl>(std::forward<Callable_>(f),Base_::Private{});
            }
        };

        class Invoker_Factory;
        class Invoker_Base{
            friend class Invoker_Factory;
        protected:
            Invoker_Base() = default;
            struct Private{explicit Private() = default;};
        };

        template<typename Tuple_>
        class Invoker final : public Invoker_Base{
            template<typename> struct result_{};

            template<typename Fn_, typename... Args_>
            struct result_<std::tuple<Fn_, Args_...>> : std::invoke_result<Fn_, Args_...>{};

            template<size_t... Ind_>
            typename result_<Tuple_>::type M_invoke_(std::_Index_tuple<Ind_...>) {
                return std::__invoke(std::get<Ind_>(std::move(m_M_t))...);
            }
        public:
            explicit Invoker(Tuple_ &&t,Private):m_M_t{std::forward<Tuple_>(t)}{}
            typename result_<Tuple_>::type operator()() {
                using Indices_ = typename std::_Build_index_tuple<std::tuple_size_v<Tuple_>>::__type;
                return M_invoke_(Indices_{});
            }
        private:
            Tuple_ m_M_t{};
        };

        class Invoker_Factory final{
            Invoker_Factory() = default;
        public:
            template<typename... Tp_>
            using decayed_tuple_ = std::tuple<std::decay_t<Tp_>...>;

            template<typename Callable_, typename... Args_>
            using Invoker_ = Invoker<decayed_tuple_<Callable_, Args_...>>;

            template<typename Callable_, typename... Args_>
            static inline auto make_invoker_(Callable_&& callable_, Args_&&... args_) {
                return Invoker_<Callable_,Args_...>{{std::forward<Callable_>(callable_),
                    std::forward<Args_>(args_)...} ,Invoker_Base::Private{}};
            }
        };

        template<typename Fn,typename... Args>
        inline void Callable_join(Fn&& fn,Args&& ...args){
            auto invoker_{Invoker_Factory::make_invoker_(std::forward<Fn>(fn),std::forward<Args>(args)...)};
            set_call(Base_impl_Factory::make_call_(std::forward<decltype(invoker_)>(invoker_)));
        }

        static Signal_Ptr create(const int &,const int &);

    protected:
        using Callable_Ptr = std::shared_ptr<Base_>;
        virtual void set_call(const Callable_Ptr &) = 0;
        XSignal() = default;

    public:
        template<typename Fn,typename... Args>
        [[nodiscard]] inline static auto Register(const int &sig,const int &flags,
                            Fn&& fn,Args&& ...args){
            const auto obj{create(sig,flags)};
            if (obj){
                obj->Callable_join(std::forward<Fn>(fn),std::forward<Args>(args)...);
            }

            return obj;
        }

        [[nodiscard]] virtual int sig() const & = 0;
        [[nodiscard]] virtual const siginfo_t& siginfo() const & = 0;
        [[nodiscard]] virtual ucontext_t* context() const & = 0;
        virtual void Unregister() = 0;

        static bool Send_signal(const int &pid_,const int &sig_,const sigval &val_);
        static siginfo_t siginfo(const int&sig);
        static void Unregister(const int &sig);

        virtual ~XSignal() = default;
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
