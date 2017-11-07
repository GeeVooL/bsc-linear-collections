#ifndef AISDI_LINEAR_VECTOR_H
#define AISDI_LINEAR_VECTOR_H

#define INIT_BUFFER_SIZE 64 // AN ITERATION OF 2!

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace aisdi
{

template <typename Type>
class Vector
{
  private:
    size_t size;
    size_t bufCapacity;
    char *buffer;
    Type *bufBegin;
    Type *next;
    Type *bufEnd;

    void increaseSize(size_t requiredSize)
    {
        size_t newCapacity = calcCapacity(requiredSize);

        char* newBuf = new char[sizeof(Type) * (newCapacity + 1)];
        
        Type* orgNext = next;
        Type* orgBufBegin = bufBegin;

        bufCapacity = newCapacity;
        next = reinterpret_cast<Type*>(newBuf) + (next - bufBegin);
        bufBegin = reinterpret_cast<Type*>(newBuf);
        bufEnd = bufBegin + bufCapacity;

        if (size > 0)
        {   
            Type* ptr = orgBufBegin;
            Type* tmpNext = bufBegin;
            while (ptr != orgNext)
            {
                new(tmpNext) Type(*ptr);
                ptr->~Type();
                ++ptr;
                ++tmpNext;
            }
        }
        
        delete [] buffer;
        buffer = newBuf;
    }

    void clear()
    {
        while (!isEmpty())
        {
            (next - 1)->~Type();
            --next;
            --size;
        }
    }

    size_t calcCapacity(size_t requiredSize)
    {
        size_t newCapacity = INIT_BUFFER_SIZE;
        while (newCapacity <= requiredSize)
            newCapacity = newCapacity << 1;
        return newCapacity;
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

    Vector() : Vector(INIT_BUFFER_SIZE) {}

    Vector(size_t capacity) : size(0), bufCapacity(capacity)
    {
        buffer = new char[sizeof(Type) * (bufCapacity + 1)];
        bufBegin = reinterpret_cast<Type *>(buffer);
        next = bufBegin;
        bufEnd = bufBegin + bufCapacity;
    }

    Vector(std::initializer_list<Type> l) : Vector(calcCapacity(l.size()))
    {
        for (const Type& el : l)
            append(el);
    }

    Vector(const Vector &other) : Vector(calcCapacity(other.size)) 
    {
        for (auto i = other.begin(); i != other.end(); i++)
            append(*i);
    }

    Vector(Vector &&other) : size(0), bufCapacity(0), buffer(nullptr), bufBegin(nullptr), next(nullptr), bufEnd(nullptr)
    {
        *this = std::move(other);
    }

    ~Vector()
    {
        if (buffer)
        {
            clear();
            delete [] buffer;
        }
    }

    Vector &operator=(const Vector &other)
    {
        if (this != &other)
        {
            clear();
            for (auto i = other.begin(); i != other.end(); i++)
                append(*i);
        }

        return *this;
    }

    Vector &operator=(Vector &&other)
    {
        if (this != &other)
        {
            if (buffer)
            {
                clear();
                delete [] buffer;
            }

            size = other.size;
            bufCapacity = other.bufCapacity;
            buffer = other.buffer;
            bufBegin = other.bufBegin;
            next = other.next;
            bufEnd = other.bufEnd;

            other.size = 0;
            other.bufCapacity = 0;
            other.buffer = nullptr;
            other.bufBegin = nullptr;
            other.next = nullptr;
            other.bufEnd = nullptr;
        }

        return *this;
    }

    bool isEmpty() const 
    {
        return bufBegin == next;
    }

    size_type getSize() const
    {
        return size;
    }

    void append(const Type &item)
    {
        if (size == bufCapacity)
            increaseSize(size + 1);

        new(next) Type(item);
        ++next;
        ++size;
    }

    void prepend(const Type &item)
    {
        if (isEmpty())
        {
            append(item);
            return;
        }

        if (size == bufCapacity)
            increaseSize(size + 1);
        
        
        Type* tmp = next - 1;
        Type* tmpNext = next;
        while (tmp != bufBegin)
        {
            new(tmpNext) Type(*tmp);
            tmp->~Type();
            --tmp;
            --tmpNext;
        }

        new(tmpNext) Type(*tmp);
        tmp->~Type();

        new(bufBegin) Type(item);
        ++next;
        ++size;
    }

    void insert(const const_iterator &insertPosition, const Type &item)
    {
        if (insertPosition == cend())
        {
            append(item);
            return;
        }

        if (insertPosition == cbegin())
        {
            prepend(item);
            return;
        }

        if (size == bufCapacity)
            increaseSize(size + 1);

        Type* tmp = next - 1;
        Type* tmpNext = next;
        while (tmp != insertPosition.ptr)
        {
            new(tmpNext) Type(*tmp);
            tmp->~Type();
            --tmp;
            --tmpNext;
        }

        new(tmpNext) Type(*tmp);
        tmp->~Type();
        
        new(insertPosition.ptr) Type(item);
        ++next;
        ++size;

    }    

    Type popFirst()
    {
        if (isEmpty())
            throw std::logic_error("Collection already empty");

        Type ret = *bufBegin;
        bufBegin->~Type();
        Type* tmp = bufBegin + 1;
        Type* tmpNext = bufBegin;
        while (tmp != next)
        {
            new(tmpNext) Type(*tmp);
            tmp->~Type();
            ++tmp;
            ++tmpNext;
        }
        --next;
        --size;
        return ret;
    }

    Type popLast() 
    { 
        if (isEmpty())
            throw std::logic_error("Collection already empty"); 
        
        Type tmp = *(next - 1);
        (next - 1)->~Type();
        --next;
        --size;
        return tmp;
    }

    void erase(const const_iterator &position)
    {
        if (isEmpty() || position == cend())
            throw std::out_of_range("Position out of range");

        position.ptr->~Type();
        Type *tmp = position.ptr + 1;
        Type *tmpNext = position.ptr;
        while (tmp != next)
        {
            new (tmpNext) Type(*tmp);
            tmp->~Type();
            ++tmp;
            ++tmpNext;
        }
        --next;
        --size;
    }

    void erase(const const_iterator &firstIncluded,
               const const_iterator &nextExcluded)
    {
        if (isEmpty())
            throw std::out_of_range("Collection already empty");

        difference_type rangeSize = nextExcluded.ptr - firstIncluded.ptr;

        Type *tmp = firstIncluded.ptr + rangeSize;
        Type *tmpNext = firstIncluded.ptr;
        auto i = 0;
        while (tmp != next)
        {
            if (i < rangeSize)
                tmpNext->~Type();
            new (tmpNext) Type(*tmp);
            tmp->~Type();
            ++tmp;
            ++tmpNext;
        }
        next -= rangeSize;
        size -= rangeSize;
    }

    iterator begin() 
    {
        return iterator(bufBegin, bufBegin, next);
    }

    iterator end() 
    {
        return iterator(next, bufBegin, next);
    }

    const_iterator cbegin() const
    {
        return const_iterator(bufBegin, bufBegin, next);
    }

    const_iterator cend() const
    {
        return const_iterator(next, bufBegin, next);
    }

    const_iterator begin() const { return cbegin(); }

    const_iterator end() const { return cend(); }
};

template <typename Type>
class Vector<Type>::ConstIterator
{
  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = typename Vector::value_type;
    using difference_type = typename Vector::difference_type;
    using pointer = typename Vector::const_pointer;
    using reference = typename Vector::const_reference;

    Type* ptr;
    Type* begin;
    Type* next;

    explicit ConstIterator(Type* ptr, Type* begin, Type* next) : ptr(ptr), begin(begin), next(next)
    {
    }

    reference operator*() const 
    {
        if (ptr == next)
            throw std::out_of_range("This iterator does not point to a valid item");
    
        return *ptr;
    }

    ConstIterator &operator++()
    {
        if (ptr == next)
            throw std::out_of_range("The next iterator does not exist");

        ++ptr;
        return *this;
    }

    ConstIterator operator++(int)
    {
        ConstIterator tmp(ptr, begin, next);
        ++(*this);
        return tmp;
    }

    ConstIterator &operator--()
    {
        if (ptr == begin)
            throw std::out_of_range("The previous iterator does not exist");

        --ptr;
        return *this;
    }

    ConstIterator operator--(int)
    {
        ConstIterator tmp(ptr, begin, next);
        --(*this);
        return tmp;
    }

    ConstIterator operator+(difference_type d) const
    {
        difference_type distToNext = next - ptr;
        if (d > distToNext)
            throw std::out_of_range("Given iterator does not exist");

        ConstIterator tmp(ptr + d, begin, next);
        return tmp;
    }

    ConstIterator operator-(difference_type d) const
    {
        difference_type distToBeg = ptr - begin;
        if (d > distToBeg)
            throw std::out_of_range("Given iterator does not exist");

        ConstIterator tmp(ptr - d, begin, next);
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
class Vector<Type>::Iterator : public Vector<Type>::ConstIterator
{
  public:
    using pointer = typename Vector::pointer;
    using reference = typename Vector::reference;

    explicit Iterator(Type* ptr, Type* begin, Type* next) : ConstIterator(ptr, begin, next) {}

    Iterator(const ConstIterator &other) : ConstIterator(other) {}

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

#endif // AISDI_LINEAR_VECTOR_H
