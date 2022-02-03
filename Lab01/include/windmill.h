#ifndef __WINDMILL_H__
#define __WINDMILL_H__

#include <shapes.h>
#include <obj_model.h>
#include <shape_instance.h>
#include <vector>
#include <optional>

namespace dng {

// derived class from Shapes
class Windmill : public Shapes {
public:
    Windmill(float angle = 0.0f, float speed = 90.0f, int numBlades = 4);
    virtual void render();
    virtual void update(float deltaT);
    static void init();

private:
    float angle;
    float speed;
    glm::mat4 baseOffset;
    glm::mat4 axleOffset;
    int numBlades;
    std::vector<glm::mat4> bladesOffset;
    std::vector<bool> bladesVisible;
    static std::optional<ObjModel> base;
    static std::optional<ObjModel> axle;
    static std::optional<ObjModel> blade;
};

}  // namespace dng

#endif  // __WINDMILL_H__
