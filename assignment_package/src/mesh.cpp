#include "mesh.h"
#include "halfedge.h"
#include "face.h"
#include "vertex.h"
#include <iostream>
#include <unordered_set>

Mesh::Mesh(OpenGLContext* context)
    : Drawable(context), faces{}, vertices{}, halfEdges{}, symMap{}
{}

void Mesh::create() {
    //Use vertices, half-edge, vertex, and face data to make VBOs
        //like the vectors of vertex positions, colors, normals, and indices
    //Resources:
    //faces, a std::vector of unique pointers of all the faces in the mesh
    //.get() - returns a raw pointer to the memory address pointed to by the uPtr

    ////Part 1:
    std::vector<glm::vec4> pos;
    std::vector<glm::vec4> cols;
    std::vector<glm::vec4> nor;
    std::vector<GLuint> idx;
//    std::vector<int> IDs;
    std::vector<int> jointIDs;
    std::vector<float> weights;

    int numVertsSoFar = 0;

    //Resources: faces, vertices, halfEdges

    //For each face:

    for (int i = 0; i < faces.size(); i++) {

        std::vector<glm::vec3> thisFacePos;
        Face *curFace = faces[i].get();
        HalfEdge *curE = curFace->hEdge;

        do {
            //1. Add the vertex curE points to pos

            Vertex *curV = curE->vertex;

            //Add the vertices joints to jointIDs
            for (int i = 0; i < curV->joints.size(); i++) {
                jointIDs.push_back(curV->joints[i]);
            }
            //add the vertices weights to weights
            for (int i = 0; i < curV->jointWeights.size(); i++) {
                weights.push_back(curV->jointWeights[i]);
            }
            //
            pos.push_back(glm::vec4(curV->pos, 1.f));
            thisFacePos.push_back(curV->pos);

            //2. Move to the next halfEdge
            curE = curE->next;

            //3. Add color to cols for each vertex
            cols.push_back(glm::vec4(curFace->color, 1.f));



        } while(curE->ID != (curFace)->hEdge->ID);


//////////////////////////
        ////idx
//////////////////////////

        int n = thisFacePos.size() - 2;

        for (int j = 0; j < n; j++) {
            idx.push_back(numVertsSoFar);
            idx.push_back(j + 1 + numVertsSoFar);
            idx.push_back(j + 2 + numVertsSoFar);
        }

        numVertsSoFar += thisFacePos.size();

        ////////////////////////
        ////normals
        ////////////////////////
        //1. For each vertex in thisFacePos, find its edges and then find its normal

        for (int j = 0; j < thisFacePos.size(); j++) {
            glm::vec3 curV = thisFacePos[j];

            glm::vec3 e1 = thisFacePos[(j + 1) % thisFacePos.size()] - curV;
            glm::vec3 e2 = thisFacePos[glm::max((j - 1), 0)] - curV;

            glm::vec3 norm = glm::cross(e1, e2);


            if (norm.x == 0 && norm.y == 0 && norm.z == 0) {

                int k = (j + 1) % thisFacePos.size();

                while ((k != j) && (norm.x == 0 && norm.y == 0 && norm.z == 0)) {
                    e2 = thisFacePos[glm::max((k - 1), 0)] - curV;
                    norm = glm::cross(e1, e2);

                    k++;
                    k %= thisFacePos.size();
                }
            }
            nor.push_back(glm::vec4(norm, 0.f));
        }
    }


    ////GUI part:

    count = idx.size();


    generateIdx();
    mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, idx.size() * sizeof(GLuint), idx.data(), GL_STATIC_DRAW);

    generatePos();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    mp_context->glBufferData(GL_ARRAY_BUFFER, pos.size() * sizeof(glm::vec4), pos.data(), GL_STATIC_DRAW);

    generateNor();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufNor);
    mp_context->glBufferData(GL_ARRAY_BUFFER, nor.size() * sizeof(glm::vec4), nor.data(), GL_STATIC_DRAW);

    generateCol();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    mp_context->glBufferData(GL_ARRAY_BUFFER, cols.size() * sizeof(glm::vec4), cols.data(), GL_STATIC_DRAW);

//    std::vector<int> jointIDs;
//    std::vector<float> weights;

//    GLuint bufIDs;
//    GLuint bufWeights;

    generateIDs();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufIDs);
    mp_context->glBufferData(GL_ARRAY_BUFFER, jointIDs.size() * sizeof(glm::vec2), jointIDs.data(), GL_STATIC_DRAW);

    generateWeights();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufWeights);
    mp_context->glBufferData(GL_ARRAY_BUFFER, weights.size() * sizeof(glm::vec2), weights.data(), GL_STATIC_DRAW);
    //
}

