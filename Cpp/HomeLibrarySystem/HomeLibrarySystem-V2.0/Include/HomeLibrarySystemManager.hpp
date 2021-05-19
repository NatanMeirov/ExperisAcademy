#ifndef __HOMELIBRARYSYSTEM_HPP__
#define __HOMELIBRARYSYSTEM_HPP__


#include "TLibrary_Inline.hpp"
#include "CollectableItem.hpp"
#include "HomeLibrarySystemUIHandler.hpp"
#include "FindItemCriteria.hpp"


class HomeLibrarySystemManager {
public:
    // A callback action function to be triggered on the given CollactableItem instance
    typedef void (*ActionCallbackFunction)(const CollectableItem& a_item, void* a_contextOfAction); // Action examples in this app: Print the items, Change the data of the items (collected [received] year)

    HomeLibrarySystemManager();
    HomeLibrarySystemManager(const HomeLibrarySystemManager& a_other);
    HomeLibrarySystemManager& operator=(const HomeLibrarySystemManager& a_other);
    ~HomeLibrarySystemManager();

    // API Methods
    void RunHomeLibrarySystem();

private:

    bool HandleHomeLibrarySystemUserChoice(HomeLibrarySystemUIHandler::MenuOptions a_option);
    void AddNewItemToLibrary(const std::string& a_itemType); // Available item types: "Book" or "Movie"
    void TriggerActionOnItemByCriteria(const FindItemCriteria& a_findCriteria, ActionCallbackFunction a_action, void* a_contextOfAction);

    // Members
    Library<CollectableItem*> m_library;
};


#endif // __HOMELIBRARYSYSTEM_HPP__