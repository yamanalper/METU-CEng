#include <iostream>
#include "parser.h"
#include "ppm.h"
#include <stdio.h>
#include <pthread.h>

using namespace parser;

#define INT_MAX 2147483647

int num_threads = 8;

struct ThreadArgs
{
    int start_row, end_row;
    const Scene *scene;
    const Camera *cam;
    unsigned char *image;
};

typedef unsigned char RGB[3];
Vec3f computeColor(Ray r, int max_depth, Vec3f background_color, Scene &scene, float t_max);
Vec3f shading(Ray r, Vec3f hit_point, Vec3f face_normal, int matId, Scene &scene, Vec3f background_color, float t);
bool anyHit(const Scene &scene, Ray &r, float t_max);

float computeDeterminant(Vec3f v1, Vec3f v2, Vec3f v3)
{
    return v1.x * (v2.y * v3.z - v2.z * v3.y) +
           v1.y * (v2.z * v3.x - v2.x * v3.z) +
           v1.z * (v2.x * v3.y - v2.y * v3.x);
}

bool intersectionTest(const Scene &scene, Ray &r, float t_max, hitRecord &rec)
{
    bool hit = false;
    rec.t = t_max;
    float eps = 1e-7f;

    for (auto &triangle : scene.triangles)
    {

        Vec3f a = scene.vertex_data[triangle.indices.v0_id - 1];
        Vec3f b = scene.vertex_data[triangle.indices.v1_id - 1];
        Vec3f c = scene.vertex_data[triangle.indices.v2_id - 1];

        float detA = computeDeterminant(a - b, a - c, r.d);
        float detT = computeDeterminant(a - b, a - c, a - r.o);

        float temp = detT / detA;

        if (temp > rec.t || temp < 0)
            continue;

        float detBeta = computeDeterminant(a - r.o, a - c, r.d);
        float detGama = computeDeterminant(a - b, a - r.o, r.d);
        float beta = detBeta / detA;
        float gama = detGama / detA;

        if (beta < -eps || gama < -eps || 1 + eps < gama + beta)
            continue;

        rec.matId = triangle.material_id;
        rec.t = temp;
        rec.face_normal = triangle.indices.face_normal;
        hit = true;
    }

    for (auto mesh : scene.meshes)
    {
        for (auto face : mesh.faces)
        {
            Vec3f a = scene.vertex_data[face.v0_id - 1];
            Vec3f b = scene.vertex_data[face.v1_id - 1];
            Vec3f c = scene.vertex_data[face.v2_id - 1];

            float detA = computeDeterminant(a - b, a - c, r.d);
            float detT = computeDeterminant(a - b, a - c, a - r.o);

            float temp = detT / detA;

            if (temp > rec.t || temp < 0)
                continue;

            float detBeta = computeDeterminant(a - r.o, a - c, r.d);
            float detGama = computeDeterminant(a - b, a - r.o, r.d);
            float beta = detBeta / detA;
            float gama = detGama / detA;

            if (beta < -eps || gama < -eps || 1 + eps < gama + beta)
                continue;

            rec.matId = mesh.material_id;
            rec.t = temp;
            rec.face_normal = face.face_normal;
            hit = true;
        }
    }

    for (auto &plane : scene.planes)
    {

        Vec3f center = scene.vertex_data[plane.center_vertex_id - 1];
        float temp = (center - r.o).dot(plane.normal) / r.d.dot(plane.normal);

        if (temp < rec.t && temp > 0)
        {
            rec.t = temp;
            rec.matId = plane.material_id;
            rec.face_normal = plane.normal;
            hit = true;
        }
    }

    for (auto &sphere : scene.spheres)
    {
        Vec3f center = scene.vertex_data[sphere.center_vertex_id - 1];
        Vec3f oc = r.o - center;

        float a = r.d.dot(r.d);
        float b = oc.dot(r.d);
        float c = oc.dot(oc) - (sphere.radius * sphere.radius);

        if ((b * b - a * c) < 0)
        {
            continue;
        }

        float sq = sqrt(b * b - a * c);

        float t1 = (-b - sq) / a;
        float t2 = (-b + sq) / a;

        if (t1 < rec.t && t1 > 0)
        {
            Vec3f phit = r.o + r.d * t1;
            phit = phit - center;
            phit.normalize();
            rec.t = t1;
            rec.matId = sphere.material_id;
            rec.face_normal = phit;
            hit = true;
        }
    }

    for (auto &cylinder : scene.cylinders)
    {
        Vec3f center = scene.vertex_data[cylinder.center_vertex_id - 1];
        Vec3f axis = cylinder.axis;
        Vec3f oc = r.o - center;

        float d_dot_a = r.d.dot(axis);
        float oc_dot_a = oc.dot(axis);

        float A = r.d.dot(r.d) - d_dot_a * d_dot_a;
        float B = 2.0f * (oc.dot(r.d) - d_dot_a * oc_dot_a);
        float C = oc.dot(oc) - oc_dot_a * oc_dot_a - cylinder.radius * cylinder.radius;

        if (fabs(A) < 1e-8f)
            continue;
        float disc = B * B - 4.0f * A * C;
        if (disc < 0.0f)
            continue;

        float sqrt_disc = sqrtf(disc);
        float invA = 0.5f / A;
        float t0 = (-B - sqrt_disc) * invA;
        float t1 = (-B + sqrt_disc) * invA;

        float t_side = -1.0f;
        for (float t_hit : {t0, t1})
        {
            if (t_hit < 1e-3f || t_hit > rec.t)
                continue;
            float proj = oc_dot_a + d_dot_a * t_hit;
            if (proj < -0.5f * cylinder.height || proj > 0.5f * cylinder.height)
                continue;
            t_side = t_hit;
            break;
        }

        float t_cap = -1.0f;
        float denom = r.d.dot(axis);
        if (fabs(denom) > 1e-6f)
        {
            Vec3f top = center + axis * (0.5f * cylinder.height);
            Vec3f bottom = center - axis * (0.5f * cylinder.height);

            float t_top = (top - r.o).dot(axis) / denom;
            if (t_top > 1e-3f && t_top < rec.t)
            {
                Vec3f p = r.o + r.d * t_top;
                if ((p - top).dot(p - top) <= cylinder.radius * cylinder.radius)
                {
                    t_cap = t_top;
                    rec.face_normal = axis;
                }
            }

            float t_bottom = (bottom - r.o).dot(axis) / denom;
            if (t_bottom > 1e-3f && t_bottom < rec.t)
            {
                Vec3f p = r.o + r.d * t_bottom;
                if ((p - bottom).dot(p - bottom) <= cylinder.radius * cylinder.radius)
                {
                    if (t_cap < 0 || t_bottom < t_cap)
                    {
                        t_cap = t_bottom;
                        rec.face_normal = axis * -1;
                    }
                }
            }
        }

        if (t_side > 0.0f && (t_cap < 0.0f || t_side < t_cap))
        {
            Vec3f p = r.o + r.d * t_side;
            Vec3f n = p - (center + axis * axis.dot(p - center));
            n.normalize();
            rec.face_normal = n;
            rec.t = t_side;
            rec.matId = cylinder.material_id;
            hit = true;
        }
        else if (t_cap > 0.0f)
        {
            rec.t = t_cap;
            rec.matId = cylinder.material_id;
            hit = true;
        }
    }

    return hit;
}

