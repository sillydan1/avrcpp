/*
 * This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * 
 * 
 * original author: sillydan1 <https://github.com/sillydan1>
 * */
#ifndef AVRCPP_TEST_DEQUE_H
#define AVRCPP_TEST_DEQUE_H
// Suppress clangd-tidy complains about static storage in gtest
#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"
#include <gtest/gtest.h>
#include "../include/deque"

TEST(deque, givenBlank_whenConstruct_thenEmptyDeque) {
    auto sut = stl::deque<int>{};
    ASSERT_EQ(0, sut.size());
    ASSERT_TRUE(sut.empty());
    ASSERT_EQ(sut.begin(), sut.end());
}

TEST(deque, givenBlank_whenPushBack_thenElementIsPushedBack) {
    auto sut = stl::deque<int>{};
    sut.push_back(3);
    ASSERT_NE(sut.begin(), sut.end());
    ASSERT_EQ(1, sut.size());
    ASSERT_EQ(3, sut.back());
    ASSERT_EQ(3, sut.front());
}

TEST(deque, givenBlank_whenPushFront_thenElementIsPushedFront) {
    auto sut = stl::deque<int>{};
    sut.push_front(3);
    ASSERT_NE(sut.begin(), sut.end());
    ASSERT_EQ(1, sut.size());
    ASSERT_EQ(3, sut.back());
    ASSERT_EQ(3, sut.front());
}

TEST(deque, givenLimitedMapSize_whenPushBack_thenMapIcreases) {
    auto sut = stl::deque<int, 2>{};
    sut.push_back(1);
    sut.push_back(2);
    sut.push_back(3); // Add another node
    ASSERT_EQ(3, sut.size());
    ASSERT_EQ(3, sut.begin()[2]);
}

TEST(deque, givenLimitedMapSize_whenPushFront_thenMapIcreases) {
    auto sut = stl::deque<int, 2>{};
    sut.push_front(1);
    sut.push_front(2);
    sut.push_front(3); // Add another node
    ASSERT_EQ(3, sut.size());
    ASSERT_EQ(3, sut.begin()[0]);
}

TEST(deque, givenSomeValues_whenForeach_thenIterateProperly) {
    auto sut = stl::deque<int>{};
    sut.push_back(1);
    sut.push_back(2);
    sut.push_back(3);
    sut.push_back(4);
    sut.push_back(5);
    int i = 1;
    for(auto& el : sut)
        EXPECT_EQ(i++, el);
}

TEST(deque, givenSomeValues_whenForeachTwice_thenIterateProperly) {
    auto sut = stl::deque<int>{};
    sut.push_front(5); // Note the push_front usage here
    sut.push_front(4);
    sut.push_front(3);
    sut.push_front(2);
    sut.push_front(1);
    int i = 1;
    for(auto& el : sut)
        EXPECT_EQ(i++, el);
    int j = 1;
    for(auto& el : sut) // foreach should not change the begin() and end() iterators.
        EXPECT_EQ(j++, el);
}

TEST(deque, givenSomeValuesAndLimitedMapSize_whenForeach_thenIterateProperly) {
    auto sut = stl::deque<int, 2>{}; // Note the limited size
    sut.push_back(1);
    sut.push_back(2);
    sut.push_back(3);
    sut.push_back(4);
    sut.push_back(5);
    sut.push_back(6);
    int i = 1;
    for(auto& el : sut)
        EXPECT_EQ(i++, el);
}

TEST(deque, givenClassWithDtor_whenClear_thenCallDtor) {
    static int dtor_counter = 0;
    static int ctor_counter = 0;
    static int cpyctor_counter = 0;
    struct test_struct {
        ~test_struct() {
            dtor_counter++;
        }
        test_struct(const test_struct& o) {
            cpyctor_counter++;
        }
        test_struct() {
            ctor_counter++;
        }
    };
    auto sut = stl::deque<test_struct>{};
    sut.push_back({});
    sut.push_back({});
    sut.push_back({});
    ASSERT_EQ(3, ctor_counter);
    ASSERT_EQ(3, cpyctor_counter);
    ASSERT_EQ(3, dtor_counter); // push_back copies the stack element - then stack kills the original
    sut.clear();
    ASSERT_EQ(3, ctor_counter);
    ASSERT_EQ(3, cpyctor_counter);
    ASSERT_EQ(6, dtor_counter);
    ASSERT_TRUE(sut.empty());
    ASSERT_EQ(0, sut.size());
    ASSERT_EQ(sut.begin(), sut.end());
}

TEST(deque, givenClass_whenEmplaceBack_thenNoCallToDtor) {
    static int dtor_counter = 0;
    static int ctor_counter = 0;
    static int cpyctor_counter = 0;
    struct test_struct {
        int v;
        ~test_struct() {
            dtor_counter++;
        }
        test_struct(const test_struct& o) {
            cpyctor_counter++;
        }
        test_struct(int v) : v{v} {
            ctor_counter++;
        }
    };
    auto sut = stl::deque<test_struct>{};
    sut.emplace_back(1);
    sut.emplace_back(2);
    sut.emplace_back(3);
    ASSERT_EQ(3, ctor_counter);
    ASSERT_EQ(0, cpyctor_counter);
    ASSERT_EQ(0, dtor_counter);
    sut.clear();
    ASSERT_EQ(3, ctor_counter);
    ASSERT_EQ(0, cpyctor_counter);
    ASSERT_EQ(3, dtor_counter);
    ASSERT_TRUE(sut.empty());
    ASSERT_EQ(0, sut.size());
    ASSERT_EQ(sut.begin(), sut.end());
}

#pragma clang diagnostic pop
#endif
