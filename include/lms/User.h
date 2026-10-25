#ifndef LMS_USER_H
#define LMS_USER_H
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

namespace lms {
struct Account { int id; std::string username, role, department; };
class User {
protected:
    Account account_;
public:
    explicit User(Account account) : account_(std::move(account)) {}
    virtual ~User() = default;
    virtual std::string role() const = 0;
    virtual void menu() = 0;
    const Account& account() const { return account_; }
};
class AdminUser : public User { public: using User::User; std::string role() const override { return "admin"; } void menu() override; };
class LecturerUser : public User { public: using User::User; std::string role() const override { return "lecturer"; } void menu() override; };
class HodUser : public User { public: using User::User; std::string role() const override { return "hod"; } void menu() override; };
class StudentUser : public User { public: using User::User; std::string role() const override { return "student"; } void menu() override; };
class UserFactory {
public:
    static std::unique_ptr<User> make(Account account) {
        if (account.role == "admin") return std::make_unique<AdminUser>(account);
        if (account.role == "lecturer") return std::make_unique<LecturerUser>(account);
        if (account.role == "hod") return std::make_unique<HodUser>(account);
        if (account.role == "student") return std::make_unique<StudentUser>(account);
        throw std::runtime_error("Unknown role");
    }
};
}
#endif
