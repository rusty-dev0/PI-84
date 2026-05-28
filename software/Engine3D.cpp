#include <vector>
#include <cmath>

#include "raylib.h"

using namespace std;

struct triangle
{
    Vector3 p[3];
    Color c;
};

struct mesh
{
    vector<triangle> tris;
};

struct mat4x4
{
    float m[4][4] = { 0 };
};

class Engine3D
{
public:
    Engine3D(int width, int height) : width(width), height(height), fTheta(0.0f)
    {
        InitWindow(width, height, "3D Engine");
        OnUserCreate();
    }

    ~Engine3D()
    {
        CloseWindow();
    }

    bool Run()
    {
        while (!WindowShouldClose())
        {
            float elapsedTime = GetFrameTime();
            OnUserUpdate(elapsedTime);
        }
        return true;
    }

private:
    int width;
    int height;
    mesh meshCube;
    mat4x4 matProj;
    float fTheta;
    Vector3 vCamera;

    void MultiplyMatrixVector(Vector3 &i, Vector3 &o, mat4x4 &m)
    {
        o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
        o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
        o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
        float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

        if (w != 0.0f)
        {
            o.x /= w;
            o.y /= w;
            o.z /= w;
        }
    }

    bool OnUserCreate()
    {
        SetTargetFPS(60);

        meshCube.tris =
        {
            // south
            { 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f,  RED },
            { 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f,  RED },

            // east                                                      
            { 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,  GREEN },
            { 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f,  GREEN },

            // north                                                     
            { 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f,  RED },
            { 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f,  RED },

            // west                                                      
            { 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f,  GREEN },
            { 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f,  GREEN },

            // top                                                       
            { 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f,  BLUE },
            { 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f,  BLUE },

            // bottom                                                    
            { 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,  BLUE },
            { 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f,  BLUE },
        };

        // projection matrix
        float fNear = 0.1f;
        float fFar = 1000.0f;
        float fFov = 90.0f;
        float fAspectRatio = (float)height / (float)width;
        float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

        matProj.m[0][0] = fAspectRatio * fFovRad;
        matProj.m[1][1] = fFovRad;
        matProj.m[2][2] = fFar / (fFar - fNear);
        matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
        matProj.m[2][3] = 1.0f;
        matProj.m[3][3] = 0.0f;
        return true;
    }

