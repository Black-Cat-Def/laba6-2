#ifndef LABA5_2_SQLITE_CONTAINER_H
#define LABA5_2_SQLITE_CONTAINER_H
#pragma once

#include "container.h"
#include "sqlite3.h"
#include <string>
#include <memory>

class SQLite_Container;

// Класс итератора для базы данных.
class SQLite_Iterator : public Iterator {
private:
    sqlite3* db;
    sqlite3_stmt* stmt;
    bool is_done;
    std::unique_ptr<GPU> current_gpu;

    void FetchCurrent();

public:
    SQLite_Iterator(sqlite3* db_ptr);
    ~SQLite_Iterator() override;

    void First() override;
    void Next() override;
    bool Is_Done() const override;
    GPU* Current_Item() const override;
};

// Этот блок кода объявляет контейнер, работающий с SQLite.
class SQLite_Container {
private:
    sqlite3* db;

public:
    SQLite_Container(const std::string& db_name);
    ~SQLite_Container();

    SQLite_Container(const SQLite_Container&) = delete;
    SQLite_Container& operator=(const SQLite_Container&) = delete;

    void Add(std::unique_ptr<GPU> item);
    std::unique_ptr<Iterator> Create_Iterator() const;
};



#endif //LABA5_2_SQLITE_CONTAINER_H
