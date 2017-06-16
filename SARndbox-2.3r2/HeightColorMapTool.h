#ifndef HCMTOOL_INCLUDED
#define HCMTOOL_INCLUDED

#include <Vrui/Tool.h>
#include <Vrui/GenericToolFactory.h>
#include <Vrui/Application.h>

/* Forward declarations: */
class Sandbox;
class ElevationColorMap;
class HeightColorMapTool;
typedef Vrui::GenericToolFactory<HeightColorMapTool> HeightColorMapToolFactory;

class HeightColorMapTool:public Vrui::Tool,public Vrui::Application::Tool<Sandbox>
{
    friend class Vrui::GenericToolFactory<HeightColorMapTool>;
 private:
    static HeightColorMapToolFactory* factory;
    unsigned int mapIndex;
    bool buttonOneActive;
    
    /* Constructors and destructors: */
 public:
    static HeightColorMapToolFactory* initClass(Vrui::ToolManager& ToolManager);
    HeightColorMapTool(const Vrui::ToolFactory* factory,const Vrui::ToolInputAssignment& inputAssignment);
    virtual ~HeightColorMapTool(void);
	
    /* Methods from class Vrui::Tool: */
    virtual const Vrui::ToolFactory* getFactory(void) const;
    virtual void buttonCallback(int buttonSlotIndex,Vrui::InputDevice::ButtonCallbackData* cbData);

    /* New methods: */
    void scrollHeightColorMaps(int buttonSlotIndex);
    
};
#endif
