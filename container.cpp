#include "container.h"

Container_Iterator::Container_Iterator(const Container* c)
    : container(c), current_index(0) {}

void Container_Iterator::First() {
    current_index = 0;
}

void Container_Iterator::Next() {
    ++current_index;
}

bool Container_Iterator::Is_Done() const {
    return container == nullptr || current_index >= container->Get_Size();
}

GPU* Container_Iterator::Current_Item() const {
    if (Is_Done()) {
        return nullptr;
    }
    return container->Get_Item(current_index);
}

Container::Container() {}

Container::~Container() {}

Container::Container(const Container& other) {
    for (size_t i = 0; i < other.Get_Size(); ++i) {
        items.push_back(other.Get_Item(i)->Copy());
    }
}

Container& Container::operator=(const Container& other) {
    if (this != &other) {
        items.clear();
        for (size_t i = 0; i < other.Get_Size(); ++i) {
            items.push_back(other.Get_Item(i)->Copy());
        }
    }
    return *this;
}

Container::Container(Container&& other) noexcept : items(std::move(other.items)) {}

Container& Container::operator=(Container&& other) noexcept {
    if (this != &other) {
        items = std::move(other.items);
    }
    return *this;
}

void Container::Add(std::unique_ptr<GPU> item) {
    items.push_back(std::move(item));
}

size_t Container::Get_Size() const {
    return items.size();
}

GPU* Container::Get_Item(size_t index) const {
    if (index < items.size()) {
        return items[index].get();
    }
    return nullptr;
}

std::unique_ptr<Iterator> Container::Create_Iterator() const {
    return std::make_unique<Container_Iterator>(this);
}

Sorted_Container_Iterator::Sorted_Container_Iterator(const Sorted_Container* c)
    : container(c), current_index(0) {}

void Sorted_Container_Iterator::First() { current_index = 0; }

void Sorted_Container_Iterator::Next() { ++current_index; }

bool Sorted_Container_Iterator::Is_Done() const {
    return container == nullptr || current_index >= container->Get_Size();
}

GPU* Sorted_Container_Iterator::Current_Item() const {
    if (Is_Done()) return nullptr;
    return container->Get_Item(current_index);
}

Sorted_Container::Sorted_Container(const Sorted_Container& other) {
    for (size_t i = 0; i < other.Get_Size(); ++i) {
        items.push_back(other.Get_Item(i)->Copy());
    }
}

Sorted_Container& Sorted_Container::operator=(const Sorted_Container& other) {
    if (this != &other) {
        items.clear();
        for (size_t i = 0; i < other.Get_Size(); ++i) {
            items.push_back(other.Get_Item(i)->Copy());
        }
    }
    return *this;
}

void Sorted_Container::Add(std::unique_ptr<GPU> item) {
    // Вставка с сохранением сортировки по оператору < класса GPU
    auto it = items.begin();
    while (it != items.end() && !(*item < *(*it))) {
        ++it;
    }
    items.insert(it, std::move(item));
}

size_t Sorted_Container::Get_Size() const { return items.size(); }

GPU* Sorted_Container::Get_Item(size_t index) const {
    if (index < items.size()) return items[index].get();
    return nullptr;
}

std::unique_ptr<Iterator> Sorted_Container::Create_Iterator() const {
    return std::make_unique<Sorted_Container_Iterator>(this);
}

Iterator_Decorator::Iterator_Decorator(std::unique_ptr<Iterator> it) : wrappee(std::move(it)) {}

void Iterator_Decorator::AdvanceToValid() {
    while (!wrappee->Is_Done() && !Is_Valid(wrappee->Current_Item())) {
        wrappee->Next();
    }
}

void Iterator_Decorator::First() {
    wrappee->First();
    AdvanceToValid();
}

void Iterator_Decorator::Next() {
    wrappee->Next();
    AdvanceToValid();
}

bool Iterator_Decorator::Is_Done() const {
    return wrappee->Is_Done();
}

GPU* Iterator_Decorator::Current_Item() const {
    return wrappee->Current_Item();
}

VRAM_Filter_Decorator::VRAM_Filter_Decorator(std::unique_ptr<Iterator> it, int vram)
    : Iterator_Decorator(std::move(it)), min_vram(vram) {}

bool VRAM_Filter_Decorator::Is_Valid(GPU* item) const {
    if (!item) return false;
    return item->Get_VRAM() >= min_vram;
}