void Mesh::createCube() {

    /*
    Step 1:
    Create each face
        Step 1A: Create the vertices of the face
        Step 1B: Create the half edges of the face
        Step 1C: Connect the half edges with vertices in a loop
        Step 1D: connect the face with some half edge
        Step 1E: Create a std::pair<Vert*, Vert*> for each HE, and use this as the key in std::unordered_map<pair<V*, V*>, HE*>
    Step 2:
    Connect Sym half edges
    */

    //VARIABLES on the STACK!

    //VARIABLES:
    Face frontUPtr;
    Face backUPtr;
    Face leftUPtr;
    Face rightUPtr;
    Face topUPtr;
    Face downUPtr;

    //Front:
    Vertex f0UPtr = Vertex(glm::vec3(-0.5, -0.5, 0.5));
    Vertex f1UPtr = Vertex(glm::vec3(0.5, -0.5, 0.5));
    Vertex f2UPtr = Vertex(glm::vec3(0.5, 0.5, 0.5));
    Vertex f3UPtr = Vertex(glm::vec3(-0.5, 0.5, 0.5));

    //Back:
    Vertex b0UPtr = Vertex(glm::vec3(0.5, -0.5, -0.5));
    Vertex b1UPtr = Vertex(glm::vec3(-0.5, -0.5, -0.5));
    Vertex b2UPtr = Vertex(glm::vec3(-0.5, 0.5, -0.5));
    Vertex b3UPtr = Vertex(glm::vec3(0.5, 0.5, -0.5));

    //Front:
    HalfEdge f0_1UPtr = HalfEdge();
    HalfEdge f1_2UPtr = HalfEdge();
    HalfEdge f2_3UPtr = HalfEdge();
    HalfEdge f3_0UPtr = HalfEdge();

    //Back:
    HalfEdge b0_1UPtr = HalfEdge();
    HalfEdge b1_2UPtr = HalfEdge();
    HalfEdge b2_3UPtr = HalfEdge();
    HalfEdge b3_0UPtr = HalfEdge();

    //Left:
    HalfEdge l0_1UPtr = HalfEdge();
    HalfEdge l1_2UPtr = HalfEdge();
    HalfEdge l2_3UPtr = HalfEdge();
    HalfEdge l3_0UPtr = HalfEdge();

    //Right:

    HalfEdge r0_1UPtr = HalfEdge();
    HalfEdge r1_2UPtr = HalfEdge();
    HalfEdge r2_3UPtr = HalfEdge();
    HalfEdge r3_0UPtr = HalfEdge();

    //Top:

    HalfEdge t0_1UPtr = HalfEdge();
    HalfEdge t1_2UPtr = HalfEdge();
    HalfEdge t2_3UPtr = HalfEdge();
    HalfEdge t3_0UPtr = HalfEdge();

    //Down:

    HalfEdge d0_1UPtr = HalfEdge();
    HalfEdge d1_2UPtr = HalfEdge();
    HalfEdge d2_3UPtr = HalfEdge();
    HalfEdge d3_0UPtr = HalfEdge();


    //POINTERS:

    //VARIABLES NOW IN THE HEAP, WITH THE VECTOR!

    this->faces.clear();
    this->faces.push_back(mkU<Face> (frontUPtr));
    this->faces.push_back(mkU<Face> (backUPtr));
    this->faces.push_back(mkU<Face> (leftUPtr));
    this->faces.push_back(mkU<Face> (rightUPtr));
    this->faces.push_back(mkU<Face> (topUPtr));
    this->faces.push_back(mkU<Face> (downUPtr));


    this->vertices.clear();
    this->vertices.push_back(mkU<Vertex> (f0UPtr));
    this->vertices.push_back(mkU<Vertex> (f1UPtr));
    this->vertices.push_back(mkU<Vertex> (f2UPtr));
    this->vertices.push_back(mkU<Vertex> (f3UPtr));

    this->vertices.push_back(mkU<Vertex> (b0UPtr));
    this->vertices.push_back(mkU<Vertex> (b1UPtr));
    this->vertices.push_back(mkU<Vertex> (b2UPtr));
    this->vertices.push_back(mkU<Vertex> (b3UPtr));

    this->halfEdges.clear();
    this->halfEdges.push_back(mkU<HalfEdge> (f0_1UPtr));
    this->halfEdges.push_back(mkU<HalfEdge> (f1_2UPtr));
    this->halfEdges.push_back(mkU<HalfEdge> (f2_3UPtr));
    this->halfEdges.push_back(mkU<HalfEdge> (f3_0UPtr));

    this->halfEdges.push_back(mkU<HalfEdge> (b0_1UPtr));
    this->halfEdges.push_back(mkU<HalfEdge> (b1_2UPtr));
    this->halfEdges.push_back(mkU<HalfEdge> (b2_3UPtr));
    this->halfEdges.push_back(mkU<HalfEdge> (b3_0UPtr));

    this->halfEdges.push_back(mkU<HalfEdge> (l0_1UPtr));
    this->halfEdges.push_back(mkU<HalfEdge> (l1_2UPtr));
    this->halfEdges.push_back(mkU<HalfEdge> (l2_3UPtr));
    this->halfEdges.push_back(mkU<HalfEdge> (l3_0UPtr));

    this->halfEdges.push_back(mkU<HalfEdge> (r0_1UPtr));
    this->halfEdges.push_back(mkU<HalfEdge> (r1_2UPtr));
    this->halfEdges.push_back(mkU<HalfEdge> (r2_3UPtr));
    this->halfEdges.push_back(mkU<HalfEdge> (r3_0UPtr));

    this->halfEdges.push_back(mkU<HalfEdge> (t0_1UPtr));
    this->halfEdges.push_back(mkU<HalfEdge> (t1_2UPtr));
    this->halfEdges.push_back(mkU<HalfEdge> (t2_3UPtr));
    this->halfEdges.push_back(mkU<HalfEdge> (t3_0UPtr));

    this->halfEdges.push_back(mkU<HalfEdge> (d0_1UPtr));
    this->halfEdges.push_back(mkU<HalfEdge> (d1_2UPtr));
    this->halfEdges.push_back(mkU<HalfEdge> (d2_3UPtr));
    this->halfEdges.push_back(mkU<HalfEdge> (d3_0UPtr));

    ///

    //Raw pointers:

    Face *front = faces[0].get();
    Face *back = faces[1].get();
    Face *left = faces[2].get();
    Face *right = faces[3].get();
    Face *top = faces[4].get();
    Face *down = faces[5].get();

    //Front:
    Vertex *f0 = vertices[0].get();
    Vertex *f1 = vertices[1].get();
    Vertex *f2 = vertices[2].get();
    Vertex *f3 = vertices[3].get();

    //Back:
    Vertex *b0 = vertices[4].get();
    Vertex *b1 = vertices[5].get();
    Vertex *b2 = vertices[6].get();
    Vertex *b3 = vertices[7].get();

    //Front:
    HalfEdge *f0_1 = halfEdges[0].get();
    HalfEdge *f1_2 = halfEdges[1].get();
    HalfEdge *f2_3 = halfEdges[2].get();
    HalfEdge *f3_0 = halfEdges[3].get();

    //Back:
    HalfEdge *b0_1 = halfEdges[4].get();
    HalfEdge *b1_2 = halfEdges[5].get();
    HalfEdge *b2_3 = halfEdges[6].get();
    HalfEdge *b3_0 = halfEdges[7].get();

    //Left:
    HalfEdge *l0_1 = halfEdges[8].get();
    HalfEdge *l1_2 = halfEdges[9].get();
    HalfEdge *l2_3 = halfEdges[10].get();
    HalfEdge *l3_0 = halfEdges[11].get();

    //Right:

    HalfEdge *r0_1 = halfEdges[12].get();
    HalfEdge *r1_2 = halfEdges[13].get();
    HalfEdge *r2_3 = halfEdges[14].get();
    HalfEdge *r3_0 = halfEdges[15].get();


    //Top:

    HalfEdge *t0_1 = halfEdges[16].get();
    HalfEdge *t1_2 = halfEdges[17].get();
    HalfEdge *t2_3 = halfEdges[18].get();
    HalfEdge *t3_0 = halfEdges[19].get();

    //Down:

    HalfEdge *d0_1 = halfEdges[20].get();
    HalfEdge *d1_2 = halfEdges[21].get();
    HalfEdge *d2_3 = halfEdges[22].get();
    HalfEdge *d3_0 = halfEdges[23].get();

    ///
    //CONNECTIONS

    //Front: (complete)

    //Face: (complete)

    front->hEdge = f0_1;

    //Vertices: (complete)
    f0->hEdge = f3_0;
    f1->hEdge = f0_1;
    f2->hEdge = f1_2;
    f3->hEdge = f2_3;
    //HalfEdges:
    //face (complete)
    f0_1->face = front;
    f1_2->face = front;
    f2_3->face = front;
    f3_0->face = front;

    //next (complete)
    f0_1->next = f1_2;
    f1_2->next = f2_3;
    f2_3->next = f3_0;
    f3_0->next = f0_1;

    //sym (complete)
    f0_1->sym = d3_0;
    f1_2->sym = r3_0;
    f2_3->sym = t3_0;
    f3_0->sym = l1_2;

    //hE -> vertex
    f0_1->vertex = f1;
    f1_2->vertex = f2;
    f2_3->vertex = f3;
    f3_0->vertex = f0;


    ////////////////////

    //Back: (complete)

    //Face: (complete)
    back->hEdge = b0_1;
    //Vertices: (complete)
    b0->hEdge = b3_0;
    b1->hEdge = b0_1;
    b2->hEdge = b1_2;
    b3->hEdge = b2_3;
    //HalfEdges:
    //face (complete)
    b0_1->face = back;
    b1_2->face = back;
    b2_3->face = back;
    b3_0->face = back;

    //next (complete)
    b0_1->next = b1_2;
    b1_2->next = b2_3;
    b2_3->next = b3_0;
    b3_0->next = b0_1;

    //sym (complete)
    b0_1->sym = d1_2;
    b1_2->sym = l3_0;
    b2_3->sym = t1_2;
    b3_0->sym = r1_2;

    //hE -> vertex
    b0_1->vertex = b1;
    b1_2->vertex = b2;
    b2_3->vertex = b3;
    b3_0->vertex = b0;

    ////////////////////

    //Left: (complete)

    //Face: (complete)
    left->hEdge = l0_1;
    //Vertices: (complete)


    b1->hEdge = l3_0;
    f0->hEdge = l0_1;
    f3->hEdge = l1_2;
    b2->hEdge = l2_3;

    //HalfEdges:
    //face (complete)
    l0_1->face = left;
    l1_2->face = left;
    l2_3->face = left;
    l3_0->face = left;

    //next (complete)
    l0_1->next = l1_2;
    l1_2->next = l2_3;
    l2_3->next = l3_0;
    l3_0->next = l0_1;

    //sym (complete)
    l0_1->sym = d0_1;
    l1_2->sym = f3_0;
    l2_3->sym = t2_3;
    l3_0->sym = b1_2;

    //hE -> vertex

    l0_1->vertex = f0;
    l1_2->vertex = f3;
    l2_3->vertex = b2;
    l3_0->vertex = b1;

    ////////////////////

    //Right: (complete)

    //Face: (complete)
    right->hEdge = r0_1;
    //Vertices: (complete)


    f1->hEdge = r3_0;
    b0->hEdge = r0_1;
    b3->hEdge = r1_2;
    f2->hEdge = r2_3;
    //HalfEdges:
    //face (complete)
    r0_1->face = right;
    r1_2->face = right;
    r2_3->face = right;
    r3_0->face = right;

    //next (complete)
    r0_1->next = r1_2;
    r1_2->next = r2_3;
    r2_3->next = r3_0;
    r3_0->next = r0_1;

    //sym (complete)
    r0_1->sym = d2_3;
    r1_2->sym = b3_0;
    r2_3->sym = t0_1;
    r3_0->sym = f1_2;

    //hE -> vertex

    r0_1->vertex = b0;
    r1_2->vertex = b3;
    r2_3->vertex = f2;
    r3_0->vertex = f1;

    ////////////////////

    //Top: (complete)

    //Face: (complete)
    top->hEdge = t0_1;
    //Vertices: (complete)


    f2->hEdge = t3_0;
    b3->hEdge = t0_1;
    b2->hEdge = t1_2;
    f3->hEdge = t2_3;
    //HalfEdges:
    //face (complete)
    t0_1->face = top;
    t1_2->face = top;
    t2_3->face = top;
    t3_0->face = top;

    //next (complete)
    t0_1->next = t1_2;
    t1_2->next = t2_3;
    t2_3->next = t3_0;
    t3_0->next = t0_1;

    //sym (complete)
    t0_1->sym = r2_3;
    t1_2->sym = b2_3;
    t2_3->sym = l2_3;
    t3_0->sym = f2_3;


    t0_1->vertex = b3;
    t1_2->vertex = b2;
    t2_3->vertex = f3;
    t3_0->vertex = f2;

    ////////////////////

    //Down: (complete)

    //Face: (complete)
    down->hEdge = d0_1;
    //Vertices: (complete)


    f0->hEdge = d3_0;
    b1->hEdge = d0_1;
    b0->hEdge = d1_2;
    f1->hEdge = d2_3;

    //HalfEdges:
    //face (complete)
    d0_1->face = down;
    d1_2->face = down;
    d2_3->face = down;
    d3_0->face = down;

    //next (complete)
    d0_1->next = d1_2;
    d1_2->next = d2_3;
    d2_3->next = d3_0;
    d3_0->next = d0_1;

    //sym (complete)
    d0_1->sym = l0_1;
    d1_2->sym = b0_1;
    d2_3->sym = r0_1;
    d3_0->sym = f0_1;

    //hE -> vertex
    d0_1->vertex = b1;
    d1_2->vertex = b0;
    d2_3->vertex = f1;
    d3_0->vertex = f0;

    //Face colors:
    front->color = glm::vec3(1, 0, 0);
    back->color = glm::vec3(0.2, 0.2, 0.4);
    left->color = glm::vec3(0, 1, 0);
    right->color = glm::vec3(0.5, 0.5, 0.1);
    top->color = glm::vec3(0, 0, 1);
    down->color = glm::vec3(0.8, 0.7, 0.9);
}

