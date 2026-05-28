#ifndef LABA5_2_CONTAINER_H
#define LABA5_2_CONTAINER_H
#pragma once

#include <vector>
#include <memory>
#include "gpu.h"

class Container;
class Sorted_Container;

class Iterator {
public:
    virtual ~Iterator() = default;
    virtual void First() = 0;
    virtual void Next() = 0;
    virtual bool Is_Done() const = 0;
    virtual GPU* Current_Item() const = 0;
};

class Container_Iterator : public Iterator {
private:
    const Container* container;
    size_t current_index;

public:
    Container_Iterator(const Container* c);
    void First() override;
    void Next() override;
    bool Is_Done() const override;
    GPU* Current_Item() const override;
};

class Container {
private:
    std::vector<std::unique_ptr<GPU>> items;

public:
    Container();
    ~Container();

    Container(const Container& other);
    Container& operator=(const Container& other);

    Container(Container&& other) noexcept;
    Container& operator=(Container&& other) noexcept;

    void Add(std::unique_ptr<GPU> item);
    size_t Get_Size() const;
    GPU* Get_Item(size_t index) const;

    std::unique_ptr<Iterator> Create_Iterator() const;
};

class Sorted_Container_Iterator : public Iterator {
private:
    const Sorted_Container* container;
    size_t current_index;

public:
    Sorted_Container_Iterator(const Sorted_Container* c);
    void First() override;
    void Next() override;
    bool Is_Done() const override;
    GPU* Current_Item() const override;
};

class Sorted_Container {
private:
    std::vector<std::unique_ptr<GPU>> items;

public:
    Sorted_Container() = default;
    ~Sorted_Container() = default;

    Sorted_Container(const Sorted_Container& other);
    Sorted_Container& operator=(const Sorted_Container& other);

    Sorted_Container(Sorted_Container&& other) noexcept = default;
    Sorted_Container& operator=(Sorted_Container&& other) noexcept = default;

    void Add(std::unique_ptr<GPU> item);
    size_t Get_Size() const;
    GPU* Get_Item(size_t index) const;

    std::unique_ptr<Iterator> Create_Iterator() const;
};

class Iterator_Decorator : public Iterator {
protected:
    std::unique_ptr<Iterator> wrappee;
    void AdvanceToValid();

public:
    Iterator_Decorator(std::unique_ptr<Iterator> it);
    void First() override;
    void Next() override;
    bool Is_Done() const override;
    GPU* Current_Item() const override;

    virtual bool Is_Valid(GPU* item) const = 0;
};

template <typename T>
class Family_Filter_Decorator : public Iterator_Decorator {
public:
    Family_Filter_Decorator(std::unique_ptr<Iterator> it) : Iterator_Decorator(std::move(it)) {}
    bool Is_Valid(GPU* item) const override {
        return item != nullptr && dynamic_cast<T*>(item) != nullptr;
    }
};

class VRAM_Filter_Decorator : public Iterator_Decorator {
private:
    int min_vram;
public:
    VRAM_Filter_Decorator(std::unique_ptr<Iterator> it, int vram);
    bool Is_Valid(GPU* item) const override;
};

#endif //LABA5_2_CONTAINER_H
