#ifndef __OBJ_MODEL_H__
#define __OBJ_MODEL_H__

#include <shapes.h>
#include <string>
#include <vector>
#include <map>
#include <GL/glew.h>
#include <glm/glm.hpp>

namespace dng {

class ObjModel : public Shapes {
public:
    ObjModel(const std::string& filename);
    virtual void render();

private:
    std::string filename;
    std::vector<GLuint> index;
    GLuint iboHandle;
    GLuint indices;
    std::map<std::pair<GLint, GLint>, GLuint> vertNormIdx;
    int indexCount;

    void generate();
    void initIndexArray();
    void processFace(std::vector<std::pair<GLint, GLint>>& faceVerts, std::vector<glm::vec3>& verts,
                     std::vector<glm::vec3>& norms);
    static std::vector<GLint> getFaceIndicies(const std::string& faceVertStr);
};

}  // namespace dng

#endif  // __OBJ_MODEL_H__
