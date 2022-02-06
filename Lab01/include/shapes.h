#ifndef __SHAPES_H__
#define __SHAPES_H__

#include <material.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <list>
#include <memory>

namespace dng {

class Shapes : public Material {
public:
    virtual void setModelMatrixParamToShader(GLuint uniform);
    virtual void setModelViewNMatrixParamToShader(GLuint uniform);
    virtual void setModel(glm::mat4 tmp);
    virtual void setModelViewN(glm::mat3 tmp);  // 3x3 matrix!
    virtual void setColor(GLubyte r, GLubyte b, GLubyte g);
    virtual void render();
    virtual void update(float deltaT);
    void initArrays();
    void deleteShape();

    static void addVertex(std::vector<GLfloat>& a, const glm::vec3& v);
    template <typename T, typename... Args,
              typename = std::enable_if_t<std::is_base_of_v<Shapes, T>>>
    static T& listAdd(Args&&... args) {
        std::unique_ptr<Shapes>& ptr = list.emplace_front(std::make_unique<T>(args...));
        ptr->it = list.begin();
        T* t = dynamic_cast<T*>(ptr.get());
        return *t;
    }
    static void listClear();
    static void step(float deltaT);
protected:
    using List = std::list<std::unique_ptr<Shapes>>;
    GLuint modelParameter;  // shader uniform variables
    GLuint modelViewNParameter;
    glm::mat4 model;       // modeling matrix
    glm::mat3 modelViewN;  // model view normals matrix
    GLubyte color[3];
    std::vector<GLfloat>* a;
    std::vector<GLfloat> vertex;
    std::vector<GLfloat> normal;
    GLuint vboHandles[2];  // vertices and normals
    GLuint vaID;
    GLuint buffer;
    GLuint points;
    GLuint normals;
    List::iterator it;

    static List list;
    static std::list<List::iterator> deleteList;
};

}  // namespace dng

#endif  // __SHAPES_H__
