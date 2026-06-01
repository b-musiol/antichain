/**
 * Antichain
 *
 * Author: Bartek Musiol (bmusiol@proton.me)
 *
 * See LICENSE
 */

#ifndef _ANTICHAIN__HPP
#define _ANTICHAIN__HPP

#include "BaseElement.hpp"
#include <vector>

/*-----------------------------------------------------
 * Declaration
 */
namespace Antichain
{

template <Template::Antichain_Base_Element Element_T> class Object
{
  public:
    Object();

  public:
    bool insert(Element_T element);
    void clean();

  public:
    std::vector<std::vector<Element_T>> get_as_buckets();
    std::vector<Element_T> get_flat();

  private:
    void reset_erase_flag();

  private:
    std::vector<std::vector<Element_T>> buckets;
    std::vector<std::vector<bool>> erase_flags;
};

} // namespace Antichain

/*-----------------------------------------------------
 * Definition
 */
namespace Antichain
{
template <Template::Antichain_Base_Element Element_T>
Object<Element_T>::Object()
    : // Put the zero-bucket for zero-sized elements (none), so that id-ing is
      // not so bug-affine
      buckets(1, std::vector<Element_T>()), erase_flags(1, std::vector<bool>())
{
}

template <Template::Antichain_Base_Element Element_T>
void Object<Element_T>::clean()
{
    for (size_t i = buckets.size() - 1; i > 0; i--)
    {
        if (buckets[i].size() > 0)
        {
            return;
        }
        else // if (buckets[i].size() == 0)
        {
            buckets.pop_back();
            erase_flags.pop_back();
        }
    }
}

template <Template::Antichain_Base_Element Element_T>
std::vector<std::vector<Element_T>> Object<Element_T>::get_as_buckets()
{
    return buckets;
}
template <Template::Antichain_Base_Element Element_T>
std::vector<Element_T> Object<Element_T>::get_flat()
{
    std::vector<Element_T> ret;

    for (size_t sz = 1; sz < buckets.size(); sz++)
    {
        for (auto &element : buckets[sz])
        {
            ret.push_back(element);
        }
    }

    return ret;
}

template <Template::Antichain_Base_Element Element_T>
void Object<Element_T>::reset_erase_flag()
{
    for (auto &row : erase_flags)
    {
        std::fill(row.begin(), row.end(), false);
    }
}

template <Template::Antichain_Base_Element Element_T>
bool Object<Element_T>::insert(Element_T element)
{
    size_t current_bucket;
    reset_erase_flag();
    // rejection loop
    for (current_bucket = 1;
         current_bucket < buckets.size() && current_bucket <= element.size();
         current_bucket++)
    {
        // Check if any of the elements in the smaller buckets are comparable
        // to the candidate element. If at least one matches, the element is
        // rejected and will not evict any other element, thus returning from
        // the method early.
        for (size_t current_reference = 0;
             current_reference < buckets[current_bucket].size();
             current_reference++)
        {
            auto &ref_element = buckets[current_bucket][current_reference];
            if (ref_element.is_comparable(element))
            {
                // Found a smaller element that is comparable. So we reject.
                return false;
            }
        }
    }

    // eviction loop
    for (current_bucket++; current_bucket < buckets.size(); current_bucket++)
    {
        // Check if any of the elements in the larger buckets are comparable
        // to the candidate element. Each one that matches is getting evicted.
        // This part of the loop runs through the entire rest of the antichain,
        // but it only runs if the element is not rejected first. AND it has a
        // good opportunity to make the whole antichain smaller.
        for (size_t current_reference = 0;
             current_reference < buckets[current_bucket].size();
             current_reference++)
        {
            auto &ref_element = buckets[current_bucket][current_reference];
            if (ref_element.is_comparable(element))
            {
                // Found a smaller element that is comparable.
                // Set the erase flag here to true
                erase_flags[current_bucket][current_reference] = true;
            }
        }
    }

    // Execute eviction
    for (size_t current_bucket = 1; current_bucket < buckets.size();
         current_bucket++)
    {
        auto &bucket           = buckets[current_bucket];
        auto &curr_erase_flags = erase_flags[current_bucket];
        auto &flags            = erase_flags[current_bucket];

        size_t write = 0;
        for (size_t read = 0; read < bucket.size(); ++read)
        {
            if (!flags[read])
            {
                bucket[write++] = std::move(bucket[read]);
            }
        }

        bucket.resize(write);
        curr_erase_flags.resize(write);
    }

    // Adjust buckets if this is an element that is larger than anything
    // before.
    // Note: We're not shrinking this automatically.
    while (buckets.size() <= element.size())
    {
        buckets.emplace_back();
        erase_flags.emplace_back();
    }
    // Insert new element, as it is not rejected
    buckets[element.size()].push_back(element);
    erase_flags[element.size()].push_back(false);

    // element has not been rejected, thus return true
    return true;
}

} // namespace Antichain

#endif // _ANTICHAIN__HPP