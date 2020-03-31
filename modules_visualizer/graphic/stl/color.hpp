#pragma once

#include "graphic/color.hpp"

#include <Eigen/Core>

#include <fstream>
#include <iostream>
#include <string>

namespace STL
{

// https://en.wikipedia.org/wiki/STL_(file_format)

struct ColorBinary {

    Color overall;
    std::vector<ColoredPolygon> polygons;

    float parse_float(std::ifstream& file)
    {
        union {
            char buf[4];
            float val;
        } data;
        file.read(data.buf, 4);
        return data.val;
    }

    Eigen::Vector3f parse_point(std::ifstream& file)
    {
        return Eigen::Vector3f{
            parse_float(file),
            parse_float(file),
            parse_float(file)};
    }

    ColorBinary(const std::string& filepath)
    {
        std::ifstream file(filepath, std::ios::binary | std::ios::in);
        if (!file.is_open()) {
            std::cerr << "[Error] [STL] Cannot open file: " << filepath << std::endl;
            std::exit(EXIT_FAILURE);
        }

        // (0-23 Byte) Comment
        {
            char comment[24];
            file.read(comment, 24);
            std::cout << "[Info] [STL] Load: " << comment << std::endl;
        }

        // (24-27 Byte) Overall Color
        {
            char color_buf[4];
            file.read(color_buf, 4);
            overall(0) = color_buf[0] / 255.0;
            overall(1) = color_buf[1] / 255.0;
            overall(2) = color_buf[2] / 255.0;
        }

        // (28-79) skip (Material Info, etc)
        {
            char skip[52];
            file.read(skip, 52);
        }

        // (80-83 Byte) Polygon総数
        union {
            char buf[4];
            int32_t N;
        } size_data;
        file.read(size_data.buf, 4);
        polygons.resize(size_data.N);

        // (84-) Polygon情報
        // STLファイルはlittle endian
        for (int i = 0; i < size_data.N; ++i) {
            ColoredPolygon polygon;

            polygon.normal = parse_point(file);
            polygon.r1 = parse_point(file);
            polygon.r2 = parse_point(file);
            polygon.r3 = parse_point(file);

            union {
                char flag[2];
                struct {
                    unsigned red : 5;
                    unsigned green : 5;
                    unsigned blue : 5;
                    bool not_unique;  // 0でunique, 1でoverall
                } color;
            } data;
            file.read(data.flag, 2);
            polygon.unique = !data.color.not_unique;
            if (not data.color.not_unique) {
                polygon.color(0) = data.color.red / 31.0;
                polygon.color(1) = data.color.green / 31.0;
                polygon.color(2) = data.color.blue / 31.0;
            }

            polygons.push_back(polygon);
        }
    }
};

}  // namespace STL
