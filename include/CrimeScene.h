#ifndef CRIME_SCENE_H
#define CRIME_SCENE_H

#include <string>
#include <vector>

struct Hotspot {
    std::string name;
    std::string inspectDescription;
    std::string evidenceId; // "" if no evidence attached
    bool examined;
};

class CrimeScene {
private:
    std::string m_id;
    std::string m_name;
    std::string m_description;
    std::vector<Hotspot> m_hotspots;

public:
    CrimeScene(const std::string& id, const std::string& name, const std::string& description);

    // Getters
    std::string getId() const;
    std::string getName() const;
    std::string getDescription() const;
    const std::vector<Hotspot>& getHotspots() const;
    size_t getHotspotCount() const;

    // Mutators
    void addHotspot(const std::string& name, const std::string& inspectDescription,
                    const std::string& evidenceId);
    std::string examineHotspot(size_t index);
    bool isHotspotExamined(size_t index) const;
    void setHotspotExamined(size_t index, bool examined);

    // Display
    void displaySceneOverview() const;
    bool isFullyExamined() const;
};

#endif // CRIME_SCENE_H