Vec3f shading(Ray r, Vec3f hit_point, Vec3f face_normal, int matId, Scene &scene, Vec3f background_color, float t)
{
    Vec3f color = scene.ambient_light.times(scene.materials[matId - 1].ambient);

    // std::cout << "color after ambient: " << color.x << " " << color.y << " " << color.z << std::endl;
    if (scene.materials[matId - 1].is_mirror)
    {
        Vec3f reflect_dir = r.d - face_normal * (2.0f * r.d.dot(face_normal));
        reflect_dir.normalize();
        Ray reflected_ray(r.o + r.d * t + face_normal * scene.shadow_ray_epsilon, reflect_dir, r.depth + 1);
        color = color + computeColor(reflected_ray, scene.max_recursion_depth, background_color, scene, INT_MAX).times(scene.materials[matId - 1].mirror);
    }
    for (auto &light : scene.point_lights)
    {
        Vec3f light_dir = light.position - hit_point;
        float light_dist = light_dir.lenght();
        light_dir.normalize();
        Ray shadow_ray(hit_point + face_normal * scene.shadow_ray_epsilon, light_dir, r.depth + 1);

        if (!anyHit(scene, shadow_ray, light_dist))
        {
            float diff = std::max(0.0f, face_normal.dot(light_dir));
            if (diff <= 0.0f)
                continue;
            Vec3f view_dir = r.o - hit_point;
            view_dir.normalize();
            Vec3f half_vector = light_dir + view_dir;
            half_vector.normalize();
            float spec = pow(std::max(0.0f, face_normal.dot(half_vector)), scene.materials[matId - 1].phong_exponent);

            color = color + (light.intensity.times(scene.materials[matId - 1].diffuse * diff) / (light_dist * light_dist)) + (light.intensity.times(scene.materials[matId - 1].specular * spec / (light_dist * light_dist)));
        }
    }

    return color;
}

