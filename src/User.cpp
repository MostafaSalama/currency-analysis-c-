#include "User.h"
#include "CSVReader.h"
#include <functional>
#include <random>
#include <chrono>
#include <sstream>

User::User()
: username(""),
  fullName(""),
  email(""),
  hashedPassword(0)
{
}

User::User(std::string _username,
           std::string _fullName,
           std::string _email,
           size_t _hashedPassword)
: username(_username),
  fullName(_fullName),
  email(_email),
  hashedPassword(_hashedPassword)
{
}

std::string User::getUsername() const
{
    return username;
}

std::string User::getFullName() const
{
    return fullName;
}

std::string User::getEmail() const
{
    return email;
}

size_t User::getHashedPassword() const
{
    return hashedPassword;
}

size_t User::hashPassword(const std::string& password)
{
    std::hash<std::string> hasher;
    return hasher(password);
}

std::string User::generateUsername()
{
    // Use current time as seed for randomness
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 generator(static_cast<unsigned int>(seed));
    
    // Generate a random 10-digit number
    std::uniform_int_distribution<long long> distribution(1000000000LL, 9999999999LL);
    long long username = distribution(generator);
    
    return std::to_string(username);
}

std::string User::toCSVLine() const
{
    std::ostringstream oss;
    oss << username << "," 
        << fullName << "," 
        << email << "," 
        << hashedPassword;
    return oss.str();
}

User User::fromCSVLine(const std::string& line)
{
    std::vector<std::string> tokens = CSVReader::tokenise(line, ',');
    
    if (tokens.size() != 4)
    {
        // Return empty user if invalid format
        return User();
    }
    
    std::string username = tokens[0];
    std::string fullName = tokens[1];
    std::string email = tokens[2];
    size_t hashedPassword = std::stoull(tokens[3]);
    
    return User(username, fullName, email, hashedPassword);
}
