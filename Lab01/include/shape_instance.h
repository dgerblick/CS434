#ifndef __SHAPE_INSTANCE_H__
#define __SHAPE_INSTANCE_H__

#include <shapes.h>
#include <type_traits>
#include <memory>

namespace dng {

template <typename T, typename = std::enable_if_t<std::is_base_of_v<Shapes, T>>>
class ShapeInstance : public Shapes {
public:
    template <typename... Args>
    ShapeInstance(Args&&... args) {
        shape = std::make_shared<T>(args...);
    }

    ShapeInstance(std::shared_ptr<T>& shape) : shape(shape) {}

    ShapeInstance(ShapeInstance<T>& instance) : shape(instance.shape) {}

    ShapeInstance<T> clone() { return ShapeInstance<T>(shape); }

    std::unique_ptr<ShapeInstance<T>> cloneToUnique() {
        return std::make_unique<ShapeInstance<T>>(shape);
    }

    virtual void render() {
        shape->setKa(ka);
        shape->setKs(ks);
        shape->setKd(kd);
        shape->setSh(sh);
        shape->setModel(model);
        shape->setModelMatrixParamToShader(modelParameter);
        shape->setModelViewNMatrixParamToShader(modelViewNParameter);
        shape->setKaToShader(kaParameter);
        shape->setKdToShader(kdParameter);
        shape->setKsToShader(ksParameter);
        shape->setShToShader(shParameter);
        shape->render();
    }

    virtual void update(float deltaT) {
        shape->update(deltaT);
    }

private:
    std::shared_ptr<T> shape;
};

}  // namespace dng

#endif  // __SHAPE_INSTANCE_H__