void Mesh::splitEdge(HalfEdge *he1) {
    uPtr<Vertex> v3Heap = mkU<Vertex> ();

    Vertex *v3 = v3Heap.get();

    ////* Step 0: find v1 and v2 *////

    Vertex *v1 = he1->vertex;
    Vertex *v2;
    HalfEdge *curE = he1;
    do {
        //1. update vertex
        v2 = curE->vertex;
        //2. Move to the next halfEdge
        curE = curE->next;
    } while(curE->ID != he1->ID);

    ////* Step 1: Create new vertex *////

        //pos = average of v1 and v2
        //halfEdge = he1 (cause arbitrary!)

    glm::vec3 v3Pos = (v1->pos + v2->pos);
    v3Pos /= 2;

    v3->pos = v3Pos;
    v3->hEdge = he1;
    vertices.push_back(std::move(v3Heap));

    ////* Step 2: Create new half edges *////
    //he2
    HalfEdge *he2 = he1->sym;

    //he1B, he2B
    uPtr<HalfEdge> he1BHeap = mkU<HalfEdge> ();
    uPtr<HalfEdge> he2BHeap = mkU<HalfEdge> ();

    HalfEdge *he1B = he1BHeap.get();
    HalfEdge *he2B = he2BHeap.get();

    he1B->vertex = v1;
    he2B->vertex = v2;

    he1B->face = he1->face;
    he2B->face = he2->face;

    ////* Step 3: Adjust the sym, next and vert pointers of all halfedges*////
    //next
    he1B->next = he1->next;
    he2B->next = he2->next;

    he1->next = he1B;
    he2->next = he2B;

    //vert
    he1->vertex = v3;
    he2->vertex = v3;

    //sym
    he1->sym = he2B;
    he2B->sym = he1;

    he2->sym = he1B;
    he1B->sym = he2;

    //Adjust hEdge pointers of the original vertices!
    v1->hEdge = he1B;
    v2->hEdge = he2B;

    //Add to halfEdges
    halfEdges.push_back(std::move(he1BHeap));
    halfEdges.push_back(std::move(he2BHeap));
}

