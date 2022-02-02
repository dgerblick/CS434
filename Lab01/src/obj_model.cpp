#include <obj_model.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <regex>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace dng {

ObjModel::ObjModel(const std::string& filename) : filename(filename) {
    generate();
    initArrays();
    initIndexArray();
}

void ObjModel::render() {
    glBindVertexArray(vaID);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glEnableVertexAttribArray(0);
    // material properties
    glUniform3fv(kaParameter, 1, glm::value_ptr(ka));
    glUniform3fv(kdParameter, 1, glm::value_ptr(kd));
    glUniform3fv(ksParameter, 1, glm::value_ptr(ks));
    glUniform1fv(shParameter, 1, &sh);
    // model matrix
    glUniformMatrix4fv(modelParameter, 1, GL_FALSE, glm::value_ptr(model));
    // model for normals
    glUniformMatrix3fv(modelViewNParameter, 1, GL_FALSE, glm::value_ptr(modelViewN));
    //glDrawArrays(GL_TRIANGLES, 0, 3 * points);
    glDrawElements(GL_TRIANGLES, 3 * points, GL_UNSIGNED_INT, nullptr);
}

void ObjModel::initIndexArray() {
    indices = index.size();
    glBindVertexArray(vaID);
    glGenBuffers(1, &iboHandle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboHandle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices * sizeof(GLuint), index.data(), GL_STATIC_DRAW);
    glBindVertexArray(0);
    index.clear();
    vertNormIdx.clear();
}

void ObjModel::generate() {
    std::ifstream ifs(filename);
    if (!ifs) {
        std::cout << "Failed to load " << filename << '\n';
        for (int i = 0; i < 3; i++) {
            vertex.push_back(0.0);
            normal.push_back(0.0);
            index.push_back(0);
        }
        return;
    }
    std::string line;
    std::map<std::string, int> stats;
    indexCount = 0;
    vertNormIdx.clear();
    std::vector<glm::vec3> verts;
    std::vector<glm::vec3> norms;

    while (std::getline(ifs, line)) {
        std::istringstream iss(line);
        std::string cmd;
        iss >> cmd;
        if (cmd[0] == '#') {
            stats["Ignored (Comment)"]++;
        } else if (cmd == "v") {
            stats["Imported Vertices"]++;
            glm::vec3 v;
            iss >> v.x >> v.y >> v.z;
            verts.push_back(v);
        } else if (cmd == "vn") {
            stats["Imported Normals"]++;
            glm::vec3 v;
            iss >> v.x >> v.y >> v.z;
            norms.push_back(v);
        } else if (cmd == "f") {
            stats["Imported Faces"]++;
            std::vector<std::pair<GLint, GLint>> faceVerts;
            while (iss) {
                std::string faceVertStr;
                iss >> faceVertStr;
                std::vector<GLint> fis = getFaceIndicies(faceVertStr);
                if (!fis.empty())
                    faceVerts.emplace_back(fis[0], fis.size() > 2 ? fis[2] : -1);
            }
            processFace(faceVerts, verts, norms);
        } else {
            stats["Ignored (cmd: " + cmd + ")"]++;
        }
        stats["Total Lines Read"]++;
    }

    stats["Processed Verticies"] = vertex.size() / 3;
    stats["Processed Triangles"] = index.size() / 3;
    std::cout << "Loaded .obj file " << filename << ":\n";
    for (const auto& [stat, count] : stats)
        std::cout << "    " << stat << ": " << count << '\n';
}

void ObjModel::processFace(std::vector<std::pair<GLint, GLint>>& faceVerts,
                           std::vector<glm::vec3>& verts, std::vector<glm::vec3>& norms) {
    if (faceVerts.size() != 3) {
        if (faceVerts.size() > 3) {
            for (int i = 1; i < faceVerts.size() - 1; i++) {
                std::vector<std::pair<GLint, GLint>> triangle = { faceVerts[0], faceVerts[i],
                                                                  faceVerts[i + 1] };
                processFace(triangle, verts, norms);
            }
        }
        return;
    }
    glm::vec3& v0 = verts[faceVerts[0].first - 1];
    glm::vec3& v1 = verts[faceVerts[1].first - 1];
    glm::vec3& v2 = verts[faceVerts[2].first - 1];
    glm::vec3& n0 = norms[faceVerts[0].second - 1];
    glm::vec3& n1 = norms[faceVerts[1].second - 1];
    glm::vec3& n2 = norms[faceVerts[2].second - 1];
    if (n0 == glm::vec3(0.0) || n1 == glm::vec3(0.0) || n2 == glm::vec3(0.0)) {
        glm::vec3 norm = glm::cross(v0 - v1, v0 - v2);
        norms.push_back(norm);
        if (n0 == glm::vec3(0.0)) {
            n0 = norm;
            faceVerts[0].second = norms.size() - 1;
        }
        if (n1 == glm::vec3(0.0)) {
            n1 = norm;
            faceVerts[1].second = norms.size() - 1;
        }
        if (n2 == glm::vec3(0.0)) {
            n2 = norm;
            faceVerts[2].second = norms.size() - 1;
        }
    }
    for (int i = 0; i < 3; i++) {
        if (vertNormIdx.count(faceVerts[i]) == 0) {
            vertNormIdx[faceVerts[i]] = indexCount;
            indexCount++;
            for (int j = 0; j < 3; j++) {
                vertex.push_back(verts[faceVerts[i].first - 1][j]);
                normal.push_back(norms[faceVerts[i].second - 1][j]);
            }
        }
        index.push_back(vertNormIdx[faceVerts[i]]);
    }
}

std::vector<GLint> ObjModel::getFaceIndicies(const std::string& faceVertStr) {
    std::vector<GLint> faceIndicies;
    const char delim = '/';
    std::string current;
    std::istringstream iss(faceVertStr);
    while (std::getline(iss, current, delim)) {
        GLint ui = 0;
        if (!current.empty()) {
            std::istringstream iss2(current);
            iss2 >> ui;
        }
        faceIndicies.push_back(ui);
    }
    return faceIndicies;
}

}  // namespace dng
