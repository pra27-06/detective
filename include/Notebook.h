#ifndef NOTEBOOK_H
#define NOTEBOOK_H

#include <string>
#include <vector>

class Notebook {
private:
    std::vector<std::string> m_caseNotes;
    std::vector<std::string> m_recordedLeads;

public:
    Notebook();

    void addNote(const std::string& note);
    void addLead(const std::string& lead);

    const std::vector<std::string>& getNotes() const;
    const std::vector<std::string>& getLeads() const;

    void displayNotebook() const;
    void clear();

    std::string serializeNotes() const;
    std::string serializeLeads() const;
    void loadNotes(const std::vector<std::string>& notes);
    void loadLeads(const std::vector<std::string>& leads);
};

#endif // NOTEBOOK_H
