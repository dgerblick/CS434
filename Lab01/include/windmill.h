#ifndef __WINDMILL_H__
#define __WINDMILL_H__

#include <shapes.h>
#include <obj_model.h>
#include <vector>

namespace dng {

// derived class from Shapes
class Windmill : public Shapes {
public:
    Windmill(int numBlades = 4);
    virtual void render();
    virtual void setKa(glm::vec3 amb);
    virtual void setKd(glm::vec3 diff);
    virtual void setKs(glm::vec3 spec);
    virtual void setSh(float sh);
    virtual void setKaToShader(GLuint uniform);
    virtual void setKdToShader(GLuint uniform);
    virtual void setKsToShader(GLuint uniform);
    virtual void setShToShader(GLuint uniform);
    virtual void setModelMatrixParamToShader(GLuint uniform);
    virtual void setModelViewNMatrixParamToShader(GLuint uniform);
    virtual void setModel(glm::mat4 tmp);
    virtual void setModelViewN(glm::mat3 tmp);
    virtual void setColor(GLubyte r, GLubyte b, GLubyte g);

private:
    ObjModel base;
    ObjModel axle;
    ObjModel blade;
    double angle;
    glm::mat4 baseOffset;
    glm::mat4 axleOffset;
    std::vector<glm::mat4> bladesOffset;
};

}  // namespace dng

#endif  // __WINDMILL_H__