void Mesh::triangulate(Face* face1) {

    while (true) {
        ////* Step 0: Check if triangle/line *////

        int count = 0;
        HalfEdge *curE = face1->hEdge;
        do {
            count++;
            curE = curE->next;
        } while (curE->ID != face1->hEdge->ID);

        //if a triangle , don't do anything

        if (count < 4) {
            return;
        }

        HalfEdge *he_0 = face1->hEdge;
        ////* Step 1: Create 2 new halfEdges he_A and he_B *////

        uPtr<HalfEdge> he_AHeap = mkU<HalfEdge> ();
        uPtr<HalfEdge> he_BHeap = mkU<HalfEdge> ();

        HalfEdge *he_A = he_AHeap.get();
        HalfEdge *he_B = he_BHeap.get();

        he_A->vertex = he_0->vertex;
        he_B->vertex = he_0->next->next->vertex;

        he_A->sym = he_B;
        he_B->sym = he_A;

        ////* Step 2: Create a second face, face2 *////

        uPtr<Face> face2Heap = mkU<Face> (face1->color);
        Face *face2 = face2Heap.get();

        he_B->face = face1;
        he_A->face = face2;
        he_0->next->face = face2;
        he_0->next->next->face = face2;

        face2->hEdge = he_A;

        ////* Step 3: Fix next pointers *////

        he_B->next = he_0->next->next->next;
        he_0->next->next->next = he_A;
        he_A->next = he_0->next;
        he_0->next = he_B;

        //Add face and halfEdges
        faces.push_back(std::move(face2Heap));
        halfEdges.push_back(std::move(he_AHeap));
        halfEdges.push_back(std::move(he_BHeap));
    }
}





