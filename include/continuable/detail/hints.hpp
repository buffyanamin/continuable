
/**

                        /~` _  _ _|_. _     _ |_ | _
                        \_,(_)| | | || ||_|(_||_)|(/_

                    https://github.com/Naios/continuable
                                   v2.0.0

  Copyright(c) 2015 - 2017 Denis Blank <denis.blank at outlook dot com>

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

#ifndef CONTINUABLE_DETAIL_HINTS_HPP_INCLUDED__
#define CONTINUABLE_DETAIL_HINTS_HPP_INCLUDED__

#include <type_traits>

#include <continuable/detail/api.hpp>
#include <continuable/detail/util.hpp>

namespace cti {
namespace detail {
namespace hints {
/// Represents a present signature hint
template <typename... Args>
using signature_hint_tag = traits::identity<Args...>;
/// Represents an absent signature hint
struct absent_signature_hint_tag {};

template <typename>
struct is_absent_hint : std::false_type {};
template <>
struct is_absent_hint<absent_signature_hint_tag> : std::true_type {};
} // namespace hints
} // namespace detail
} // namespace cti

#endif // CONTINUABLE_DETAIL_HINTS_HPP_INCLUDED__
