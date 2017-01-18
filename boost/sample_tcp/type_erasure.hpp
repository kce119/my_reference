#ifndef INCLUDE_GUARD_TYPE_ERASURE_HPP
#define INCLUDE_GUARD_TYPE_ERASURE_HPP

#include <memory>
#include <boost/type_erasure/any.hpp>

// shared_anyの定義

template <typename Concept>
class shared_any : public boost::type_erasure::any<Concept, boost::type_erasure::_self&> {
    using base_type = boost::type_erasure::any<Concept, boost::type_erasure::_self&>;
    std::shared_ptr<void> ownership_;
public:
    template <class U>
    shared_any(std::shared_ptr<U> p)
        : base_type(*p), ownership_(p) {}

    template <class U>
    shared_any& operator=(std::shared_ptr<U> p) {
        base_type temp(*p);
        ownership_ = p;
        return *this;
    }
    void const* get_address() const {
        return ownership_.get();
    }
};

#include <boost/type_erasure/member.hpp>
#include <boost/type_erasure/operators.hpp>
#include <boost/any.hpp>
#include <string>
using namespace boost::type_erasure;

// メンバ関数引数の数と、制約名(has_foo)、実関数名(foo)マッピング
// ※ ここはシグニチャのみなので、戻り値に関する情報は含まれない。
BOOST_TYPE_ERASURE_MEMBER((ns)(has_send_list_num), send_list_num, 1)

namespace mpl = boost::mpl;

namespace ns { // なんらかのnamespaceは必須 *1
// 制約の記述
//    抽象型名↓
using abstract_type = shared_any<
    mpl::vector<
        destructible<>, // 破棄可能(最小限の制約)
        assignable<>,
        has_send_list_num<void(std::size_t)>
    >
>;
}

#endif //INCLUDE_GUARD_TYPE_ERASURE
