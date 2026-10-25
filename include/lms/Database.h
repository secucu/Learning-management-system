#ifndef LMS_DATABASE_H
#define LMS_DATABASE_H
#include <sqlite3.h>
#include <stdexcept>
#include <string>

namespace lms {
class Statement {
    sqlite3_stmt* stmt_{};
public:
    Statement(sqlite3* db, const std::string& sql) {
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt_, nullptr) != SQLITE_OK)
            throw std::runtime_error(sqlite3_errmsg(db));
    }
    ~Statement() { sqlite3_finalize(stmt_); }
    Statement(const Statement&) = delete;
    Statement& operator=(const Statement&) = delete;
    void text(int i, const std::string& v) { if (sqlite3_bind_text(stmt_, i, v.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK) throw std::runtime_error("Bind failed"); }
    void integer(int i, int v) { if (sqlite3_bind_int(stmt_, i, v) != SQLITE_OK) throw std::runtime_error("Bind failed"); }
    void real(int i, double v) { if (sqlite3_bind_double(stmt_, i, v) != SQLITE_OK) throw std::runtime_error("Bind failed"); }
    bool row() { int r = sqlite3_step(stmt_); if (r == SQLITE_ROW) return true; if (r == SQLITE_DONE) return false; throw std::runtime_error(sqlite3_errmsg(sqlite3_db_handle(stmt_))); }
    void done() { if (sqlite3_step(stmt_) != SQLITE_DONE) throw std::runtime_error(sqlite3_errmsg(sqlite3_db_handle(stmt_))); }
    std::string str(int c) const { auto p = sqlite3_column_text(stmt_, c); return p ? reinterpret_cast<const char*>(p) : ""; }
    int num(int c) const { return sqlite3_column_int(stmt_, c); }
    double decimal(int c) const { return sqlite3_column_double(stmt_, c); }
};

class Database {
    sqlite3* db_{};
public:
    explicit Database(const std::string& file) {
        if (sqlite3_open(file.c_str(), &db_) != SQLITE_OK) throw std::runtime_error("Cannot open database");
        sqlite3_busy_timeout(db_, 5000);
    }
    ~Database() { sqlite3_close(db_); }
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;
    sqlite3* get() const { return db_; }
    void exec(const std::string& sql) {
        char* error = nullptr;
        if (sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &error) != SQLITE_OK) {
            std::string message = error ? error : "Database error";
            sqlite3_free(error);
            throw std::runtime_error(message);
        }
    }
};
}
#endif
