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
        for (const Type &i : l)
            append(i);
    }

    LinkedList(const LinkedList &other) : LinkedList()
    {
        for (auto i = other.begin(); i != other.end(); i++)
            append(*i);
    }

    LinkedList(LinkedList &&other) : sentinel(nullptr), first(nullptr), size(0)
    {
        *this = std::move(other);
    }

    ~LinkedList()
    {
        if (sentinel)
        {
            clear();
            delete sentinel;
        }

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

    LinkedList &operator=(LinkedList &&other)
    {
        if (this != &other)
        {
            if (sentinel)
                clear();
            
            sentinel = other.sentinel;
            first = other.first;
            size = other.size;

            other.sentinel = nullptr;
            other.first = nullptr;
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
            first = ptr;

        sentinel->prev->next = ptr;
        ptr->next = sentinel;
        ptr->prev = sentinel->prev;
        sentinel->prev = ptr;
        size++;
    }

    void prepend(const Type &item)
    {
        Node *ptr = new Node(item);
        first = ptr;
        sentinel->next->prev = ptr;
        ptr->next = sentinel->next;
        ptr->prev = sentinel;
        sentinel->next = ptr;
        size++;
    }

    void insert(const const_iterator &insertPosition, const Type &item)
    {
        if (insertPosition == cbegin())
        {
            prepend(item);
            return;
        }

        if (insertPosition == cend())
        {
            append(item);
            return;
        }

        Node *prev, *org;
        org = insertPosition.ptr;
        prev = org->prev;

        Node *ptr = new Node(item);
        prev->next = ptr;
        ptr->next = org;
        org->prev = ptr;
        ptr->prev = prev;
        size++;
    }

    Type popFirst()
    {
        if (isEmpty())
            throw std::logic_error("Collection already empty");

        Node *ptr = first;
        sentinel->next = ptr->next;
        ptr->next->prev = ptr->prev;
        first = sentinel->next;

        Type data = *(ptr->data);
        delete ptr;
        size--;
        return data;
    }

    Type popLast() 
    {
        if (isEmpty())
            throw std::logic_error("Collection already empty");

        Node *ptr = sentinel->prev;
        sentinel->prev = ptr->prev;
        ptr->prev->next = ptr->next;
        if (ptr == first)
            first = sentinel;

        Type data = *(ptr->data);
        delete ptr;
        size--;
        return data;
    }

    void erase(const const_iterator &position)
    {
        if (isEmpty() || position == cend())
            throw std::out_of_range("Position out of range");
        
        if (position == cbegin())
        {
            popFirst();
            return;
        }

        if (position == --cend())
        {
            popLast();
            return;
        }

        Node* org = position.ptr;
        org->prev->next = org->next;
        org->next->prev = org->prev;
        delete org;
        size--;
    }

    void erase(const const_iterator &firstIncluded, const const_iterator &lastExcluded)
    {
        Node *ptr = firstIncluded.ptr;
        Node *beg = ptr->prev;
        Node *last = lastExcluded.ptr;
        Node *next;

        if(firstIncluded == cbegin())
            first = last;

        while(ptr != last)
        {
            next = ptr->next;
            delete ptr;
            size--;
            ptr = next;
        }

        beg->next = last;
        last->prev = beg;
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

    char buffer[sizeof(Type)];
    Type *data;
    Node *next;
    Node *prev;

    Node() : next(this), prev(this) {}
    
    Node(const Type &item) : Node()
    {
        data = new(buffer) Type(item);
    }
    
    ~Node()
    {
        data->~Type();
    }
};

template <typename Type>
class LinkedList<Type>::ConstIterator
{
  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = typename LinkedList::value_type;
    using difference_type = typename LinkedList::difference_type;
    using pointer = typename LinkedList::const_pointer;
    using reference = typename LinkedList::const_reference;

    LinkedList<Type>::Node *ptr;
    LinkedList<Type>::Node *sentinel;

    explicit ConstIterator(LinkedList<Type>::Node *ptr, LinkedList<Type>::Node *sentinel) : ptr(ptr), sentinel(sentinel) 
    {
    }

    reference operator*() const
    {
        if (ptr == sentinel)
            throw std::out_of_range("This iterator does not point to a valid node");
        
        return *(ptr->data);
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

    ConstIterator operator+(difference_type d) const
    {
        ConstIterator tmp(ptr, sentinel);
        for (auto i = 0; i < d; i++)
            ++tmp;
        return tmp;
    }

    ConstIterator operator-(difference_type d) const
    {
        ConstIterator tmp(ptr, sentinel);
        for (auto i = 0; i < d; i++)
            --tmp;
        return tmp;
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
