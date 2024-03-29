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
#ifndef AVRCPP_TEST_VECTOR_H
#define AVRCPP_TEST_VECTOR_H
#include "../include/vector"
// Suppress clangd-tidy complains about static storage in gtest
#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"

TEST(vector, givenEmpty_whenAdd_thenAdded) {
    auto sut = stl::vector<int>();
    EXPECT_EQ(0, sut.size());
    sut.push_back(1);
    EXPECT_EQ(1, sut.size());
    EXPECT_EQ(1, sut[0]);
}

TEST(vector, givenElements_whenEraseBegin_thenRemoved) {
    auto sut = stl::vector<int>();
    sut.push_back(1);
    sut.push_back(2);
    sut.push_back(3);
    EXPECT_EQ(3, sut.size());
    sut.erase(sut.begin());
    EXPECT_EQ(2, sut.size());
    sut.erase(sut.begin());
    EXPECT_EQ(1, sut.size());
    sut.erase(sut.begin());
    EXPECT_EQ(0, sut.size());
}

TEST(vector, givenElements_whenInsertingAtIndex_thenItemIsInserted) {
    auto sut = stl::vector<int>();
    sut.push_back(1);
    sut.push_back(3);
    sut.push_back(4);
    auto it = sut.begin();
    EXPECT_EQ(1, *(it++));
    EXPECT_EQ(3, *(it++));
    EXPECT_EQ(4, *(it++));
    EXPECT_EQ(sut.end(), it);
    sut.insert(sut.begin()+1, 2);
    it = sut.begin();
    EXPECT_EQ(1, *(it++));
    EXPECT_EQ(2, *(it++));
    EXPECT_EQ(3, *(it++));
    EXPECT_EQ(4, *(it++));
    EXPECT_EQ(sut.end(), it);
}

TEST(vector, givenElements_whenErasingAndInsertingInsertingAtIndex_thenItemIsReplaced) {
    auto sut = stl::vector<int>();
    sut.push_back(1);
    sut.push_back(3);
    sut.push_back(4);
    sut.erase(sut.begin()+1);
    sut.insert(sut.begin()+1, 2);
    auto it = sut.begin();
    EXPECT_EQ(1, *(it++));
    EXPECT_EQ(2, *(it++));
    EXPECT_EQ(4, *(it++));
    EXPECT_EQ(sut.end(), it);
}

TEST(vector, givenElements_whenErasingEnd_thenLastElementIsRemoved) {
    auto sut = stl::vector<int>();
    sut.push_back(1);
    sut.push_back(2);
    sut.push_back(3);
    sut.erase(sut.end());
    auto it = sut.begin();
    EXPECT_EQ(1, *(it++));
    EXPECT_EQ(2, *(it++));
    EXPECT_EQ(sut.end(), it);
}

#pragma clang diagnostic pop
#endif
