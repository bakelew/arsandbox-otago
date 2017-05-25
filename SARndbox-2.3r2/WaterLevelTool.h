#ifndef WATERLEVELTOOL_INCLUDED
#define WATERLEVELTOOL_INCLUDED

#define WATER_ADJUST_SPEED 0.01

#include <Vrui/Tool.h>
#include <Vrui/GenericToolFactory.h>
#include <Vrui/Application.h>

/* Forward declarations: */
class Sandbox;
class WaterLevelTool;
typedef Vrui::GenericToolFactory<WaterLevelTool> WaterLevelToolFactory;

class WaterLevelTool:public Vrui::Tool,public Vrui::Application::Tool<Sandbox>
{
    friend class Vrui::GenericToolFactory<WaterLevelTool>;
 private:
    static WaterLevelToolFactory* factory;
    bool buttonZeroActive;
    bool buttonOneActive;
    double waterLevelOffset;
    
    /* Constructors and destructors: */
 public:
    static WaterLevelToolFactory* initClass(Vrui::ToolManager& ToolManager);
    WaterLevelTool(const Vrui::ToolFactory* factory,const Vrui::ToolInputAssignment& inputAssignment);
    virtual ~WaterLevelTool(void);
	
    /* Methods from class Vrui::Tool: */
    virtual const Vrui::ToolFactory* getFactory(void) const;
    virtual void buttonCallback(int buttonSlotIndex,Vrui::InputDevice::ButtonCallbackData* cbData);
    virtual void frame(void);

    /* New methods: */
    void adjustWaterLevel(int buttonSlotIndex);
    
};
#endif
