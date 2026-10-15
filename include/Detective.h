#ifndef DETECTIVE_H
#define DETECTIVE_H

#include "Character.h"

class Detective : public Character {
private:
    std::string m_badgeNumber;
    std::string m_rank;
    int m_score;
    int m_energy;
    int m_maxEnergy;
    int m_hintsUsed;

public:
    Detective(const std::string& id, const std::string& name,
              const std::string& badgeNumber, const std::string& rank);

    // Overridden Virtual Methods (Polymorphic Implementation)
    std::string getRole() const override;
    void displayProfile() const override;
    std::string interact(const std::string& topic) override;

    // Detective Specific Methods
    int getScore() const;
    void addScore(int delta);
    void deductScore(int delta);

    int getEnergy() const;
    int getMaxEnergy() const;
    bool spendEnergy(int cost);
    void restoreEnergy(int amount);

    int getHintsUsed() const;
    void incrementHints();

    void setScore(int score);
    void setEnergy(int energy);
    void setHintsUsed(int hints);
};

#endif // DETECTIVE_H
