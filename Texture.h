#pragma once

#include "utilities.h"
#include "Color.h"
#include "Image.h"
#include "Perlin.h"

class Texture {
public:
	virtual ~Texture() = default;
	virtual Color3 value(double u, double v, const Point3& p) const = 0;
};

class SolidColor : public Texture {
public:
	SolidColor(Color3 c) : color_value(c) {}
	SolidColor(double red, double green, double blue) : SolidColor(Color3(red, green, blue)) {}

	Color3 value(double u, double v, const Point3& p) const override {
		return color_value;
	}

private:
	Color3 color_value;
};

class CheckerTexture : public Texture {
public:
    CheckerTexture(double _scale, shared_ptr<Texture> _even, shared_ptr<Texture> _odd)
        : inv_scale(1.0 / _scale), even(_even), odd(_odd) {}

    CheckerTexture(double _scale, Color3 c1, Color3 c2)
        : inv_scale(1.0 / _scale),
        even(make_shared<SolidColor>(c1)),
        odd(make_shared<SolidColor>(c2))
    {}

    Color3 value(double u, double v, const Point3& p) const override {
        auto xInteger = static_cast<int>(std::floor(inv_scale * p.x()));
        auto yInteger = static_cast<int>(std::floor(inv_scale * p.y()));
        auto zInteger = static_cast<int>(std::floor(inv_scale * p.z()));

        bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

        return isEven ? even->value(u, v, p) : odd->value(u, v, p);
    }

private:
    double inv_scale;
    shared_ptr<Texture> even;
    shared_ptr<Texture> odd;
};

class ImageTexture : public Texture {
public:
    ImageTexture(const char* filename) : image(filename) {}

    Color3 value(double u, double v, const Point3& p) const override {
        // If we have no texture data, then return solid cyan as a debugging aid.
        if (image.height() <= 0) return Color3(0, 1, 1);

        // Clamp input texture coordinates to [0,1] x [1,0]
        u = Interval(0, 1).clamp(u);
        v = 1.0 - Interval(0, 1).clamp(v);  // Flip V to image coordinates

        auto i = static_cast<int>(u * image.width());
        auto j = static_cast<int>(v * image.height());
        auto pixel = image.pixel_data(i, j);

        auto color_scale = 1.0 / 255.0;
        return Color3(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
    }

private:
    Image image;
};

class NoiseTexture : public Texture {
public:
    NoiseTexture() {}
    NoiseTexture(double sc) : scale(sc) {}

    Color3 value(double u, double v, const Point3& p) const override {
        auto s = scale * p;
        // direct turbulence
        // return Color3(1, 1, 1) * perlinNoise.turb(s);
        // indirect turbulence
        return Color3(1, 1, 1) * 0.5 * (1 + std::sin(scale * p.z() + 10 * perlinNoise.turb(s)));
    }

private:
    Perlin perlinNoise;
    double scale;
};