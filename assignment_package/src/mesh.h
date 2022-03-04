#pragma once
#include "glm/glm.hpp"
#include "halfedge.h"
#include "face.h"
#include "vertex.h"
#include "drawable.h"
#include <QListWidgetItem>
#include <unordered_map>
#include <map>

#include "smartpointerhelp.h"
class Face;
class Vertex;
class HalfEdge;

class Mesh : public Drawable
{
public:
    //fields:
    std::vector<uPtr<Face>> faces;
    std::vector<uPtr<Vertex>> vertices;
    std::vector<uPtr<HalfEdge>> halfEdges;

    std::map<std::pair<int, int>, HalfEdge*> symMap;

    //Constructors:
    Mesh(OpenGLContext* context);

    //Methods:
    //Create override
    void create() override;

    //createCube

    void createCube();

    //Topology editing
    //splitEdge
    void splitEdge(HalfEdge* he1);

    //triangulate
    void triangulate(Face* face1);

    //Catmull Clark Subdivision
    void subdivide();

    void quadrangulate(Vertex* c, std::vector<HalfEdge*> v);

    //import OBJ
    void importVertexFromOBJ(glm::vec3 vertPos);
    void constructFacesFromOBJ(std::vector<int>);
    void clearMesh();
};
