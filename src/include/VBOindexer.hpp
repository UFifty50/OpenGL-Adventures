#ifndef VBOINDEXER_HPP
#define VBOINDEXER_HPP

#include <vector>
#include "glm/glm.hpp"


void fast_indexVBO (
    const std::vector<glm::vec3>& inVertices,
    const std::vector<glm::vec2>& inUVs,
    const std::vector<glm::vec3>& inNormals,

    std::vector<unsigned short>& outIndices,
    std::vector<glm::vec3>& outVertices,
    std::vector<glm::vec2>& outUVs,
    std::vector<glm::vec3>& outNormals
);

void indexVBO_TBN(
    const std::vector<glm::vec3>& inVertices,
    const std::vector<glm::vec2>& inUVs,
    const std::vector<glm::vec3>& inNormals,
    const std::vector<glm::vec3>& inTangents,
    const std::vector<glm::vec3>& inBitangents,

    std::vector<unsigned short>& outIndices,
    std::vector<glm::vec3>& outVertices,
    std::vector<glm::vec2>& outUVs,
    std::vector<glm::vec3>& outNormals,
    std::vector<glm::vec3>& outTangents,
    std::vector<glm::vec3>& outBitangents
);

#endif
