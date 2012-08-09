#ifndef IVW_ENTRYEXITPOINTS_H
#define IVW_ENTRYEXITPOINTS_H

#include "inviwo/core/inviwo.h"
#include "inviwo/core/ports/imageport.h"
#include "inviwo/core/ports/volumeport.h"
#include "../../inviwo/modules/opengl/inviwoopengl.h"
#include "../../inviwo/modules/opengl/processorgl.h"
#include "../../inviwo/modules/opengl/glwrap/shader.h"
#include "inviwo/core/interaction/trackball.h"


namespace inviwo {

    class EntryExitPoints : public ProcessorGL {
    public:
        EntryExitPoints();
        ~EntryExitPoints();

        void initialize();
        void deinitialize();

        virtual std::string getClassName() const { return "EntryExitPoints"; }
        virtual std::string getCategory() const  { return "Entry Exit Points"; }
        virtual CodeState getCodeState() const   { return CODE_STATE_STABLE; }

    protected:
        virtual void process();

    private:
        VolumePort volumePort_;
        ImagePort entryPort_;
        ImagePort exitPort_;

        CameraProperty camera_;
        FloatProperty viewDist_;

        Shader* shader_;

        GLuint listID_;

        void renderBoundingBox(vec3 llf, vec3 urb);
    };

} // namespace

#endif // IVW_ENTRYEXITPOINTS_H
