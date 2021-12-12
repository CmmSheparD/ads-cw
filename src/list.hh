#pragma once
#ifndef PARSING_TABLE_HH
#define PARSING_TABLE_HH

#include <memory>
#include <stdexcept>

namespace data_structs {


template<class T>
class List {
public:
    class Iterator;

    Iterator begin();
    Iterator end();

    List() : head_(nullptr), tail_(nullptr), size_(0) {}
    List(const List &src);
    List(List &&src);

    ~List() { clear(); }

    void push_back(const T &value);
    void push_front(const T &value);
    void pop_back();
    void pop_front();

    T &at(size_t i);
    void insert(const T &value, size_t i);
    void remove(size_t i);

    void clear();

    size_t size() { return size_; }
    bool empty() { return size_ == 0; }
private:
    template<class U>
    struct Node;

    std::shared_ptr<Node<T>> head_;
    std::shared_ptr<Node<T>> tail_;
    size_t size_;
};


template<class T>
template<class U>
struct List<T>::Node {
    Node(const U &data) : next(nullptr), prev(nullptr), data(data) {}

    std::shared_ptr<Node> next;
    std::shared_ptr<Node> prev;

    U data;
};


template<class T>
class List<T>::Iterator {
public:
    Iterator() : cur_(nullptr) {}
    Iterator(const Iterator &other) : cur_(other.cur) {}
    Iterator(const Iterator &&other) { cur_.swap(other.cur_); }

    ~Iterator() = default;

    // Iterator &operator++();
    // Iterator operator++(int);

    // Iterator &operator--();
    // Iterator operator--(int);

    // friend bool operator==(const Iterator &left, const Iterator &right);
    // friend bool operator!=(const Iterator &left, const Iterator &right);
private:
    std::shared_ptr<Node<T>> cur_;
};


template<class T>
List<T>::List(const List<T> &src) : size_(0)
{
    std::shared_ptr<Node<T>> cur = src.head_;
    while (cur) {
        push_back(cur->data);
        cur = cur->next;
    }
}

template<class T>
List<T>::List(List<T> &&src) : size_(src.size_), size_(src.size_)
{
    head_.swap(src.head_);
    tail_.swap(src.tail_);
}


template<class T>
void List<T>::push_back(const T &value)
{
    std::shared_ptr<Node<T>> node(new Node<T>(value));
    if (!tail_) {
        head_ = node;
    } else {
        tail_->next = node;
        node->prev = tail_;
    }
    tail_ = node;
    ++size_;
}

template<class T>
void List<T>::push_front(const T &value)
{
    std::shared_ptr<Node<T>> node(new Node<T>(value));
    if (!head_) {
        tail_ = node;
    } else {
        head_->prev = node;
        node->next = head_;
    }
    head_ = node;
    ++size_;
}

template<class T>
void List<T>::pop_back()
{
    if (!empty()) {
        tail_ = tail_->prev;
        if (!tail_)
            head_ = nullptr;
        else
            tail_->next = nullptr;
        --size_;
    }
}

template<class T>
void List<T>::pop_front()
{
    if (!empty()) {
        head_ = head_->next;
        if (!head_)
            tail_ = nullptr;
        else
            head_->prev = nullptr;
        --size_;
    }
}


template<class T>
T &List<T>::at(size_t i)
{
    if (i >= size_) {
        throw std::out_of_range("Index out of range.");
    } else if (i == 0) {
        return head_->data;
    } else if (i == size_ - 1) {
        return tail_->data;
    }
    std::shared_ptr<Node<T>> tmp = head_->next;
    for (size_t j = 1; j != i; ++j)
        tmp = tmp->next;
    return tmp->data;
}

template<class T>
void List<T>::insert(const T &value, size_t i)
{
    if (i > size_) // if i == size_, node will be inserted after the last
        throw std::out_of_range("Index out of range.");
    else if (i == 0)
        return push_front(value);
    else if (i == size_)
        return push_back(value);
    std::shared_ptr<Node<T>> node(new Node<T>(value));
    std::shared_ptr<Node<T>> tmp;
    if (i == size_ - 1) {
        tmp = tail_;
    } else {
        tmp = head_->next;
        for (size_t j = 1; j != i; ++j)
            tmp = tmp->next;
    }
    node->prev = tmp->prev;
    node->next = tmp;
    tmp->prev->next = node;
    tmp->prev = node;
    ++size_;
}

template<class T>
void List<T>::remove(size_t i)
{
    if (i >= size_) {
        throw std::out_of_range("Index out of range.");
    } else if (i == 0) {
        return pop_front();
    } else if (i == size_ - 1) {
        return pop_back();
    }
    std::shared_ptr<Node<T>> tmp = head_->next;
    for (size_t j = 1; j != i; ++j)
        tmp = tmp->next;
    tmp->prev->next = tmp->next;
    tmp->next->prev = tmp->prev;
    --size_;
}


template<class T>
void List<T>::clear()
{
    if (empty())
        return;
    tail_.reset();
    while (head_) {
       head_->prev.reset();
       head_ = head_->next;
    }
    size_ = 0;
}

}   // namespace data_structs

#endif  // PARSING_TABLE_HH