void Mesh::subdivide() {



    int originalFacesCount = faces.size();
    int originalHalfEdgesCount = halfEdges.size();
    int originalVerticesCount = vertices.size();

    ////0. Helpful data structures:

    std::unordered_map<int, Vertex*> faceToCentroidMap;
    std::unordered_set<HalfEdge*> hasBeenSplitSet;

    ////1. Create centroid vertices
    //For every single face, create a centroid vertex, and add it to vertices

//    std::cout << "Creating centroid vertices" << std::endl;
    for (int i = 0; i < faces.size(); i++) {
        Face *f = faces[i].get();

        uPtr<Vertex> centroidHeap = mkU<Vertex> ();
        Vertex* centroid = centroidHeap.get();

        glm::vec3 centroidPos = glm::vec3(0);
        int vertexCount = 0;

        HalfEdge *cur = f->hEdge;
        do {

            centroidPos += cur->vertex->pos;
            vertexCount++;
            cur = cur->next;

        } while (cur->ID != f->hEdge->ID);

        centroidPos /= vertexCount;

        centroid->pos = centroidPos;

        vertices.push_back(std::move(centroidHeap));

        faceToCentroidMap[f->ID] = centroid;
    }

    ////2. Compute smoothed midpoint of each edge in the mesh

//    std::cout << "Creating smoothed midpoints" << std::endl;
    //Iterate through all unique halfEdges in the mesh
    //Well.. we must use faces! not just the halfEdge vector, since we need to ensure we are splitting unique edges from each face
    //For each half Edge,
        //2 cases to consider:
        //Case 1: Edge has 2 faces incident on it
        //Case 2: Edge only has one face incident to it (i.e, it is a border edge

    for (int i = 0; i < originalHalfEdgesCount; i++) {
        HalfEdge *hE = halfEdges[i].get();
//        std::cout << "for halfEdge ID: " << hE->ID <<  " calling split" << std::endl;
        Face *f1 = hE->face;
        Face *f2 = hE->sym->face;

        //1. Call split edge on this halfEdge IF it has not already been split

        if (hasBeenSplitSet.find(hE) == hasBeenSplitSet.end()) {
//            std::cout << "Not been split before, so splitting\n";
            splitEdge(hE);
            //We need to set 4 edges as split!
            hasBeenSplitSet.insert(hE);
            hasBeenSplitSet.insert(hE->sym);
            hasBeenSplitSet.insert(hE->next);
            hasBeenSplitSet.insert(hE->next->sym);

            //New relevant halfEdges are at the end of halfEdges

            Vertex *midpoint = hE->vertex;

            //2. Find the vertices bounding this halfEdge

            HalfEdge *cur = hE;
            Vertex *boundingVertex1;
            do {
                boundingVertex1 = cur->vertex;
                cur = cur->next;
            } while (cur->ID != hE->ID);

            Vertex *boundingVertex2 = hE->next->vertex;

            //boundingVertex1 and 2!!

            //3. Compute midpoints pos!

            if (f2 != nullptr) {
                //Case 1: 2 faces incident
                midpoint->pos = (boundingVertex1->pos + boundingVertex2->pos + faceToCentroidMap[f1->ID]->pos + faceToCentroidMap[f2->ID]->pos) / 4.f;
            } else {
                //Case 2: 1 face only
                midpoint->pos = (boundingVertex1->pos + boundingVertex2->pos + faceToCentroidMap[f1->ID]->pos) / 3.f;
            }
        }
    }

    ////3. Smooth original vertices

    // Iterate through the original vertices, and for each:

        // Adjacent vertices ..?
        // Face 1, then sym, explore face 2, find the half edge pointing to v, then sym, etc... UNTIL we reach face 1 again
        // ^ do this in a for loop

    for (int i = 0; i < originalVerticesCount; i++) {
        std::unordered_set<int> seenMidpoint;


        float n = 0;
        glm::vec3 sumMidpoint = glm::vec3(0);
        glm::vec3 sumCentroid = glm::vec3(0);

        Vertex *v = vertices[i].get();
        HalfEdge *cur = v->hEdge;

        do {
            Face *curFace = cur->face;
            //Centroid
            sumCentroid += faceToCentroidMap[curFace->ID]->pos;

            //Midpoint Vertices (2 of them)
            //1 pointed to by cur, and the other we need to do a forloop

            Vertex *midpoint1 = cur->next->vertex;
            Vertex *midpoint2;

            HalfEdge *cur2 = cur;

            do {
                midpoint2 = cur2->vertex;
                cur2 = cur2->next;
            } while (cur2->ID != cur->ID);

            if (seenMidpoint.find(midpoint2->ID) == seenMidpoint.end()) {
                sumMidpoint += midpoint2->pos;
                seenMidpoint.insert(midpoint2->ID);
                n++;

            }

            if (seenMidpoint.find(midpoint1->ID) == seenMidpoint.end()) {
                sumMidpoint += midpoint1->pos;
                seenMidpoint.insert(midpoint1->ID);
                n++;

            }

            cur = cur->next->sym;
        } while (cur->ID != v->hEdge->ID);

        v->pos = ((n - 2.f) / n) * v->pos + (1 / (n * n)) * (sumMidpoint + sumCentroid);
    }





    ////4. Quadrangulate Function

    //For each of the initial faces, we will create a list of all hEdges pointing to midpoints, and then we will call
    //quadrangulate on the face and the list as inputs
    for (int i = 0; i < originalFacesCount; i++) {

        Face *f = faces[i].get();

        //1. Make list
        std::vector<HalfEdge*> midpointHalfEdges;

        HalfEdge *cur = f->hEdge;
        bool b = true;

        do {
            if (b) {
                midpointHalfEdges.push_back(cur);

            } else {

            }
            b = !b;
            cur = cur->next;
        } while (cur->ID != f->hEdge->ID);

        quadrangulate(faceToCentroidMap[f->ID], midpointHalfEdges);
    }
}

