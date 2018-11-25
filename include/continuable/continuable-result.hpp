
/*

                        /~` _  _ _|_. _     _ |_ | _
                        \_,(_)| | | || ||_|(_||_)|(/_

                    https://github.com/Naios/continuable
                                   v3.0.0

  Copyright(c) 2015 - 2018 Denis Blank <denis.blank at outlook dot com>

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files(the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions :

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
**/

#ifndef CONTINUABLE_RESULT_HPP_INCLUDED
#define CONTINUABLE_RESULT_HPP_INCLUDED

#include <type_traits>
#include <utility>
#include <continuable/continuable-primitives.hpp>
#include <continuable/detail/utility/flat-variant.hpp>
#include <continuable/detail/utility/result-trait.hpp>
#include <continuable/detail/utility/traits.hpp>

namespace cti {
/// A class which is convertible to any result and that definitly holds no
/// value so the real result gets invalidated when
/// this object is passed to it
struct empty_result {};

/// A class which is convertible to any result and that definitly holds
/// an exception which is then passed to the converted result object.
class exceptional_result {
  exception_t exception_;

public:
  exceptional_result() = delete;
  exceptional_result(exceptional_result const&) = default;
  exceptional_result(exceptional_result&&) = default;
  exceptional_result& operator=(exceptional_result const&) = default;
  exceptional_result& operator=(exceptional_result&&) = default;
  ~exceptional_result() = default;

  explicit exceptional_result(exception_t exception)
      // NOLINTNEXTLINE(hicpp-move-const-arg, performance-move-const-arg)
      : exception_(std::move(exception)) {
  }

  exceptional_result& operator=(exception_t exception) {
    // NOLINTNEXTLINE(hicpp-move-const-arg, performance-move-const-arg)
    exception_ = std::move(exception);
    return *this;
  }

  void set_exception(exception_t exception) {
    // NOLINTNEXTLINE(hicpp-move-const-arg, performance-move-const-arg)
    exception_ = std::move(exception);
  }

  exception_t& get_exception() & noexcept {
    return exception_;
  }
  exception_t const& get_exception() const& noexcept {
    return exception_;
  }
  exception_t&& get_exception() && noexcept {
    return std::move(exception_);
  }
};

/// A class similar to the one in the result proposal,
/// however it's capable of carrying an exception_t.
template <typename... T>
class result {
  using trait_t = detail::result_trait<T...>;
  using surrogate_t = typename trait_t::surrogate_t;

public:
  using value_t = typename trait_t::value_t;

  result() = default;
  result(result const&) = default;
  result(result&&) = default;
  result& operator=(result const&) = default;
  result& operator=(result&&) = default;
  ~result() = default;

  template <typename... Args,
            decltype(trait_t::wrap(std::declval<Args>()...))* = nullptr>
  explicit result(Args&&... values)
      : variant_(trait_t::wrap(std::forward<Args>(values)...)) {
  }
  explicit result(exception_t exception) : variant_(std::move(exception)) {
  }
  result(empty_result){};
  result(exceptional_result exceptional_result)
      : variant_(std::move(exceptional_result.get_exception())) {
  }

  result& operator=(empty_result) {
    set_empty();
    return *this;
  }
  result& operator=(exceptional_result exceptional_result) {
    set_exception(std::move(exceptional_result.get_exception()));
    return *this;
  }

  void set_empty() {
    variant_.set_empty();
  }
  void set_value(T... values) {
    variant_ = trait_t::wrap(std::move(values)...);
  }
  void set_exception(exception_t exception) {
    variant_ = std::move(exception);
  }

  bool is_empty() const noexcept {
    return variant_.is_empty();
  }
  bool is_value() const noexcept {
    return variant_.template is<surrogate_t>();
  }
  bool is_exception() const noexcept {
    return variant_.template is<exception_t>();
  }

  explicit constexpr operator bool() const noexcept {
    return is_value();
  }

  decltype(auto) get_value() & noexcept {
    return trait_t::unwrap(variant_.template cast<surrogate_t>());
  }
  decltype(auto) get_value() const& noexcept {
    return trait_t::unwrap(variant_.template cast<surrogate_t>());
  }
  decltype(auto) get_value() && noexcept {
    return trait_t::unwrap(std::move(variant_).template cast<surrogate_t>());
  }

  decltype(auto) operator*() & noexcept {
    return get_value();
  }
  decltype(auto) operator*() const& noexcept {
    return get_value();
  }
  decltype(auto) operator*() && noexcept {
    return std::move(*this).get_value();
  }

  exception_t& get_exception() & noexcept {
    return variant_.template cast<exception_t>();
  }
  exception_t const& get_exception() const& noexcept {
    return variant_.template cast<exception_t>();
  }
  exception_t&& get_exception() && noexcept {
    return std::move(variant_).template cast<exception_t>();
  }

private:
  detail::container::flat_variant<surrogate_t, exception_t> variant_;
};

template <typename... T>
auto make_result(T&&... values) {
  return result<detail::traits::unrefcv_t<T>...>(std::forward<T>(values)...);
}

// NOLINTNEXTLINE(performance-unnecessary-value-param)
inline exceptional_result make_exceptional_result(exception_t exception) {
  // NOLINTNEXTLINE(hicpp-move-const-arg, performance-move-const-arg)
  return exceptional_result{std::move(exception)};
}

inline empty_result make_empty_result() {
  return {};
}
} // namespace cti

#endif // CONTINUABLE_RESULT_HPP_INCLUDED