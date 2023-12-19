#include <iostream>
#include <vector>
#include <cmath>
#include <limits>

struct Vec3
{
    float x, y, z;
    Vec3() : x(0), y(0), z(0) {}
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
};

Vec3 operator+(const Vec3 &a, const Vec3 &b)
{
    return Vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vec3 operator-(const Vec3 &a, const Vec3 &b)
{
    return Vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

Vec3 operator*(float t, const Vec3 &v)
{
    return Vec3(t * v.x, t * v.y, t * v.z);
}

float dot(const Vec3 &a, const Vec3 &b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 cross(const Vec3 &a, const Vec3 &b)
{
    return Vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

Vec3 normalize(const Vec3 &v)
{
    float len = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    return Vec3(v.x / len, v.y / len, v.z / len);
}

struct Ray
{
    Vec3 origin, direction;
    Ray(const Vec3 &origin, const Vec3 &direction) : origin(origin), direction(direction) {}
};

struct Sphere
{
    Vec3 center;
    float radius;
    Sphere(const Vec3 &center, float radius) : center(center), radius(radius) {}
};

bool intersect(const Ray &ray, const Sphere &sphere, float &t)
{
    Vec3 oc = ray.origin - sphere.center;
    float a = dot(ray.direction, ray.direction);
    float b = 2.0f * dot(oc, ray.direction);
    float c = dot(oc, oc) - sphere.radius * sphere.radius;
    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
    {
        return false;
    }
    else
    {
        float sqrtDiscriminant = std::sqrt(discriminant);
        float t1 = (-b - sqrtDiscriminant) / (2.0f * a);
        float t2 = (-b + sqrtDiscriminant) / (2.0f * a);
        t = (t1 < t2) ? t1 : t2;
        return true;
    }
}

Vec3 trace(const Ray &ray, const std::vector<Sphere> &spheres)
{
    const float kInfinity = std::numeric_limits<float>::infinity();
    float tNear = kInfinity;
    const Sphere *hitSphere = nullptr;

    for (const Sphere &sphere : spheres)
    {
        float t = kInfinity;
        if (intersect(ray, sphere, t) && t < tNear)
        {
            tNear = t;
            hitSphere = &sphere;
        }
    }

    if (hitSphere)
    {
        // Sphere intersection point
        Vec3 hitPoint = ray.origin + tNear * ray.direction;
        // Normal at the intersection point
        Vec3 normal = normalize(hitPoint - hitSphere->center);
        // Simple shading (Lambertian reflection)
        float intensity = std::max(0.0f, dot(normalize(Vec3(-1, -1, -1)), normal));
        return Vec3(0.8, 0.8, 0.8) * intensity;
    }

    // Background color
    return Vec3(0.2, 0.7, 0.8);
}

int main()
{
    const int width = 800;
    const int height = 600;

    std::vector<Sphere> spheres;
    spheres.push_back(Sphere(Vec3(0, 0, -5), 1.0f));
    spheres.push_back(Sphere(Vec3(0, -1001, 0), 1000.0f));

    Vec3 cameraPos(0, 0, 0);

    std::cout << "P3\n"
              << width << " " << height << "\n255\n";

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            // Normalized screen coordinates
            float u = float(x) / width;
            float v = float(y) / height;
            // Ray direction from camera to pixel on screen
            Vec3 rayDir(u - 0.5f, v - 0.5f, 1.0f);
            rayDir = normalize(rayDir);
            // Create a ray from camera position through the screen pixel
            Ray ray(cameraPos, rayDir);
            // Trace the ray and get the color
            Vec3 color = trace(ray, spheres);
            // Scale color values to 0-255 range and output to ppm image
            int ir = int(255.99 * color.x);
            int ig = int(255.99 * color.y);
            int ib = int(255.99 * color.z);
            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }

    return 0;
}