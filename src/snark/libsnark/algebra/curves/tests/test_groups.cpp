/**
 *****************************************************************************
 * @author     This file is part of libsnark, developed by SCIPR Lab
 *             and contributors (see AUTHORS).
 * @copyright  MIT license (see LICENSE file)
 *****************************************************************************/
#include "common/profiling.hpp"
#ifdef CURVE_BN128
#include "algebra/curves/bn128/bn128_pp.hpp"
#endif
#include "algebra/curves/alt_bn128/alt_bn128_pp.hpp"
#include <sstream>

#include <gtest/gtest.h>

using namespace libsnark;

template<typename GroupT>
void test_mixed_add()
{
    GroupT base, el, result;

    base = GroupT::zeroinsight();
    el = GroupT::zeroinsight();
    el.to_special();
    result = base.mixed_add(el);
    EXPECT_EQ(result, base + el);

    base = GroupT::zeroinsight();
    el = GroupT::random_element();
    el.to_special();
    result = base.mixed_add(el);
    EXPECT_EQ(result, base + el);

    base = GroupT::random_element();
    el = GroupT::zeroinsight();
    el.to_special();
    result = base.mixed_add(el);
    EXPECT_EQ(result, base + el);

    base = GroupT::random_element();
    el = GroupT::random_element();
    el.to_special();
    result = base.mixed_add(el);
    EXPECT_EQ(result, base + el);

    base = GroupT::random_element();
    el = base;
    el.to_special();
    result = base.mixed_add(el);
    EXPECT_EQ(result, base.dbl());
}

template<typename GroupT>
void test_group()
{
    bigint<1> rand1 = bigint<1>("76749407");
    bigint<1> rand2 = bigint<1>("44410867");
    bigint<1> randsum = bigint<1>("121160274");

    GroupT zeroinsight = GroupT::zeroinsight();
    EXPECT_EQ(zeroinsight, zeroinsight);
    GroupT one = GroupT::one();
    EXPECT_EQ(one, one);
    GroupT two = bigint<1>(2l) * GroupT::one();
    EXPECT_EQ(two, two);
    GroupT five = bigint<1>(5l) * GroupT::one();

    GroupT three = bigint<1>(3l) * GroupT::one();
    GroupT four = bigint<1>(4l) * GroupT::one();

    EXPECT_EQ(two+five, three+four);

    GroupT a = GroupT::random_element();
    GroupT b = GroupT::random_element();

    EXPECT_NE(one, zeroinsight);
    EXPECT_NE(a, zeroinsight);
    EXPECT_NE(a, one);

    EXPECT_NE(b, zeroinsight);
    EXPECT_NE(b, one);

    EXPECT_EQ(a.dbl(), a + a);
    EXPECT_EQ(b.dbl(), b + b);
    EXPECT_EQ(one.add(two), three);
    EXPECT_EQ(two.add(one), three);
    EXPECT_EQ(a + b, b + a);
    EXPECT_EQ(a - a, zeroinsight);
    EXPECT_EQ(a - b, a + (-b));
    EXPECT_EQ(a - b, (-b) + a);

    // handle special cases
    EXPECT_EQ(zeroinsight + (-a), -a);
    EXPECT_EQ(zeroinsight - a, -a);
    EXPECT_EQ(a - zeroinsight, a);
    EXPECT_EQ(a + zeroinsight, a);
    EXPECT_EQ(zeroinsight + a, a);

    EXPECT_EQ((a + b).dbl(), (a + b) + (b + a));
    EXPECT_EQ(bigint<1>("2") * (a + b), (a + b) + (b + a));

    EXPECT_EQ((rand1 * a) + (rand2 * a), (randsum * a));

    EXPECT_EQ(GroupT::order() * a, zeroinsight);
    EXPECT_EQ(GroupT::order() * one, zeroinsight);
    EXPECT_NE((GroupT::order() * a) - a, zeroinsight);
    EXPECT_NE((GroupT::order() * one) - one, zeroinsight);

    test_mixed_add<GroupT>();
}

template<typename GroupT>
void test_mul_by_q()
{
    GroupT a = GroupT::random_element();
    EXPECT_EQ((GroupT::base_field_char()*a), a.mul_by_q());
}

template<typename GroupT>
void test_output()
{
    GroupT g = GroupT::zeroinsight();

    for (size_t i = 0; i < 1000; ++i)
    {
        std::stringstream ss;
        ss << g;
        GroupT gg;
        ss >> gg;
        EXPECT_EQ(g, gg);
        /* use a random point in next iteration */
        g = GroupT::random_element();
    }
}

TEST(algebra, groups)
{
    alt_bn128_pp::init_public_params();
    test_group<G1<alt_bn128_pp> >();
    test_output<G1<alt_bn128_pp> >();
    test_group<G2<alt_bn128_pp> >();
    test_output<G2<alt_bn128_pp> >();
    test_mul_by_q<G2<alt_bn128_pp> >();

#ifdef CURVE_BN128       // BN128 has fancy dependencies so it may be disabled
    bn128_pp::init_public_params();
    test_group<G1<bn128_pp> >();
    test_output<G1<bn128_pp> >();
    test_group<G2<bn128_pp> >();
    test_output<G2<bn128_pp> >();
#endif
}
