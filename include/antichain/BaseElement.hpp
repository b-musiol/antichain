/**
 * Antichain Element
 * Base class
 *
 * Author: Bartek Musiol (bmusiol@proton.me)
 *
 * See LICENSE
 */

#ifndef _ANTICHAIN_BASEELEMENT_HPP
#define _ANTICHAIN_BASEELEMENT_HPP

#include <algorithm>
#include <concepts>
#include <cstddef>
#include <iterator>
#include <set>

/*-----------------------------------------------------
 * Declaration
 */
namespace Antichain
{
namespace Template
{

template <typename T>
std::set<T> set_minus(const std::set<T> &a, const std::set<T> &b);

template <typename DerivedElement> struct BaseElement
{
    virtual size_t size() const = 0;
    bool is_comparable(const DerivedElement &other) const;
};

template <typename T>
concept Antichain_Base_Element = std::derived_from<T, BaseElement<T>>;

} // namespace Template
} // namespace Antichain

/*-----------------------------------------------------
 * Definition
 */
namespace Antichain
{
namespace Template
{

template <typename T>
std::set<T> set_minus(const std::set<T> &a, const std::set<T> &b)
{
    std::set<T> result;

    std::set_difference(a.begin(),
                        a.end(),
                        b.begin(),
                        b.end(),
                        std::inserter(result, result.begin()));

    return result;
}

template <typename DerivedElement>
bool BaseElement<DerivedElement>::is_comparable(
    const DerivedElement &other) const
{
    return static_cast<const DerivedElement *>(this)->is_comparable_impl(other);
}
} // namespace Template
} // namespace Antichain

#endif // _ANTICHAIN_BASEELEMENT_HPP