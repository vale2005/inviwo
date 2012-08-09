#ifndef IVW_SHADER_H
#define IVW_SHADER_H

#include <io.h>
#include "inviwo/core/inviwo.h"
#include "../inviwoopengl.h"
#include "shaderobject.h"

namespace inviwo {

    class Shader {

    public:
        Shader(std::string fragmentFilename);
        Shader(std::string vertexFilename, std::string fragmentFilename);
        virtual ~Shader();

        void initialize();
        void deinitialize();

        void attachShaderObject(ShaderObject* shaderObject);
        void link();

        void activate();
        void deactivate();

        void setUniform(const std::string name, GLint value);

    private:
        unsigned int id_;

        std::string vertexFilename_;
        std::string fragmentFilename_;
        ShaderObject* vertexShaderObject_;
        ShaderObject* fragmentShaderObject_;
    };

} // namespace

#endif // IVW_SHADER_H
