/**
 * Antichain
 * concepts
 *
 * Author: Bartek Musiol (bmusiol@proton.me)
 *
 * See LICENSE
 */

#ifndef _ANTICHAIN_CONCEPTS_HPP
#define _ANTICHAIN_CONCEPTS_HPP

#include <concepts>
#include <string>
#include <type_traits>

namespace Antichain
{
namespace _Internal_
{

template <typename T>
concept StringLike = std::same_as<std::remove_cvref_t<T>, std::string> ||
                     std::same_as<std::remove_cvref_t<T>, std::string_view>;

template <typename T>
concept IntegralOrString = std::integral<T> || StringLike<T>;


} // namespace Concept
} // namespace Antichain

#endif // _KNOKAN_CONCEPTS_HPP