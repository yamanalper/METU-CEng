#ifndef __HW1__PARSER__
#define __HW1__PARSER__

#include <string>
#include <vector>
#include <math.h>

namespace parser
{
    // Notice that all the structures are as simple as possible
    // so that you are not enforced to adopt any style or design.
    struct Vec3f
    {
        float x, y, z;
        // need default values
        Vec3f()
        {
        }

        Vec3f(float a, float b, float c)
        {
            x = a;
            y = b;
            z = c;
        }

        Vec3f operator+(const Vec3f &v) const
        {
            return Vec3f(x + v.x, y + v.y, z + v.z);
        }

        Vec3f operator-(const Vec3f &v) const
        {
            return Vec3f(x - v.x, y - v.y, z - v.z);
        }

        // Cross product
        Vec3f operator*(const Vec3f &v) const
        {
            return Vec3f((y * v.z) - (z * v.y),
                         (z * v.x) - (x * v.z),
                         (x * v.y) - (y * v.x));
        }

        float dot(const Vec3f &v) const
        {
            return x * v.x + y * v.y + z * v.z;
        }

        Vec3f operator*(const int &i) const
        {
            return Vec3f(x * i, y * i, z * i);
        }

        Vec3f operator*(const float &i) const
        {
            return Vec3f(x * i, y * i, z * i);
        }

        Vec3f operator/(const float &i) const
        {
            return Vec3f(x / i, y / i, z / i);
        }

        Vec3f times(const Vec3f &v) const
        {
            return Vec3f(x * v.x, y * v.y, z * v.z);
        }

        float lenght() const
        {
            return sqrt(x * x + y * y + z * z);
        }

        void normalize()
        {
            float l = lenght();
            x = x / l;
            y = y / l;
            z = z / l;
        }
    };

    struct Vec3i
    {
        int x, y, z;

        Vec3i() {}

        Vec3i(int a, int b, int c)
        {
            x = a;
            y = b;
            z = c;
        }
    };

    struct Vec4f
    {
        float x, y, z, w;
    };

    struct Camera
    {
        Vec3f position;
        Vec3f gaze;
        Vec3f up;
        Vec3f u;
        Vec4f near_plane;
        float near_distance;
        int image_width, image_height;
        std::string image_name;
    };

    struct Ray
    {
        Vec3f o, d;
        int depth;

        Ray(Vec3f oo, Vec3f dd, int dpt)
        {
            o = oo;
            d = dd;
            depth = dpt;
        }
    };

    struct PointLight
    {
        Vec3f position;
        Vec3f intensity;
    };

    struct Material
    {
        bool is_mirror;
        Vec3f ambient;
        Vec3f diffuse;
        Vec3f specular;
        Vec3f mirror;
        float phong_exponent;
    };

    struct Face
    {
        int v0_id;
        int v1_id;
        int v2_id;
        Vec3f face_normal;
    };

    struct Mesh
    {
        int material_id;
        std::vector<Face> faces;
    };

    struct Triangle
    {
        int material_id;
        Face indices;
    };

    struct Sphere
    {
        int material_id;
        int center_vertex_id;
        float radius;
    };

    struct Cylinder
    {
        int material_id;
        int center_vertex_id;
        Vec3f axis;
        float radius;
        float height;
    };

    struct Plane
    {
        int material_id;
        int center_vertex_id;
        Vec3f normal;
    };

    struct hitRecord
    {
        float t;
        Vec3f face_normal;
        int matId;
    };

    struct Scene
    {
        // Data
        Vec3i background_color;
        float shadow_ray_epsilon;
        int max_recursion_depth;
        std::vector<Camera> cameras;
        Vec3f ambient_light;
        std::vector<PointLight> point_lights;
        std::vector<Material> materials;
        std::vector<Vec3f> vertex_data;
        std::vector<Mesh> meshes;
        std::vector<Triangle> triangles;
        std::vector<Sphere> spheres;
        std::vector<Cylinder> cylinders;
        std::vector<Plane> planes;

        // Functions
        void loadFromXml(const std::string &filepath);
    };
    void calculateNormal(parser::Vec3f &v0, parser::Vec3f &v1, parser::Vec3f &v2, parser::Vec3f &normal);
}

#endif
