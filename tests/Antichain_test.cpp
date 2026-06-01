/**
 * Antichain
 * Tests
 *
 * Author: Bartek Musiol (bmusiol@proton.me)
 *
 * See LICENSE
 */

#include "../include/antichain/Antichain.hpp"
#include "../include/antichain/SimpleListElement.hpp"
#include <gtest/gtest.h>
#include <set>

template <typename Elem_T>
void print_buckets(const char *prefix, Antichain::Object<Elem_T> &antichain)
{
    int bucket_ctr = 0;
    int entry_ctr  = 0;
    auto d2        = antichain.get_as_buckets();
    bucket_ctr     = 0;
    std::cout << "\n" << prefix << "\n";
    for (auto &bucket : d2)
    {
        std::cout << "Bucket " << bucket_ctr << ":\n";
        entry_ctr = 0;
        for (auto &entry : bucket)
        {
            std::cout << "  Entry #" << entry_ctr << ": ";
            auto elem = entry.get();
            for (auto &digit : elem)
            {
                std::cout << " " << digit << " ";
            }
            std::cout << "\n";
            entry_ctr++;
        }
        bucket_ctr++;
    }
    std::cout << "\n";
}

TEST(Antichain, Basic_SimpleListElement)
{
    using Elem = Antichain::Predefined::SimpleListElement<int>;
    Antichain::Object<Elem> antichain;
    int bucket_ctr = 0;
    int entry_ctr  = 0;

    EXPECT_TRUE(antichain.insert(Elem({1})));
    print_buckets<Elem>("inserting (1)", antichain);

    EXPECT_FALSE(antichain.insert(Elem({1})));
    print_buckets<Elem>("inserting (1) again", antichain);

    EXPECT_FALSE(antichain.insert(Elem({1, 2})));
    print_buckets<Elem>("inserting (1, 2)", antichain);

    EXPECT_TRUE(antichain.insert(Elem({2, 3})));
    print_buckets<Elem>("inserting (2, 3)", antichain);

    EXPECT_TRUE(antichain.insert(Elem({3})));
    print_buckets<Elem>("inserting (3)", antichain);

    EXPECT_FALSE(antichain.insert(Elem({3, 1})));
    print_buckets<Elem>("inserting (3, 1)", antichain);

    EXPECT_TRUE(antichain.insert(Elem({5, 6})));
    print_buckets<Elem>("inserting (5, 6)", antichain);

    auto flat                        = antichain.get_flat();
    std::set<std::set<int>> expected = {{1}, {3}, {5, 6}};
    for (auto &entry : flat)
    {
        expected.erase(entry.get());
    }
    EXPECT_EQ(expected.size(), 0);
}