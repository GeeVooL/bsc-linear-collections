#ifndef AISDI_LINEAR_LINKEDLIST_H
#define AISDI_LINEAR_LINKEDLIST_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <iostream>

namespace aisdi
{

template <typename Type>
class LinkedList
{
  private:
    class Node;
    Node *sentinel;
    Node *first;
    std::size_t size;

    void clear()
    {
        Node *ptr = first;
        Node *next;
        while(ptr != sentinel)
        {
            next = ptr->next;
            delete ptr;
            ptr = next;
        }

        sentinel->prev = sentinel;
        sentinel->next = sentinel;
        first = sentinel;
    }

  public:
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;
    using value_type = Type;
    using pointer = Type *;
    using reference = Type &;
    using const_pointer = const Type *;
    using const_reference = const Type &;

    class ConstIterator;
    class Iterator;
    using iterator = Iterator;
    using const_iterator = ConstIterator;

    LinkedList()
    {
        sentinel = new Node();
        first = sentinel;
        size = 0;
    }

    LinkedList(std::initializer_list<Type> l) : LinkedList()
    {
        for (auto i : l)
            append(i);
    }

    LinkedList(const LinkedList &other) : LinkedList()
    {
        for (auto i = other.begin(); i != other.end(); i++)
            append(*i);
    }

    LinkedList(LinkedList &&other) : sentinel(other.sentinel), first(other.first), size(other.size)
    {
        other.sentinel = new Node();
        other.first = other.sentinel;
        other.size = 0;
    }

    ~LinkedList()
    {
        clear();
        
        delete sentinel;
    }

    LinkedList &operator=(const LinkedList &other)
    {
        if (this != &other)
        {
            clear();
            for (auto i = other.begin(); i != other.end(); i++)
                append(*i);
        }

        return *this;
    }

    LinkedList &operator=(LinkedList &&other) //TODO: FIX
    {
        if (this != &other)
        {
            clear();
            
            sentinel = other.sentinel;
            first = other.first;
            size = other.size;

            other.sentinel = new Node();
            other.first = sentinel;
            other.size = 0;
        }

        return *this;
    }

    bool isEmpty() const
    {
        return first == sentinel;
    }

    size_type getSize() const
    {
        return size;
    }

    void append(const Type &item)
    {
        Node *ptr = new Node(item);
        
        if (isEmpty())
        {
            first = ptr;
        }

        sentinel->prev->next = ptr;
        ptr->next = sentinel;
        ptr->prev = sentinel->prev;
        sentinel->prev = ptr;
        size++;
    }

    void prepend(const Type &item) //TODO: FIX
    {
        Node *ptr = new Node(item);
        if (isEmpty())
        {
            first = ptr;
        }
        sentinel->next->prev = ptr;
        ptr->next = sentinel->next;
        ptr->prev = sentinel;
        sentinel->next = ptr;
        size++;
    }

    void insert(const const_iterator &insertPosition, const Type &item) //TODO: IMPLEMENT
    {
        (void)insertPosition;
        (void)item;
        throw std::runtime_error("TODO");
    }

    Type popFirst()
    {
        if (isEmpty())
        {
            throw std::logic_error("Collection already empty");
        }

        Node *ptr = first;
        sentinel->next = ptr->next;
        ptr->next->prev = ptr->prev;
        first = sentinel->next;

        Type data = ptr->data;
        delete ptr;
        size--;
        return data;
    }

    Type popLast() //TODO: FIX
    {
        if (isEmpty())
        {
            throw std::logic_error("Collection already empty");
        }

        Node *ptr = sentinel->prev;
        sentinel->prev = ptr->prev;
        ptr->prev->next = ptr->next;

        Type data = ptr->data;
        delete ptr;
        size--;
        return data;
    }

    void erase(const const_iterator &possition)
    {
        (void)possition;
        throw std::runtime_error("TODO");
    }

    void erase(const const_iterator &firstIncluded, const const_iterator &lastExcluded)
    {
        (void)firstIncluded;
        (void)lastExcluded;
        throw std::runtime_error("TODO");
    }

