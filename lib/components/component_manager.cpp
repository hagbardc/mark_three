#include <component_manager.h>




namespace component {


ComponentManager::ComponentManager(int defaultSize)
:   m_currentComponents(0),
    m_componentArraySize(defaultSize)
{
    m_componentArray = new component_data*[m_componentArraySize];
}


ComponentManager::~ComponentManager()
{
    for(int ii=0; ii<m_currentComponents; ++ii) {
        if(m_componentArray[ii]->component) {
            delete m_componentArray[ii]->component;
        }
    }
    delete[] m_componentArray;
}

int ComponentManager::addComponent(ComponentBase *component, int pinStart, int pinEnd)
{
    if(m_currentComponents >= m_componentArraySize) {
        return 0;
    }

    m_componentArray[m_currentComponents] = new component_data();
    m_componentArray[m_currentComponents]->component = component;
    m_componentArray[m_currentComponents]->pinStart = pinStart;
    m_componentArray[m_currentComponents]->pinEnd = pinStart;

    if(pinEnd) {
        m_componentArray[m_currentComponents]->pinEnd = pinEnd;
    }

    int toReturn = m_currentComponents;
    ++m_currentComponents;
    return toReturn;
}

component_data *ComponentManager::getComponentDataAtIndex(int index)
{
    if(index >= m_currentComponents) {
        return 0;
    }

    return m_componentArray[index];
}
ComponentBase *ComponentManager::getComponentAtIndex(int index)
{
    if(index >= m_currentComponents) {
        return 0;
    }

    return m_componentArray[index]->component;
}




}
