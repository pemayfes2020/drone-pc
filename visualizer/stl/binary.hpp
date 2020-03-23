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
            union {
                char buf[4 * 3 * 4 + 2];
                float n[3];
                float r1[3];
                float r2[3];
                float r3[3];
                uint16_t flag_unused;
            } p;
            file.read(p.buf, sizeof(p));
            polygons.push_back(
                Polygon{
                    Eigen::Vector3f{p.n[0], p.n[1], p.n[2]},
                    Eigen::Vector3f{p.r1[0], p.r1[1], p.r1[2]},
                    Eigen::Vector3f{p.r2[0], p.r2[1], p.r2[2]},
                    Eigen::Vector3f{p.r3[0], p.r3[1], p.r3[2]}});
        }
    }
};

}  // namespace STL
