#ifndef AISDI_LINEAR_LINKEDLIST_H
#define AISDI_LINEAR_LINKEDLIST_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

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
        sentinel->prev = sentinel;
        sentinel->next = sentinel;
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
            append(i);
    }

    LinkedList(LinkedList &&other)
    {
        (void)other;
        throw std::runtime_error("TODO");
    }

    ~LinkedList()
    {
    }

    LinkedList &operator=(const LinkedList &other)
    {
        (void)other;
        throw std::runtime_error("TODO");
    }

    LinkedList &operator=(LinkedList &&other)
    {
        (void)other;
        throw std::runtime_error("TODO");
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

    void prepend(const Type &item)
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

    void insert(const const_iterator &insertPosition, const Type &item)
    {
        (void)insertPosition;
        (void)item;
        throw std::runtime_error("TODO");
    }

    Type popFirst()
    {
        if (!isEmpty())
        {
            Node *ptr = first;
            sentinel->next = ptr->next;
            ptr->next->prev = ptr->prev;
            first = sentinel->next;

            Type data = ptr->data;
            delete ptr;
            return data;
        }

        throw std::logic_error();
    }

    Type popLast()
    {
        if (!isEmpty())
        {
            Node *ptr = sentinel->prev;
            sentinel->prev = ptr->prev;
            ptr->prev->next = ptr->next;

            Type data = ptr->data;
            delete ptr;
            return data;
        }

        throw std::logic_error();
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
        return iterator(first);
    }

    iterator end()
    {
        return iterator(sentinel);
    }

    const_iterator cbegin() const
    {
        return const_iterator(first);
    }

    const_iterator cend() const
    {
        return const_iterator(sentinel)
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

    Node() : next(nullptr), prev(nullptr) {}
    Node(const Type &data) : data(data), next(nullptr), prev(nullptr) {}
};

template <typename Type>
class LinkedList<Type>::ConstIterator
{
  private:
    LinkedList<Type>::Node *ptr;

  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = typename LinkedList::value_type;
    using difference_type = typename LinkedList::difference_type;
    using pointer = typename LinkedList::const_pointer;
    using reference = typename LinkedList::const_reference;

    explicit ConstIterator(LinkedList<Type>::Node *ptr)
    {
        this->ptr = ptr;
    }

    reference operator*() const
    {
        if (ptr == LinkedList<Type>::sentinel)
            throw std::out_of_range("This iterator does not point to a valid node");
        
        return ptr->data;
    }

    ConstIterator &operator++()
    {
        if (ptr == LinkedList<Type>::sentinel)
            throw std::out_of_range("The next iterator does not exist");

        ptr = ptr->next;
        return *this;
    }

    ConstIterator operator++(int)
    {
        ConstIterator tmp(ptr);
        ++(*this);
        return tmp;
    }

    ConstIterator &operator--()
    {
        if (ptr == LinkedList<Type>::first)
            throw std::out_of_range("The previous iterator does not exist");
        
        ptr = ptr->prev;
        return *this;
    }

    ConstIterator operator--(int)
    {
        ConstIterator tmp(ptr);
        --(*this);
        return tmp;
    }

    ConstIterator operator+(difference_type d) const
    {
        (void)d;
        throw std::runtime_error("TODO");
    }

    ConstIterator operator-(difference_type d) const
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
  private:
    LinkedList<Type>::Node *ptr;

  public:
    using pointer = typename LinkedList::pointer;
    using reference = typename LinkedList::reference;

    explicit Iterator(LinkedList<Type>::Node *ptr)
    {
        this->ptr = ptr;
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
