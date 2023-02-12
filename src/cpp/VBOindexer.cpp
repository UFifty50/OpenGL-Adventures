#include <vector>
#include <map>
#include <string.h>

#include "glm/glm.hpp"


struct PackedVertex {
    glm::vec3 position;
    glm::vec2 uv;
    glm::vec3 normal;

    bool operator<(const PackedVertex& other) const {
        return ::memcmp((void*)this, (void*)&other, sizeof(PackedVertex)) > 0;
    }
};

bool isSimilar(float vertex1, float vertex2) {
    return ::fabs(vertex1 - vertex2) < 0.005f;
}

bool getSimilarVertexIndex(
    const glm::vec3& inVertex,
    const glm::vec2& inUV,
    const glm::vec3& inNormal,
    std::vector<glm::vec3>& outVertices,
    std::vector<glm::vec2>& outUVs,
    std::vector<glm::vec3>& outNormals,
    unsigned short& result
) {
    // simple linear search
    for (unsigned int i = 0; i < outVertices.size(); i++) {
        if (
            ::isSimilar(inVertex.x, outVertices[i].x) &&
            ::isSimilar(inVertex.y, outVertices[i].y) &&
            ::isSimilar(inVertex.z, outVertices[i].z) &&
            ::isSimilar(inUV.x,     outUVs[i].x     ) &&
            ::isSimilar(inUV.y,     outUVs[i].y     ) &&
            ::isSimilar(inNormal.x, outNormals[i].x ) &&
            ::isSimilar(inNormal.y, outNormals[i].y ) &&
            ::isSimilar(inNormal.z, outNormals[i].z )
        ) {
            result = i;
            return true;
        }
    }

    // no other vertex could be used instead
    return false;
}

bool fast_getSimilarVertexIndex(
    PackedVertex& packed,
    std::map<PackedVertex, unsigned short>& VertexToOutIndex,
    unsigned short& result
) {
    std::map<PackedVertex, unsigned short>::iterator iter = VertexToOutIndex.find(packed);
    if (iter == VertexToOutIndex.end()) {
        return false;
    } else {
        result = iter->second;
        return true;
    }
}

void indexVBO(
    const std::vector<glm::vec3>& inVertices,
    const std::vector<glm::vec2>& inUVs,
    const std::vector<glm::vec3>& inNormals,

    std::vector<unsigned short>& outIndices,
    std::vector<glm::vec3>& outVertices,
    std::vector<glm::vec2>& outUVs,
    std::vector<glm::vec3>& outNormals
) {
    // For each input vertex
    for (unsigned int i = 0; i < inVertices.size(); i++) {

        // Try to find a similar vertex
        unsigned short index;
        bool found = ::getSimilarVertexIndex(inVertices[i], inUVs[i], inNormals[i], outVertices, outUVs, outNormals, index);

        if (found) { // A similar vertex is already in the VBO
            outIndices.push_back(index);
        } else { // If not, it needs to be added in the output data.
            outVertices.push_back(inVertices[i]);
            outUVs.push_back(inUVs[i]);
            outNormals.push_back(inNormals[i]);
            outIndices.push_back((unsigned short)outVertices.size() - 1);
        }
    }
}

void fast_indexVBO (
    const std::vector<glm::vec3>& inVertices,
    const std::vector<glm::vec2>& inUVs,
    const std::vector<glm::vec3>& inNormals,

    std::vector<unsigned short>& outIndices,
    std::vector<glm::vec3>& outVertices,
    std::vector<glm::vec2>& outUVs,
    std::vector<glm::vec3>& outNormals
) {
    std::map<PackedVertex, unsigned short> VertexToOutIndex;

    for (unsigned int i = 0; i < inVertices.size(); i++) {

        PackedVertex packed = { inVertices[i], inUVs[i], inNormals[i] };

        // Try to find a similar vertex
        unsigned short index;
        bool found = ::fast_getSimilarVertexIndex(packed, VertexToOutIndex, index);

        if (found) { // A similar vertex is already in the VBO
            outIndices.push_back(index);
        } else { // If not, it needs to be added to the output data
            outVertices.push_back(inVertices[i]);
            outUVs.push_back(inUVs[i]);
            outNormals.push_back(inNormals[i]);
            unsigned short newindex = (unsigned short)outVertices.size() - 1;
            outIndices.push_back(newindex);
            VertexToOutIndex[packed] = newindex;
        }
    }
}

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
) {
    for (unsigned int i = 0; i < inVertices.size(); i++) {

        // Try to find a similar vertex
        unsigned short index;
        bool found = ::getSimilarVertexIndex(inVertices[i], inUVs[i], inNormals[i], outVertices, outUVs, outNormals, index);

        if (found) { // A similar vertex is already in the VBO
            outIndices.push_back(index);

            // Average the tangents and the bitangents
            outTangents[index] += inTangents[i];
            outBitangents[index] += inBitangents[i];
        } else { // If not, it needs to be added to the output data
            outVertices.push_back(inVertices[i]);
            outUVs.push_back(inUVs[i]);
            outNormals.push_back(inNormals[i]);
            outTangents.push_back(inTangents[i]);
            outBitangents.push_back(inBitangents[i]);
            outIndices.push_back((unsigned short)outVertices.size() - 1);
        }
    }
}