void Mesh::quadrangulate(Vertex* centroid, std::vector<HalfEdge*> v) {

    //For each of the halfEdges in v, we create 2 new half Edges, hE3 and hE4, a new face, and connect them all accordingly
    //Quad 1 (No syms)
    HalfEdge *he0 = v[0];
    HalfEdge *finalHalfEdge = he0->next;

    uPtr<HalfEdge> he1Heap = mkU<HalfEdge> ();
    uPtr<HalfEdge> he2Heap = mkU<HalfEdge> ();
    HalfEdge *he1 = he1Heap.get();
    HalfEdge *he2 = he2Heap.get();
    HalfEdge *he3 = v[v.size() - 1]->next;

    //HalfEdges:
        //face:
    he1->face = he0->face;
    he2->face = he0->face;
    he3->face = he0->face;

        //next:
    he0->next = he1;
    he1->next = he2;
    he2->next = he3;
    he3->next = he0;

        //vertex:
    he1->vertex = centroid;
    he2->vertex = v[v.size() - 1]->vertex;

    //Centroid:
    centroid->hEdge = he1;

    //Add to vectors:
    halfEdges.push_back(std::move(he1Heap));
    halfEdges.push_back(std::move(he2Heap));

    //Middle Quads (sym match behind)
    for (int i = 1; i < v.size() - 1; i++) {
        uPtr<HalfEdge> he1HeapMid = mkU<HalfEdge> ();
        uPtr<HalfEdge> he2HeapMid = mkU<HalfEdge> ();

        HalfEdge *heMid = v[i];
        HalfEdge *he1Mid = he1HeapMid.get();
        HalfEdge *he2Mid = he2HeapMid.get();
        HalfEdge *he3Mid = finalHalfEdge;
        finalHalfEdge = heMid->next;


        float r = static_cast< float >((arc4random_uniform(70))) / 100;
        float g = static_cast< float >(arc4random_uniform(70)) / 100;
        float b = static_cast< float >(arc4random_uniform(70)) / 100;

        uPtr<Face> newFaceHeapMid = mkU<Face> (glm::vec3(r, g, b));
        Face *newFaceMid = newFaceHeapMid.get();

        //FACE:
        //hedge
        newFaceMid->hEdge = heMid;

        //HALFEDGE:

        //face
        heMid->face = newFaceMid;
        he1Mid->face = newFaceMid;
        he2Mid->face = newFaceMid;
        he3Mid->face = newFaceMid;

        //vertex
        he1Mid->vertex = centroid;
        he2Mid->vertex = v[i - 1]->vertex;

        //next
        heMid->next = he1Mid;
        he1Mid->next = he2Mid;
        he2Mid->next = he3Mid;
        he3Mid->next = heMid;

        //sym
        v[i]->next->next->sym = v[i - 1]->next;
        v[i - 1]->next->sym = v[i]->next->next;

        //VERTEX:
        v[i - 1]->vertex->hEdge = he2Mid;

        halfEdges.push_back(std::move(he1HeapMid));
        halfEdges.push_back(std::move(he2HeapMid));

        faces.push_back(std::move(newFaceHeapMid));
    }

    //Final Quad (Sym match with first)

    //new halfEdges
    uPtr<HalfEdge> he1HeapFinal = mkU<HalfEdge>();
    uPtr<HalfEdge> he2HeapFinal = mkU<HalfEdge>();

    HalfEdge *heFinal = v[v.size() - 1];
    HalfEdge *he1Final = he1HeapFinal.get();
    HalfEdge *he2Final = he2HeapFinal.get();
    HalfEdge *he3Final = finalHalfEdge;

    uPtr<Face> newFaceHeapFinal = mkU<Face>(he0->face->color);
    Face *newFaceFinal = newFaceHeapFinal.get();

    //FACE:
    //hedge
    newFaceFinal->hEdge = heFinal;

    //HALFEDGE:

    //face
    heFinal->face = newFaceFinal;
    he1Final->face = newFaceFinal;
    he2Final->face = newFaceFinal;
    he3Final->face = newFaceFinal;

    //vertex
    he1Final->vertex = centroid;
    he2Final->vertex = v[v.size() - 2]->vertex;

    //next
    heFinal->next = he1Final;
    he1Final->next = he2Final;
    he2Final->next = he3Final;
    he3Final->next = heFinal;
    //sym

    v[v.size() - 1]->next->next->sym = v[v.size() - 2]->next;
    v[v.size() - 2]->next->sym = v[v.size() - 1]->next->next;
    v[v.size() - 1]->next->sym = v[0]->next->next;
    v[0]->next->next->sym = v[v.size() - 1]->next;

    //VERTEX:
    v[v.size() - 2]->vertex->hEdge = he3Final;

    halfEdges.push_back(std::move(he1HeapFinal));
    halfEdges.push_back(std::move(he2HeapFinal));
    faces.push_back(std::move(newFaceHeapFinal));
}

