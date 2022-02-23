#include "../src/list.hh"

#include <iostream>
#include <stdexcept>

#include <gtest/gtest.h>

using data_structs::List;


TEST(Base, Creation)
{
    List<int> list;
    ASSERT_EQ(list.size(), 0);
    ASSERT_TRUE(list.empty());
}

TEST(Base, GetSize)
{
    List<int> list;
    ASSERT_EQ(list.size(), 0);
}

TEST(Base, IsEmpty)
{
    List<int> list;
    ASSERT_TRUE(list.empty());
    list.push_back(0);
    ASSERT_FALSE(list.empty());
}

TEST(Base, CopyConstruct)
{
    List<int> list0;
    list0.push_back(0);
    list0.push_back(1);
    list0.push_back(2);
    list0.push_back(3);
    list0.push_back(4);
    List<int> list1 = list0;
    ASSERT_FALSE(list1.empty());
    ASSERT_EQ(list1.size(), list0.size());
    for (size_t i = 0; i < list0.size(); ++i)
        ASSERT_EQ(list0.at(i), list1.at(i));
}


class ListEmptyTest : public ::testing::Test {
protected:
    List<int> list_;
};

TEST_F(ListEmptyTest, PushBack)
{
    list_.push_back(0);
    ASSERT_EQ(list_.size(), 1);
    ASSERT_FALSE(list_.empty());
}

TEST_F(ListEmptyTest, PushFront)
{
    list_.push_front(0);
    ASSERT_EQ(list_.size(), 1);
    ASSERT_FALSE(list_.empty());
}

TEST_F(ListEmptyTest, PopBack)
{
    list_.pop_back();
    ASSERT_EQ(list_.size(), 0);
    ASSERT_TRUE(list_.empty());
}

TEST_F(ListEmptyTest, PopFront)
{
    list_.pop_front();
    ASSERT_EQ(list_.size(), 0);
    ASSERT_TRUE(list_.empty());
}

TEST_F(ListEmptyTest, Insert)
{
    ASSERT_THROW(list_.insert(0, 1), std::out_of_range);
    list_.insert(0, 0);
    ASSERT_EQ(list_.size(), 1);
    ASSERT_FALSE(list_.empty());
}

TEST_F(ListEmptyTest, Remove)
{
    ASSERT_THROW(list_.remove(0), std::out_of_range);
    ASSERT_THROW(list_.remove(2), std::out_of_range);
    ASSERT_EQ(list_.size(), 0);
    ASSERT_TRUE(list_.empty());
}

TEST_F(ListEmptyTest, At)
{
    ASSERT_THROW(list_.at(0), std::out_of_range);
    ASSERT_THROW(list_.at(2), std::out_of_range);
}

// TEST_F(ListEmptyTest, Set)
// {
//     ASSERT_THROW(list_.set(0, 1), std::out_of_range);
//     ASSERT_THROW(list_.set(2, 1), std::out_of_range);
// }

TEST_F(ListEmptyTest, Clear)
{
    list_.clear();
    ASSERT_EQ(list_.size(), 0);
    ASSERT_TRUE(list_.empty());
}


// TEST_F(ListEmptyTest, InsertList)
// {
//     List<int> list;
//     ASSERT_THROW(list_.insert(list, 1), std::out_of_range);
//     list_.insert(list, 0);
//     std::cout << "list_ " << list_ << std::endl;
//     ASSERT_EQ(list_.size(), 0);
//     ASSERT_TRUE(list_.empty());

//     list.push_back(0);
//     std::cout << "list " << list << std::endl;
//     std::cout << "list_ "<< list_ << std::endl;
//     list_.insert(list, 0);
//     std::cout << "list " << list << std::endl;
//     std::cout << "list_ "<< list_ << std::endl;
//     ASSERT_EQ(list_.size(), list.size());
//     ASSERT_FALSE(list.empty());
//     ASSERT_FALSE(list_.empty());
//     for (size_t i = 0; i < list.size(); ++i)
//         ASSERT_EQ(list.at(i), list_.at(i));

