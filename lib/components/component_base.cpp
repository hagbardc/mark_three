#include <component_base.h>

namespace component {

ComponentBase::ComponentBase()
 : m_recentStateChange(false)
{
    strncpy(m_componentName, "\0", 1);
}

void ComponentBase::setComponentName( const char *name)
{
      strcpy(this->m_componentName, name);
}


void ComponentBase::populateStateChange(
    JsonObject &json, const char *element,
    const char *action, const char *value)
{
    if( !action || !value || !element) {
        return;
    }
    json["component"] = this->m_componentName;
    json["element"] = element;
    json["action"] = action;
    json["value"] = value;
}

}