    bool OnUserUpdate(float fElapsedTime)
    {
        BeginDrawing();

            ClearBackground(BLACK);

            mat4x4 matRotZ, matRotX;
            fTheta += 1.0f * fElapsedTime;

            // rotation Z
            matRotZ.m[0][0] = cosf(fTheta);
            matRotZ.m[0][1] = sinf(fTheta);
            matRotZ.m[1][0] = -sinf(fTheta);
            matRotZ.m[1][1] = cosf(fTheta);
            matRotZ.m[2][2] = 1;
            matRotZ.m[3][3] = 1;

            // rotation X
            matRotX.m[0][0] = 1;
            matRotX.m[1][1] = cosf(fTheta * 0.5f);
            matRotX.m[1][2] = sinf(fTheta * 0.5f);
            matRotX.m[2][1] = -sinf(fTheta * 0.5f);
            matRotX.m[2][2] = cosf(fTheta * 0.5f);
            matRotX.m[3][3] = 1;

            // draw triangles
            for (auto tri : meshCube.tris)
            {
                triangle triProjected, triTranslated, triRotatedZ, triRotatedZX;

                // rotate around Z-axis
                MultiplyMatrixVector(tri.p[0], triRotatedZ.p[0], matRotZ);
                MultiplyMatrixVector(tri.p[1], triRotatedZ.p[1], matRotZ);
                MultiplyMatrixVector(tri.p[2], triRotatedZ.p[2], matRotZ);

                // rotate around X-axis
                MultiplyMatrixVector(triRotatedZ.p[0], triRotatedZX.p[0], matRotX);
                MultiplyMatrixVector(triRotatedZ.p[1], triRotatedZX.p[1], matRotX);
                MultiplyMatrixVector(triRotatedZ.p[2], triRotatedZX.p[2], matRotX);

                // offset into the screen
                triTranslated = triRotatedZX;
                triTranslated.p[0].z = triRotatedZX.p[0].z + 3.0f;
                triTranslated.p[1].z = triRotatedZX.p[1].z + 3.0f;
                triTranslated.p[2].z = triRotatedZX.p[2].z + 3.0f;

                Vector3 normal, line1, line2;
                line1.x = triTranslated.p[1].x - triTranslated.p[0].x;
                line1.y = triTranslated.p[1].y - triTranslated.p[0].y;
                line1.z = triTranslated.p[1].z - triTranslated.p[0].z;

                line2.x = triTranslated.p[2].x - triTranslated.p[0].x;
                line2.y = triTranslated.p[2].y - triTranslated.p[0].y;
                line2.z = triTranslated.p[2].z - triTranslated.p[0].z;

                normal.x = line1.y * line2.z - line1.z * line2.y;
                normal.y = line1.z * line2.x - line1.x * line2.z;
                normal.z = line1.x * line2.y - line1.y * line2.x;

                float l = sqrtf(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
                normal.x /= l;
                normal.y /= l;
                normal.z /= l;

                // if (normal.z < 0)
                if(normal.x * (triTranslated.p[0].x - vCamera.x) + 
                    normal.y * (triTranslated.p[0].y - vCamera.y) +
                    normal.z * (triTranslated.p[0].z - vCamera.z) < 0.0f)
                {
                    // illumination
                    Vector3 lightDirection = { 0.0f, 0.0f, -1.0f };
                    float l = sqrtf(lightDirection.x * lightDirection.x + lightDirection.y * lightDirection.y + lightDirection.z * lightDirection.z);
                    lightDirection.x /= l;
                    lightDirection.y /= l;
                    lightDirection.z /= l;

                    float dot = abs(normal.x * lightDirection.x + normal.y * lightDirection.y + normal.z * lightDirection.z);
                    float ambiant = 0.0f;
                    float intensity = ambiant + dot * (1.0f - ambiant);
                    
                    Color faceColor = tri.c;
                    faceColor.r = (unsigned char)(faceColor.r * intensity);
                    faceColor.g = (unsigned char)(faceColor.g * intensity);
                    faceColor.b = (unsigned char)(faceColor.b * intensity);

                    // project triangles to the 2D screen
                    MultiplyMatrixVector(triTranslated.p[0], triProjected.p[0], matProj);
                    MultiplyMatrixVector(triTranslated.p[1], triProjected.p[1], matProj);
                    MultiplyMatrixVector(triTranslated.p[2], triProjected.p[2], matProj);

                    // scale into view
                    triProjected.p[0].x += 1.0f; triProjected.p[0].y += 1.0f;
                    triProjected.p[1].x += 1.0f; triProjected.p[1].y += 1.0f;
                    triProjected.p[2].x += 1.0f; triProjected.p[2].y += 1.0f;

                    triProjected.p[0].x *= 0.5f * (float)width;
                    triProjected.p[0].y *= 0.5f * (float)height;
                    triProjected.p[1].x *= 0.5f * (float)width;
                    triProjected.p[1].y *= 0.5f * (float)height;
                    triProjected.p[2].x *= 0.5f * (float)width;
                    triProjected.p[2].y *= 0.5f * (float)height;

                    DrawTriangle(
                        Vector2{triProjected.p[0].x, triProjected.p[0].y},
                        Vector2{triProjected.p[1].x, triProjected.p[1].y},
                        Vector2{triProjected.p[2].x, triProjected.p[2].y},
                        faceColor
                    );

                    // for debug later
                    // DrawTriangleLines(
                    //     Vector2{triProjected.p[0].x, triProjected.p[0].y},
                    //     Vector2{triProjected.p[1].x, triProjected.p[1].y},
                    //     Vector2{triProjected.p[2].x, triProjected.p[2].y},
                    //     faceColor <-- you can make this white to see a wireframe or something
                    // );
                }
            }

        EndDrawing();
        return true;
    }

};