#include "sqlite_container.h"
#include <iostream>


// Этот блок кода отвечает за открытие файла базы данных.
SQLite_Container::SQLite_Container(const std::string& db_name) {
    if (sqlite3_open(db_name.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Ошибка открытия БД: " << sqlite3_errmsg(db) << std::endl;
    }

    const char* sql = "CREATE TABLE IF NOT EXISTS gpus ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "type INTEGER, model TEXT, type_vram TEXT, "
                      "vram INTEGER, tdp INTEGER, extra_param INTEGER);";

    char* err_msg = nullptr;
    if (sqlite3_exec(db, sql, 0, 0, &err_msg) != SQLITE_OK) {
        std::cerr << "Ошибка создания таблицы: " << err_msg << std::endl;
        sqlite3_free(err_msg);
    }
}

SQLite_Container::~SQLite_Container() {
    sqlite3_close(db);
}

// Этот блок кода реализует сохранение объекта GPU в базу данных.
void SQLite_Container::Add(std::unique_ptr<GPU> item) {
    if (!item) return;

    std::string sql = "INSERT INTO gpus (type, model, type_vram, vram, tdp, extra_param) VALUES (?, ?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, static_cast<int>(item->Get_Type_Enum()));
        sqlite3_bind_text(stmt, 2, item->Get_Model().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, item->Get_Type_VRAM().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 4, item->Get_VRAM());
        sqlite3_bind_int(stmt, 5, item->Get_TDP());
        sqlite3_bind_int(stmt, 6, item->Get_Extra_Param());

        sqlite3_step(stmt);
    }
    sqlite3_finalize(stmt);
}

std::unique_ptr<Iterator> SQLite_Container::Create_Iterator() const {
    return std::make_unique<SQLite_Iterator>(db);
}


SQLite_Iterator::SQLite_Iterator(sqlite3* db_ptr) : db(db_ptr), stmt(nullptr), is_done(true) {}

SQLite_Iterator::~SQLite_Iterator() {
    if (stmt) sqlite3_finalize(stmt);
}

// Этот блок кода является фабрикой
void SQLite_Iterator::FetchCurrent() {
    if (is_done) {
        current_gpu.reset();
        return;
    }

    int type = sqlite3_column_int(stmt, 1);
    std::string model = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
    std::string type_vram = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
    int vram = sqlite3_column_int(stmt, 4);
    int tdp = sqlite3_column_int(stmt, 5);
    bool extra = sqlite3_column_int(stmt, 6) != 0;

    if (type == static_cast<int>(GPU_Type::RTX)) {
        current_gpu = std::make_unique<RTX_GPU>(model, type_vram, vram, tdp, extra);
    } else if (type == static_cast<int>(GPU_Type::INTEGRATED)) {
        current_gpu = std::make_unique<Integrated_GPU>(model, type_vram, vram, tdp, extra);
    } else if (type == static_cast<int>(GPU_Type::PRO)) {
        current_gpu = std::make_unique<Pro_GPU>(model, type_vram, vram, tdp, extra);
    }
}

// Этот блок кода управляет SQL-запросом на чтение
void SQLite_Iterator::First() {
    if (stmt) sqlite3_finalize(stmt);

    std::string sql = "SELECT * FROM gpus;";
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        is_done = (sqlite3_step(stmt) != SQLITE_ROW);
        FetchCurrent();
    } else {
        is_done = true;
    }
}

void SQLite_Iterator::Next() {
    if (!is_done) {
        is_done = (sqlite3_step(stmt) != SQLITE_ROW);
        FetchCurrent();
    }
}

bool SQLite_Iterator::Is_Done() const {
    return is_done;
}

GPU* SQLite_Iterator::Current_Item() const {
    return current_gpu.get();
}