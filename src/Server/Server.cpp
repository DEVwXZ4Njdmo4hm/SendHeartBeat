#include <print>
#include <asio.hpp>
#include <pfr.hpp>
#include <sqlite3.h>

int main()
{
    std::println("Hello, this is Server!");
    std::println("ASIO Version: {}", ASIO_VERSION);
    std::println("PFR Version: {}", PFR_VERSION);
    std::println("SQLite3 Version: {}", SQLITE_VERSION);

    return 0;
}
