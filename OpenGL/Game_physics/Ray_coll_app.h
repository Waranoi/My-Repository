#ifndef RAY_COLL_APP_H
#define RAY_COLL_APP_H

#include "../Window/Window.h"
#include "../Graphics_node/MeshResource.h"
#include "../Graphics_node/TextureResource.h"
#include "../Graphics_node/ShaderObject.h"
#include "../Graphics_node/GraphicsNode.h"
#include "../Math/My_math.h"
#include "QuadObject.h"
#include "AABBManager.h"
#include "Object.h"

class Ray_coll_app
{
	public:
        Ray_coll_app();
        ~Ray_coll_app();

        /// open app
        bool Open();
        /// run app
        void Run();

        Matrix4D CreateView();
        Matrix4D CreateProjection();
        Object* CreateObject(const char* texture, const char* vs, const char* fs, char* mesh, bool withUV, bool withNormal, float mass);
        void DrawQuads();
        void DrawObjects();
        void DrawForceObject(float dt);
    private:

        ShaderObject shader;
        Display::Window* window;
        float mouseX = 0.0f;
        float mouseY = 0.0f;
        float eyeX = 0.0f;
        float eyeY = 0.0f;
        float eyeZ = 0.0f;
        float matX = 0.0f;
        float matY = 0.0f;

        Vector4D eye = Vector4D(0, 0, 1.0f);
        Vector4D target = Vector4D(0, 0, 0);
        Vector4D up = Vector4D(0, 1, 0);
        Matrix4D view;
        Matrix4D projection;

        //QUADS
        std::vector<QuadObject> quads;
        bool isIntersecting = false;
        Vector4D intersection;

        //OBJECTS
        std::vector<Object*> objects;

        //FORCE OBJECT
        Object *forceObj;
}

#endif //RAY_COLL_APP_H