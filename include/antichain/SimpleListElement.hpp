/**
 * Antichain Element
 * Simple List Element of Integral or String type
 *
 * Author: Bartek Musiol (bmusiol@proton.me)
 *
 * See LICENSE
 */

#ifndef _ANTICHAIN_SIMPLELISTELEMENT_HPP
#define _ANTICHAIN_SIMPLELISTELEMENT_HPP

#include "BaseElement.hpp"
#include "internal/concepts.hpp"

#include <cstddef>
#include <set>

/*-----------------------------------------------------
 * Declaration
 */
namespace Antichain
{
namespace Predefined
{

template <_Internal_::IntegralOrString inner_T>
class SimpleListElement
    : public Template::BaseElement<SimpleListElement<inner_T>>
{
  public:
    SimpleListElement() = default;
    SimpleListElement(std::set<inner_T> data);

  public:
    size_t size() const override;
    std::set<inner_T> get();

  public:
    bool is_comparable_impl(const SimpleListElement &other) const;

  private:
  private:
    std::set<inner_T> data;
};
} // namespace Predefined
} // namespace Antichain

/*-----------------------------------------------------
 * Declaration
 */
namespace Antichain
{
namespace Predefined
{

template <_Internal_::IntegralOrString inner_T>
SimpleListElement<inner_T>::SimpleListElement(std::set<inner_T> data)
    : data(std::move(data))
{
}

template <_Internal_::IntegralOrString inner_T>
std::set<inner_T> SimpleListElement<inner_T>::get()
{
    return data;
}

template <_Internal_::IntegralOrString inner_T>
size_t SimpleListElement<inner_T>::size() const
{
    return data.size();
}

template <_Internal_::IntegralOrString inner_T>
bool SimpleListElement<inner_T>::is_comparable_impl(
    const SimpleListElement &other) const
{
    if (this->data.size() == other.data.size())
    {
        // this (1, 2) : other (1, 2) => true
        // this (1, 2) : other (2, 3) => false
        return this->data == other.data;
    }
    else if (this->data.size() > other.data.size())
    {
        // this (1, 2) : other (1) => true
        // this (1, 2) : other (4) => false
        auto rem         = Template::set_minus(this->data, other.data);
        size_t diff_size = this->size() - other.size();
        return rem.size() == diff_size;
    }
    else // if (this->data.size() <other.data.size())
    {
        // this (1) : other (1, 2) => true
        // this (4) : other (1, 2) => false
        auto rem         = Template::set_minus(other.data, this->data);
        size_t diff_size = other.size() - this->size();
        return rem.size() == diff_size;
    }
}

template <typename T>
static std::set<T> set_minus(const std::set<T> &a, const std::set<T> &b)
{
    std::set<T> result;

    std::set_difference(a.begin(),
                        a.end(),
                        b.begin(),
                        b.end(),
                        std::inserter(result, result.begin()));

    return result;
}

} // namespace Predefined
} // namespace Antichain

#endif // _ANTICHAIN_BASEELEMENT_HPP