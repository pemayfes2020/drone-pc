#pragma once

#include <Eigen/Core>

#include <fstream>
#include <iostream>
#include <string>


namespace STL
{

// https://en.wikipedia.org/wiki/STL_(file_format)

struct Binary {

    std::vector<Polygon> polygons;

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

    Binary(const std::string& filepath)
    {
        std::ifstream file(filepath, std::ios::binary | std::ios::in);
        if (!file.is_open()) {
            std::cerr << "[Error] [STL] Cannot open file: " << filepath << std::endl;
            std::exit(EXIT_FAILURE);
        }

        // (0-79 Byte) Comment
        char comment[80];
        file.read(comment, 80);
        std::cout << "[Info] [STL] Load: " << comment << std::endl;

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
            Polygon polygon;

            polygon.normal = parse_point(file);
            polygon.r1 = parse_point(file);
            polygon.r2 = parse_point(file);
            polygon.r3 = parse_point(file);

            [[maybe_unused]] char flag[2];

            file.read(flag, 2);
            polygons.push_back(polygon);
        }
    }
};

}  // namespace STL
