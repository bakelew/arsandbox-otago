#ifndef WATERDISABLETOOL_INCLUDED
#define WATERDISABLETOOL_INCLUDED

#include <Vrui/Tool.h>
#include <Vrui/GenericToolFactory.h>
#include <Vrui/Application.h>
#include "WaterTable2.h"

/* Forward declarations: */
class Sandbox;
class WaterDisableTool;
typedef Vrui::GenericToolFactory<WaterDisableTool> WaterDisableToolFactory;

class WaterDisableTool:public Vrui::Tool,public Vrui::Application::Tool<Sandbox>
{
    friend class Vrui::GenericToolFactory<WaterDisableTool>;
 private:
    static WaterDisableToolFactory* factory;
    bool buttonZeroActive;
    bool waterOn;
    WaterTable2* wtBkp;
    
    /* Constructors and destructors: */
 public:
    static WaterDisableToolFactory* initClass(Vrui::ToolManager& ToolManager);
    WaterDisableTool(const Vrui::ToolFactory* factory,const Vrui::ToolInputAssignment& inputAssignment);
    virtual ~WaterDisableTool(void);
	
    /* Methods from class Vrui::Tool: */
    virtual const Vrui::ToolFactory* getFactory(void) const;
    virtual void buttonCallback(int buttonSlotIndex,Vrui::InputDevice::ButtonCallbackData* cbData);

    /* New methods: */
    void toggleWater();
    
};
#endif
