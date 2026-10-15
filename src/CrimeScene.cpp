#include "CrimeScene.h"
#include "TerminalUI.h"
#include <iostream>

CrimeScene::CrimeScene(const std::string& id, const std::string& name, const std::string& description)
    : m_id(id), m_name(name), m_description(description) {}

std::string CrimeScene::getId() const { return m_id; }
std::string CrimeScene::getName() const { return m_name; }
std::string CrimeScene::getDescription() const { return m_description; }
const std::vector<Hotspot>& CrimeScene::getHotspots() const { return m_hotspots; }
size_t CrimeScene::getHotspotCount() const { return m_hotspots.size(); }

void CrimeScene::addHotspot(const std::string& name, const std::string& inspectDescription,
                            const std::string& evidenceId) {
    Hotspot hs;
    hs.name = name;
    hs.inspectDescription = inspectDescription;
    hs.evidenceId = evidenceId;
    hs.examined = false;
    m_hotspots.push_back(hs);
}

std::string CrimeScene::examineHotspot(size_t index) {
    if (index >= m_hotspots.size()) return "";
    m_hotspots[index].examined = true;
    return m_hotspots[index].evidenceId;
}

bool CrimeScene::isHotspotExamined(size_t index) const {
    if (index >= m_hotspots.size()) return false;
    return m_hotspots[index].examined;
}

void CrimeScene::setHotspotExamined(size_t index, bool examined) {
    if (index < m_hotspots.size()) {
        m_hotspots[index].examined = examined;
    }
}

void CrimeScene::displaySceneOverview() const {
    std::vector<std::string> lines;
    lines.push_back("Area: " + m_name);
    lines.push_back("");
    lines.push_back(m_description);
    lines.push_back("");
    lines.push_back("Points of Interest:");
    for (size_t i = 0; i < m_hotspots.size(); ++i) {
        std::string status = m_hotspots[i].examined ? "[EXAMINED]" : "[UNEXAMINED]";
        lines.push_back("  " + std::to_string(i + 1) + ". " + m_hotspots[i].name + " " + status);
    }
    UI::printBox("SCENE INVESTIGATION - " + m_name, lines, 74);
}

bool CrimeScene::isFullyExamined() const {
    for (const auto& hs : m_hotspots) {
        if (!hs.examined) return false;
    }
    return true;
}
