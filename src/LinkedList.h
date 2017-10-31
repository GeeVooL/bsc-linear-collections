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
    class Node
    {
      public:
        Type data;
        Node *next;
        Node *prev;

        Node() : next(nullptr), prev(nullptr) {}
        Node(const Type &data) : data(data), next(nullptr), prev(nullptr) {}
    };

    Node *sentinel; //public or private?
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

    LinkedList(std::initializer_list<Type> l)
    {
        for (auto i : l)
            append(i);
    }

    LinkedList(const LinkedList &other)
    {
        for (auto i : other)
        {
            append(i);
        }
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
        throw std::runtime_error("TODO");
    }

    Type popLast()
    {
        throw std::runtime_error("TODO");
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
        throw std::runtime_error("TODO");
    }

    iterator end()
    {
        throw std::runtime_error("TODO");
    }

    const_iterator cbegin() const
    {
        throw std::runtime_error("TODO");
    }

    const_iterator cend() const
    {
        throw std::runtime_error("TODO");
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
class LinkedList<Type>::ConstIterator
{
  public:
    // using iterator_category = std::bidirectional_iterator_tag; TODO: What is wrong here?
    using value_type = typename LinkedList::value_type;
    using difference_type = typename LinkedList::difference_type;
    using pointer = typename LinkedList::const_pointer;
    using reference = typename LinkedList::const_reference;

    explicit ConstIterator()
    {
    }

    reference operator*() const
    {
        throw std::runtime_error("TODO");
    }

    ConstIterator &operator++()
    {
        throw std::runtime_error("TODO");
    }

    ConstIterator operator++(int)
    {
        throw std::runtime_error("TODO");
    }

    ConstIterator &operator--()
    {
        throw std::runtime_error("TODO");
    }

    ConstIterator operator--(int)
    {
        throw std::runtime_error("TODO");
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
        (void)other;
        throw std::runtime_error("TODO");
    }

    bool operator!=(const ConstIterator &other) const
    {
        (void)other;
        throw std::runtime_error("TODO");
    }
};

template <typename Type>
class LinkedList<Type>::Iterator : public LinkedList<Type>::ConstIterator
{
  public:
    using pointer = typename LinkedList::pointer;
    using reference = typename LinkedList::reference;

    explicit Iterator()
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
