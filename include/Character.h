#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include <vector>

// Abstract Base Class demonstrating Abstraction and Polymorphism
class Character {
protected:
    std::string m_id;
    std::string m_name;
    std::string m_occupation;
    std::string m_personality;
    std::string m_greeting;

public:
    Character(const std::string& id, const std::string& name,
              const std::string& occupation, const std::string& personality,
              const std::string& greeting);
    virtual ~Character() = default;

    // Pure Virtual Functions (Abstract Interface)
    virtual std::string getRole() const = 0;
    virtual void displayProfile() const = 0;
    virtual std::string interact(const std::string& topic) = 0;

    // Common Getters
    std::string getId() const;
    std::string getName() const;
    std::string getOccupation() const;
    std::string getPersonality() const;
    std::string getGreeting() const;
};

#endif // CHARACTER_H