bool anyHit(const Scene &scene, Ray &r, float t_max)
{
    hitRecord h;
    return intersectionTest(scene, r, t_max, h);
}

Vec3f computeColor(Ray r, int max_depth, Vec3f background_color, Scene &scene, float t_max)
{
    hitRecord h;

    if (r.depth > max_depth)
        return Vec3f(0, 0, 0); // Bacground color or 0, 0, 0??

    if (!intersectionTest(scene, r, t_max, h))
    {
        if (r.depth == 0)
            return background_color;
        else
            return Vec3f(0, 0, 0);
    }

    return shading(r, r.o + r.d * h.t, h.face_normal, h.matId, scene, background_color, h.t);
}

void *render_rows(void *arg)
{
    ThreadArgs *data = static_cast<ThreadArgs *>(arg);
    const Scene &scene = *data->scene;
    const Camera &cam = *data->cam;

    int width = cam.image_width;
    int height = cam.image_height;

    Vec3f topLeft = cam.position + (cam.gaze * cam.near_distance) + (cam.u * cam.near_plane.x) + (cam.up * cam.near_plane.w);
    float pixel_w = (cam.near_plane.y - cam.near_plane.x) / width;
    float pixel_h = (cam.near_plane.w - cam.near_plane.z) / height;
    topLeft = topLeft + (cam.up * (-0.5f * pixel_h)) + (cam.u * (0.5f * pixel_w));

    Vec3f background(scene.background_color.x,
                     scene.background_color.y,
                     scene.background_color.z);

    for (int j = data->start_row; j < data->end_row; ++j)
    {
        for (int i = 0; i < width; ++i)
        {
            Vec3f pixel_pos = topLeft + (cam.u * (i * pixel_w)) + (cam.up * (j * -pixel_h));
            Vec3f ray_dir = pixel_pos - cam.position;
            ray_dir.normalize();

            Ray r(cam.position, ray_dir, 0);
            Vec3f color = computeColor(r, scene.max_recursion_depth, background, const_cast<Scene &>(scene), INT_MAX);

            color.x = std::min(255.0f, std::max(0.0f, color.x));
            color.y = std::min(255.0f, std::max(0.0f, color.y));
            color.z = std::min(255.0f, std::max(0.0f, color.z));

            int idx = (j * width + i) * 3;
            data->image[idx + 0] = static_cast<unsigned char>(color.x);
            data->image[idx + 1] = static_cast<unsigned char>(color.y);
            data->image[idx + 2] = static_cast<unsigned char>(color.z);
        }
    }

    pthread_exit(nullptr);
}

int main(int argc, char *argv[])
{
    parser::Scene scene;

    scene.loadFromXml(argv[1]);

    for (auto &cam : scene.cameras)
    {
        int width = cam.image_width;
        int height = cam.image_height;

        unsigned char *image = new unsigned char[width * height * 3];

        pthread_t threads[num_threads];
        ThreadArgs args[num_threads];

        int rows_per_thread = (height + num_threads - 1) / num_threads;

        for (int t = 0; t < num_threads; ++t)
        {
            args[t].start_row = t * rows_per_thread;
            args[t].end_row = std::min(height, (t + 1) * rows_per_thread);
            args[t].scene = &scene;
            args[t].cam = &cam;
            args[t].image = image;

            pthread_create(&threads[t], nullptr, render_rows, &args[t]);
        }

        for (int t = 0; t < num_threads; ++t)
        {
            pthread_join(threads[t], nullptr);
        }

        write_ppm(cam.image_name.c_str(), image, width, height);
        delete[] image;
    }

    return 0;
}
