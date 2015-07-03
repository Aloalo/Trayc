/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef FD_FUNCTIONRASTERIZER_H
#define FD_FUNCTIONRASTERIZER_H

#include "FunctionDrawer.h"
#include <Engine/Geometry/IndexContainer.h>

struct Batch
{
    GLuint VAO;
    GLuint VBO;
    GLsizei count;
};


class FunctionRasterizer : public FunctionDrawer
{
public:
    FunctionRasterizer(void);
    ~FunctionRasterizer(void);

    void SetFunction(const std::string &F, const std::string &Fx, const std::string &Fy);
    void ApplyFunction();

    virtual void Draw(const engine::Camera &cam) const override;

private:
    int GenerateMesh(int ctVertices);
    int GenerateIndices(int ctVertices);

    std::vector<Batch> mBatches;
    GLuint mIBO;
    GLsizei mCtIndices;
};

#endif