    iterator begin()
    {
        return iterator(first, sentinel);
    }

    iterator end()
    {
        return iterator(sentinel, sentinel);
    }

    const_iterator cbegin() const
    {
        return const_iterator(first, sentinel);
    }

    const_iterator cend() const
    {
        return const_iterator(sentinel, sentinel);
    }

    const_iterator begin() const
    {
        return cbegin();
    }

    const_iterator end() const
    {
        return cend();
    }
};

template <typename Type>
class LinkedList<Type>::Node
{
  public:
    Type data;
    Node *next;
    Node *prev;

    Node() : next(this), prev(this) {}
    Node(const Type &data) : data(data), next(nullptr), prev(nullptr) {}
    ~Node() {}
};

template <typename Type>
class LinkedList<Type>::ConstIterator
{
  private:
    LinkedList<Type>::Node *ptr;
    LinkedList<Type>::Node *sentinel;

  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = typename LinkedList::value_type;
    using difference_type = typename LinkedList::difference_type;
    using pointer = typename LinkedList::const_pointer;
    using reference = typename LinkedList::const_reference;

    explicit ConstIterator(LinkedList<Type>::Node *ptr, LinkedList<Type>::Node *sentinel) : ptr(ptr), sentinel(sentinel) 
    {
    }

    reference operator*() const
    {
        if (ptr == sentinel)
            throw std::out_of_range("This iterator does not point to a valid node");
        
        return ptr->data;
    }

    ConstIterator &operator++()
    {
        if (ptr == sentinel)
            throw std::out_of_range("The next iterator does not exist");

        ptr = ptr->next;
        return *this;
    }

    ConstIterator operator++(int)
    {
        ConstIterator tmp(ptr, sentinel);
        ++(*this);
        return tmp;
    }

    ConstIterator &operator--()
    {
        if (ptr == sentinel->next)
            throw std::out_of_range("The previous iterator does not exist");
        
        ptr = ptr->prev;
        return *this;
    }

    ConstIterator operator--(int)
    {
        ConstIterator tmp(ptr, sentinel);
        --(*this);
        return tmp;
    }

    ConstIterator operator+(difference_type d) const //TODO: IMPLEMENT
    {
        (void)d;
        throw std::runtime_error("TODO");
    }

    ConstIterator operator-(difference_type d) const //TODO: IMPLEMENT
    {
        (void)d;
        throw std::runtime_error("TODO");
    }

    bool operator==(const ConstIterator &other) const
    {
        return this->ptr == other.ptr;
    }

    bool operator!=(const ConstIterator &other) const
    {
        return !(*this == other);
    }
};

template <typename Type>
class LinkedList<Type>::Iterator : public LinkedList<Type>::ConstIterator
{
  public:
    using pointer = typename LinkedList::pointer;
    using reference = typename LinkedList::reference;

    explicit Iterator(LinkedList<Type>::Node *ptr, LinkedList<Type>::Node *sentinel) : ConstIterator(ptr, sentinel)
    {
    }

    Iterator(const ConstIterator &other)
        : ConstIterator(other)
    {
    }

    Iterator &operator++()
    {
        ConstIterator::operator++();
        return *this;
    }

    Iterator operator++(int)
    {
        auto result = *this;
        ConstIterator::operator++();
        return result;
    }

    Iterator &operator--()
    {
        ConstIterator::operator--();
        return *this;
    }

    Iterator operator--(int)
    {
        auto result = *this;
        ConstIterator::operator--();
        return result;
    }

    Iterator operator+(difference_type d) const
    {
        return ConstIterator::operator+(d);
    }

    Iterator operator-(difference_type d) const
    {
        return ConstIterator::operator-(d);
    }

    reference operator*() const
    {
        // ugly cast, yet reduces code duplication.
        return const_cast<reference>(ConstIterator::operator*());
    }
};
}

#endif // AISDI_LINEAR_LINKEDLIST_H
