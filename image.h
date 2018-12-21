#include <vector>
#include <string>
#include <fstream>
#include "vec3.h"

class Image
{
public:
    int width, height;

private:
    std::vector<vec3> buffer;

public:
    Image(int w, int h): width(w), height(h)
    {
        //Seta o tamanho do buffer para o canvas total
        //Necessario, pois pode-se tentar setar um pixel numa posicao
        //ainda nao existente no buffer(ex: buffer tem tamanho 10, mas tentou 
        //setar o pixel na posicao 20)
        this->buffer.reserve(width*height);
    }

    void set_pixel(int x, int y, const vec3& color) 
    {
        buffer[y * width + x] = color;
    }

    void save_as_pbm(const std::string &filepath, const std::string &filename)
    {
        // Save result to a PPM image (keep these flags if you compile under Windows)
        std::ofstream ofs(filepath + filename + ".ppm", std::ios::out | std::ios::binary); 
        ofs << "P6\n" << width << " " << height << "\n255\n"; 
        for (unsigned i = 0; i < width * height; ++i)
        { 
            ofs << (unsigned char)(std::min(double(1), (double)buffer[i].r()) * 255.0) << 
                (unsigned char)(std::min(double(1), (double)buffer[i].g()) * 255.0) << 
                (unsigned char)(std::min(double(1), (double)buffer[i].b()) * 255.0); 
        }
        ofs.close();
    }
};