//     list_.clear();
//     list.push_back(0);
//     list.push_back(1);
//     list.push_back(2);
//     std::cout << "list " << list << std::endl;
//     std::cout << "list_ "<< list_ << std::endl;
//     list_.insert(list, 0);
//     std::cout << "list " << list << std::endl;
//     std::cout << "list_ "<< list_ << std::endl;
//     ASSERT_EQ(list_.size(), list.size());
//     ASSERT_FALSE(list.empty());
//     ASSERT_FALSE(list_.empty());
//     for (size_t i = 0; i < list.size(); ++i)
//         ASSERT_EQ(list.at(i), list_.at(i));
// }

TEST_F(ListEmptyTest, IteratorCreationTest)
{
    List<int>::Iterator beg, end;
    ASSERT_NO_THROW(beg = list_.begin());
    ASSERT_NO_THROW(end = list_.end());
    ASSERT_TRUE(beg == end);
}

TEST_F(ListEmptyTest, IteratorDereferenceIncPrefixTest)
{
    List<int>::Iterator beg = list_.begin();
    ASSERT_THROW(++beg, std::logic_error);
    ASSERT_THROW(*beg, std::logic_error);
}

TEST_F(ListEmptyTest, IteratorDereferenceIncPostfixTest)
{
    List<int>::Iterator beg = list_.begin();
    ASSERT_THROW(beg++, std::logic_error);
    ASSERT_THROW(*beg, std::logic_error);
}

TEST_F(ListEmptyTest, IteratorDereferenceDecPrefixTest)
{
    List<int>::Iterator end = list_.end();
    ASSERT_THROW(--end, std::out_of_range);
    ASSERT_THROW(*end, std::logic_error);
}

TEST_F(ListEmptyTest, IteratorDereferenceDecPostfixTest)
{
    List<int>::Iterator end = list_.end();
    ASSERT_THROW(end--, std::out_of_range);
    ASSERT_THROW(*end, std::logic_error);
}


class ListTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        list_.push_back(0);
        list_.push_back(1);
        list_.push_back(2);
        list_.push_back(3);
    }

    List<int> list_;
};

TEST_F(ListTest, PushBack)
{
    size_t s = list_.size() + 1;
    list_.push_back(0);
    ASSERT_EQ(list_.size(), s);
    ASSERT_FALSE(list_.empty());
}

TEST_F(ListTest, PushFront)
{
    size_t s = list_.size() + 1;
    list_.push_front(0);
    ASSERT_EQ(list_.size(), s);
    ASSERT_FALSE(list_.empty());
}

TEST_F(ListTest, PopBack)
{
    size_t s = list_.size() - 1;
    list_.pop_back();
    ASSERT_EQ(list_.size(), s);
    ASSERT_FALSE(list_.empty());
}

TEST_F(ListTest, PopFront)
{
    size_t s = list_.size() - 1;
    list_.pop_front();
    ASSERT_EQ(list_.size(), s);
    ASSERT_FALSE(list_.empty());
}

TEST_F(ListTest, Insert)
{
    size_t s = list_.size() + 1;
    ASSERT_THROW(list_.insert(0, s), std::out_of_range);

    list_.insert(5, 0);
    ASSERT_EQ(list_.size(), s);
    ASSERT_FALSE(list_.empty());

    list_.insert(5, s++);
    ASSERT_EQ(list_.size(), s);
    ASSERT_FALSE(list_.empty());

    list_.insert(10, s/2);
    ASSERT_EQ(list_.size(), ++s);
    ASSERT_FALSE(list_.empty());
}

TEST_F(ListTest, Remove)
{
    size_t s = list_.size();
    ASSERT_THROW(list_.remove(s), std::out_of_range);
    ASSERT_EQ(list_.size(), s);
    ASSERT_FALSE(list_.empty());

    list_.remove(s/2);
    ASSERT_EQ(list_.size(), --s);
    ASSERT_FALSE(list_.empty());

    list_.remove(0);
    ASSERT_EQ(list_.size(), --s);
    ASSERT_FALSE(list_.empty());

    list_.remove(--s);
    ASSERT_EQ(list_.size(), s);
    ASSERT_FALSE(list_.empty());
}

