#include <sstream>
#include <fstream>

#include "XYZTool.h"
#include "Sandbox.h"

XYZToolFactory* XYZTool::factory=0;
XYZToolFactory* XYZTool::initClass(Vrui::ToolManager& toolManager)
{
    /* Create the tool factory: */
    factory=new XYZToolFactory("XYZTool","Export XYZ data",0,toolManager);
	
    /* Set up the tool class' input layout: */
    factory->setNumButtons(1);
    factory->setButtonFunction(0,"Export XYZ");
	
    /* Register and return the class: */
    toolManager.addClass(factory,Vrui::ToolManager::defaultToolFactoryDestructor);
    return factory;
}
XYZTool::XYZTool(const Vrui::ToolFactory* factory,const Vrui::ToolInputAssignment& inputAssignment)
    :Vrui::Tool(factory,inputAssignment),buttonDown(false)
{
}

XYZTool::~XYZTool(void)
{
}

const Vrui::ToolFactory* XYZTool::getFactory(void) const
{
    return factory;
}

void XYZTool::buttonCallback(int buttonSlotIndex,Vrui::InputDevice::ButtonCallbackData* cbData)
{
    // toggle the button state so we only save one file per up-down key press
    buttonDown = !buttonDown;
}

void XYZTool::frame(void)
{
    if (buttonDown) {
        // get the latest kinect frame
        const Kinect::FrameBuffer& frameBuffer = application->filteredFrames.getLockedValue();

        // get the time for the filename timestamp
        time_t s;
        time(&s);

        // construct the filenames
        std::stringstream ss;
        std::stringstream ssH;
        ss << XYZ_OUTPUT_DIR << "/xyz" << s << ".xyz";
        ssH << XYZ_OUTPUT_DIR << "/xyzh" << s << ".xyzh";

        // open the files
        std::ofstream outFile;
        std::ofstream outFileH;
        outFile.open(ss.str().c_str(), std::ofstream::out | std::ofstream::trunc);
        outFileH.open(ssH.str().c_str(), std::ofstream::out | std::ofstream::trunc);

        // get the frame data and per-pixel depth correction values
        const FilteredDepth *frameData = frameBuffer.getData<FilteredDepth>();
        Kinect::FrameSource::DepthCorrection *dc = application->camera->getDepthCorrectionParameters();
        Kinect::FrameSource::DepthCorrection::PixelCorrection *pixelDepthCorrection = dc->getPixelCorrection(application->frameSize);

        // loop over each pixel in the frame
        for (uint y=0; y<application->frameSize[1]; ++y) {
            for (uint x=0; x<application->frameSize[0]; ++x) {

                // get the filtered depth value, and add half-pixel xy offsets, depth correct z
                FilteredDepth d = frameData[y*application->frameSize[0]+x];
                double px=double(x)+0.5;
                double py=double(y)+0.5;
                double pz = pixelDepthCorrection[y*application->frameSize[0]+x].correct(d);

                // calculate the homogeneous coord weight
                const Kinect::FrameSource::IntrinsicParameters::PTransform::Matrix& m=application->cameraIps.depthProjection.getMatrix();
                double weight=Math::sqr(Math::sqr((m(2,0)*px+m(2,1)*py+m(2,2)*pz+m(2,3))/(m(3,0)*px+m(3,1)*py+m(3,2)*pz+m(3,3))));

                // generate the homogeneneous coord
                Kinect::FrameSource::IntrinsicParameters::PTransform::HVector c;
                c[0]=px*weight;
                c[1]=py*weight;
                c[2]=pz*weight;
                c[3]=weight;

                // transform into object coords, save a non-homogeneous version as well
                Geometry::ProjectiveTransformation<double, 3>::HVector oPoint = application->cameraIps.depthProjection.transform(c).toPoint();
                Kinect::FrameSource::IntrinsicParameters::PTransform::HVector oPointH = application->cameraIps.depthProjection.transform(c);

                // output the point cloud with inverted (+ve) z, and the homogeneous point cloud
                outFile << oPoint[0] << " " << oPoint[1] << " " << -oPoint[2] << "\n";
                outFileH << oPointH[0] << " " << oPointH[1] << " " << oPointH[2] << " " << oPointH[3] << "\n";
            }
        }

        // close the files
        outFile.close();
        outFileH.close();
    }
}
