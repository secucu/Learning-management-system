#ifndef LMS_UTILITIES_H
#define LMS_UTILITIES_H
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <array>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace lms {
inline std::string currentUtcTime() {
    auto t = std::time(nullptr); std::tm tm{}; gmtime_r(&t, &tm);
    std::ostringstream out; out << std::put_time(&tm, "%Y-%m-%d %H:%M:%S"); return out.str();
}
inline std::string hashPassword(const std::string& password, const std::string& salt) {
    std::array<unsigned char, 32> bytes{};
    if (!PKCS5_PBKDF2_HMAC(password.c_str(), static_cast<int>(password.size()),
        reinterpret_cast<const unsigned char*>(salt.data()), static_cast<int>(salt.size()),
        210000, EVP_sha256(), static_cast<int>(bytes.size()), bytes.data()))
        throw std::runtime_error("Password hashing failed");
    std::ostringstream out;
    for (auto byte : bytes) out << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    return out.str();
}
inline std::string generateSalt() {
    std::array<unsigned char, 16> bytes{};
    if (RAND_bytes(bytes.data(), static_cast<int>(bytes.size())) != 1) throw std::runtime_error("Secure random generation failed");
    std::ostringstream out;
    for (auto byte : bytes) out << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    return out.str();
}
inline std::string readInput(const std::string& prompt) { std::cout << prompt; std::string value; std::getline(std::cin, value); return value; }
inline int readNumber(const std::string& prompt, int minimum = 1, int maximum = 1000000) {
    for (;;) { try { auto value = readInput(prompt); std::size_t used = 0; int n = std::stoi(value, &used); if (used == value.size() && n >= minimum && n <= maximum) return n; } catch (...) {} std::cout << "Enter a whole number from " << minimum << " to " << maximum << ".\n"; }
}
inline double readScore(const std::string& prompt, double maximum) {
    for (;;) { try { auto value = readInput(prompt); std::size_t used = 0; double n = std::stod(value, &used); if (used == value.size() && n >= 0 && n <= maximum) return n; } catch (...) {} std::cout << "Enter a mark between 0 and " << maximum << ".\n"; }
}
inline std::string roleName(const std::string& role) { return role == "admin" ? "Administrator" : role == "lecturer" ? "Lecturer" : role == "hod" ? "Head of Department" : "Student"; }
}
#endif