TEST_F(ListTest, At)
{
    ASSERT_THROW(list_.at(10), std::out_of_range);
    for (size_t i = 0; i < list_.size(); ++i)
        ASSERT_EQ(list_.at(i), i);
}

TEST_F(ListTest, SetByAt)
{
    int a = 10;
    for (size_t i = 0; i < list_.size(); ++i) {
        list_.at(i) = a;
        ASSERT_EQ(list_.at(i), a);
    }
}


// TEST_F(ListTest, Set)
// {
//     size_t s = list_.size();
//     ASSERT_THROW(list_.set(s, 1), std::out_of_range);
//     for (size_t i = 0; i < s; ++i) {
//         list_.set(i, s);
//         ASSERT_EQ(list_.at(i), s);
//     }
//     std::cout << list_ << std::endl;
// }

TEST_F(ListTest, Clear)
{
    list_.clear();
    ASSERT_EQ(list_.size(), 0);
    ASSERT_TRUE(list_.empty());
}

// TEST_F(ListTest, InsertEmptyList)
// {
//     List<int> list;
//     size_t s = list_.size();
//     list_.insert(list, 0);
//     ASSERT_EQ(list_.size(), s);
//     ASSERT_FALSE(list_.empty());
// }

// TEST_F(ListTest, InsertListAtHead)
// {
//     List<int> list;
//     size_t s = list_.size();
//     list.push_back(10);
//     list_.insert(list, 0);
//     ASSERT_EQ(list_.size(), s + list.size());
//     ASSERT_FALSE(list.empty());
//     ASSERT_FALSE(list_.empty());
//     for (size_t i = 0; i < list.size(); ++i)
//         ASSERT_EQ(list.at(i), list_.at(i));
// }

// TEST_F(ListTest, InsertListInMiddle)
// {
//     List<int> list;
//     size_t s = list_.size();
//     list.push_back(0);
//     list.push_back(1);
//     list.push_back(2);
//     list_.insert(list, s/2);
//     ASSERT_EQ(list_.size(), s + list.size());
//     ASSERT_FALSE(list.empty());
//     ASSERT_FALSE(list_.empty());
//     for (size_t j = 0, i  = s/2; j < list.size(); ++i, ++j)
//         ASSERT_EQ(list.at(j), list_.at(i));
// }

// TEST_F(ListTest, InsertListAtTail)
// {
//     List<int> list;
//     size_t s = list_.size();
//     list.push_front(0);
//     list.push_front(1);
//     list.push_front(2);
//     list_.insert(list, s);
//     ASSERT_EQ(list_.size(), s + list.size());
//     ASSERT_FALSE(list.empty());
//     ASSERT_FALSE(list_.empty());
//     for (size_t j = 0, i  = s; j < list.size(); ++i, ++j)
//         ASSERT_EQ(list.at(j), list_.at(i));
// }

TEST_F(ListTest, IteratorCreationTest)
{
    List<int>::Iterator beg, end;
    ASSERT_NO_THROW(beg = list_.begin());
    ASSERT_NO_THROW(end = list_.end());
    ASSERT_FALSE(beg == end);
    ASSERT_TRUE(beg != end);
}

TEST_F(ListTest, IteratorDereferencePrefixTest)
{
    List<int>::Iterator beg = list_.begin();
    for (size_t i = 0; i < list_.size(); ++i) {
        ASSERT_EQ(*beg, list_.at(i));
        ++beg;
    }
    ASSERT_THROW(++beg, std::out_of_range);
    ASSERT_THROW(*beg, std::out_of_range);
}

TEST_F(ListTest, IteratorDereferencePostfixTest)
{
    List<int>::Iterator beg = list_.begin();
    for (size_t i = 0; i < list_.size(); ++i) {
        ASSERT_EQ(*beg, list_.at(i));
        beg++;
    }
    ASSERT_THROW(beg++, std::out_of_range);
    ASSERT_THROW(*beg, std::out_of_range);
}
