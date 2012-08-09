#ifndef IVW_SHADEROBJECT_H
#define IVW_SHADEROBJECT_H

#include <io.h>
#include "inviwo/core/inviwo.h"
#include "../inviwoopengl.h"

namespace inviwo {

    class ShaderObject {

    public:
        ShaderObject(GLenum shaderType, std::string fileName);
        ~ShaderObject();

        GLuint getID() { return id_; }

        void initialize();
        void deinitialize();

        void loadSource(std::string fileName);
        void upload();
        void compile();
        std::string getCompileLog();

    private:
        GLenum shaderType_;
        std::string fileName_;

        GLuint id_;
        const char* source_;
    };

} // namespace

#endif // IVW_SHADEROBJECT_H