void Mesh::importVertexFromOBJ(glm::vec3 vertPos) {
    //faces{}, vertices{}, halfEdges{}
    uPtr<Vertex> vHeap = mkU<Vertex> ();
    Vertex* v = vHeap.get();
    v->pos = vertPos;
    vertices.push_back(std::move(vHeap));
}









void Mesh::constructFacesFromOBJ(std::vector<int> verticesInFace) {

    //0. Create new face

    //random color float

    float r = static_cast< float >((arc4random_uniform(70))) / 100;
    float g = static_cast< float >(arc4random_uniform(70)) / 100;
    float b = static_cast< float >(arc4random_uniform(70)) / 100;

    uPtr<Face> newFaceHeap = mkU<Face> (glm::vec3(r, g, b));
    Face *newFace = newFaceHeap.get();
    faces.push_back(std::move(newFaceHeap));

    //1. Iterate through verticesInFace, each int value will correspond to a vertex in vertices

    ////Very First vertex
    Vertex *vFirst = vertices[verticesInFace[0]].get();

    uPtr<HalfEdge> newHEHeapFirst = mkU<HalfEdge> ();
    HalfEdge *newHEFirst = newHEHeapFirst.get();
    halfEdges.push_back(std::move(newHEHeapFirst));

    newHEFirst->vertex = vFirst;

    //if this vertex has not yet been assigned some half edge,

    if (vFirst->hEdge == nullptr) {
        vFirst->hEdge = newHEFirst;
    }

    //face
    newHEFirst->face = newFace;

    //SYM CHECK (bound1 -> bound2) (sym == bound2 -> bound1)

    Vertex* bound1 = vertices[verticesInFace[verticesInFace.size() - 1]].get();
    Vertex* bound2 = vFirst;

    if (symMap.find(std::make_pair(bound2->ID ,bound1->ID)) == symMap.end()) {
        //Case 1: the sym to this halfEdge has not been discovered yet, so we should add this guy
        symMap.insert(std::make_pair(std::pair<int, int> (bound1->ID, bound2->ID), newHEFirst));
    } else {
        //Set syms!
        symMap.find(std::make_pair(bound2->ID ,bound1->ID))->second->sym = newHEFirst;
        newHEFirst->sym = symMap.find(std::pair<int,int> (bound2->ID ,bound1->ID))->second;
//        symMap.insert(std::make_pair(std::pair<int, int> (bound1->ID, bound2->ID), newHEFirst));
    }

    //Set face's edge pointer
    newFace->hEdge = newHEFirst;

    HalfEdge *previous = newHEFirst;

    //
    ////Mid vertices
    for (int i = 1; i < verticesInFace.size(); i++) {
        Vertex *v = vertices[verticesInFace[i]].get();
        //For each vertex, create a new halfEdge

        uPtr<HalfEdge> newHEHeap = mkU<HalfEdge> ();
        HalfEdge *newHEMid = newHEHeap.get();
        halfEdges.push_back(std::move(newHEHeap));

        newHEMid->vertex = v;

        //if this vertex has not yet been assigned some half edge,
        if (v->hEdge == nullptr) {
            v->hEdge = newHEMid;
        }

        //next pointer
        previous->next = newHEMid;

        //update previous
        previous = newHEMid;

        //set face's hEdge
        newHEMid->face = newFace;

        //SYM CHECK
        //Bounding vertices = previous vert and current vert
        Vertex* bound1Mid = vertices[verticesInFace[i - 1]].get();
        Vertex* bound2Mid = v;

        if (symMap.find(std::make_pair(bound2Mid->ID ,bound1Mid->ID)) == symMap.end()) {
            //Case 1: the sym to this halfEdge has not been discovered yet, so we should add this guy
            symMap.insert(std::make_pair(std::pair<int, int> (bound1Mid->ID, bound2Mid->ID), newHEMid));
        } else {
            //Set syms!
            symMap.find(std::make_pair(bound2Mid->ID ,bound1Mid->ID))->second->sym = newHEMid;
            newHEMid->sym = symMap.find(std::make_pair(bound2Mid->ID ,bound1Mid->ID))->second;
        }
    }
    ////Final iteration
    previous->next = newHEFirst;
}

void Mesh::clearMesh() {
    faces.clear();
    vertices.clear();
    halfEdges.clear();
    symMap.clear();
}
