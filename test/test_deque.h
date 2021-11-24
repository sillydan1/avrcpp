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

TEST(deque, givenBlankStart_whenConstruct_thenEmptyDeque) {
    auto sut = stl::deque<int>{};
    ASSERT_EQ(0, sut.size());
    ASSERT_TRUE(sut.empty());
    ASSERT_EQ(sut.begin(), sut.end());
}

TEST(deque, givenBlankStart_whenPushBack_thenElementIsPushedBack) {
    auto sut = stl::deque<int>{};
    sut.push_back(3);
    ASSERT_NE(sut.begin(), sut.end());
    ASSERT_EQ(1, sut.size());
    ASSERT_EQ(3, sut.back());
    ASSERT_EQ(3, sut.front());
}

#pragma clang diagnostic pop
#endif
