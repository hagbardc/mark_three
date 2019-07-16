#ifndef INCLUDED_COMPONENT_MANAGER_H
#define INCLUDED_COMPONENT_MANAGER_H

#include <component_base.h>

namespace component {


typedef struct component_data {
    ComponentBase *component;
    int pinStart;   //  Start of pin assignment range
    int pinEnd;     //  End of pin assignment range
}  component_info;


// This class is intended to hold a (sort of dynamically allocated)) array
// of components, and the relevant information there fore
class ComponentManager {
public:
    // default size is the initial size of the allocated array

    ComponentManager(int defaultSize = 50);
    virtual ~ComponentManager();

    //  Adds a component to the manager.  Returns the index of the
    //  component (for later direct addressing, if desired)
    int addComponent(ComponentBase *component, int pinStart, int pinEnd=0);

    component_data *getComponentDataAtIndex(int index);
    ComponentBase *getComponentAtIndex(int index);

    int getNumberOfComponents() { return m_currentComponents;}


private:
    component_data **m_componentArray;
    int m_currentComponents;
    int m_componentArraySize;
};


}  // end namespace

#endif
