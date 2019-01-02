#include "image.h"
#include "perlin_noise.h"
#include "FastNoise.h"

/*const vec3 BEACH = vec3(0, 0.1, 0.95);
const vec3 WATER = vec3(0, 0.1, 0.75);
const vec3 FOREST = vec3(0.2, .7, 0.1);
const vec3 JUNGLE = vec3(0.2, .9, 0.1);
const vec3 SNOW = vec3(0.9, 0.9, 0.9);
*/


const vec3 SNOW = vec3(0.8, 0.8, 0.8);
const vec3 TUNDRA = vec3(0.766667, 0.766667, 0.633333);
const vec3 BARE = vec3(0.733333, 0.733333, 0.733333);
const vec3 SCORCHED = vec3(0.6, 0.6, 0.6);
const vec3 TAIGA = vec3(0.8, 0.831373, 0.733333);
const vec3 SHRUBLAND = vec3(0.768627, 0.8, 0.733333);
const vec3 TEMPERATEDESERT = vec3(0.894118, 0.909804, 0.792157);
const vec3 TEMPERATERAINFOREST = vec3(0.643137, 0.768627, 0.658824);
const vec3 DECIDUOUSFOREST = vec3(0.705882, 0.788235, 0.662745);
const vec3 GRASSLAND = vec3(0.768627, 0.831373, 0.666667);
const vec3 TROPICALRAINFOREST = vec3(0.611765, 0.733333, 0.662745);
const vec3 SEASONFOREST = vec3(0.662745, 0.8, 0.643137);
const vec3 SUBTROPICALDESERT = vec3(0.913725, 0.866667, 0.780392);
const vec3 OCEAN = vec3(0.211765, 0.211765, 0.380392);
const vec3 BEACH = vec3(0.356863, 0.517647, 0.678431);

vec3 biome(float e, float m) {      
  if (e < 0.4) return BEACH;
  if (e < 0.3) return OCEAN;
  
  if (e > 0.65) {
    if (m < 0.1) return SCORCHED;
    if (m < 0.2) return BARE;
    if (m < 0.5) return TUNDRA;
    return SNOW;
  }

  if (e > 0.55) {
    if (m < 0.33) return TEMPERATEDESERT;
    if (m < 0.66) return SHRUBLAND;
    return TAIGA;
  }

  if (e > 0.45) {
    if (m < 0.16) return TEMPERATEDESERT;
    if (m < 0.50) return GRASSLAND;
    if (m < 0.83) return DECIDUOUSFOREST;
    return TEMPERATERAINFOREST;
  }

  if (m < 0.16) return SUBTROPICALDESERT;
  if (m < 0.33) return GRASSLAND;
  if (m < 0.66) return SEASONFOREST;
  return TROPICALRAINFOREST;

}

/*vec3 biome(float e, float m)
{
    if (e < 0.4) return BEACH;
    if (e < 0.3) return OCEAN;

    return SEASONFOREST;
}*/

void clamp_vector(float min, float max, vec3 &v)
{
    if(v.x() < min)
        v.e[0] = min;
    else if (v.x() > max)
        v.e[0] = max;

    if(v.y() < min)
        v.e[1] = min;
    else if (v.y() > max)
        v.e[1] = max;

    if(v.z() < min)
        v.e[2] = min;
    else if (v.z() > max)
        v.e[2] = max;
}

int main()
{

    int width = 1920;
    int height = 1080;
    Image im(width, height);

    PerlinNoise pn(6, 0.6);

    FastNoise el(5646);
    el.SetNoiseType(FastNoise::SimplexFractal);
    el.SetFrequency(0.0025);
    el.SetFractalType(FastNoise::FBM);
    el.SetFractalOctaves(8);
    el.SetFractalGain(0.65);

    FastNoise el2(5646);
    el2.SetNoiseType(FastNoise::SimplexFractal);
    el2.SetFrequency(0.006513);
    el2.SetFractalType(FastNoise::FBM);
    el2.SetFractalOctaves(7);
    el2.SetFractalGain(0.6);

    FastNoise ms(564894);
    ms.SetNoiseType(FastNoise::SimplexFractal);
    ms.SetFrequency(0.048);
    ms.SetFractalType(FastNoise::FBM);
    ms.SetFractalOctaves(4);
    ms.SetFractalGain(0.9);

    int offset = width/8;
    
    const float mult = 1.5f;
    for(int y = 0; y < height; ++y)
    {
        for(int x = 0; x < width; ++x)
        {
            float moisture = (ms.GetNoise(x,y)+1.f)/2.f;
            vec3 c1;
            vec3 c2;

            float e1 = (el.GetNoise(x,y)+1.f)/2.f;
            float elevation1 = std::pow(e1, 1.);

            float e2 = (el2.GetNoise(x,y)+1.f)/2.f + 0.175;
            float elevation2 = std::pow(e2, 1.314);

            vec3 c;

            if( x < offset)
                c = biome(e1, moisture)*mult*elevation1;
            else if(x > width - offset)
                c = biome(e2, moisture)*mult*elevation2;
            else
            {
                float t = (x-offset)/(float)(width-offset);

                float elev = (1 - t)*elevation1 + t*elevation2;
                float e = (1 - t)*e1 + t*e2;

                c = biome(e, moisture)*mult*elev; 

            }
                im.set_pixel(x, y, c);

        }
    }
    im.save_as_pbm("Images/", "test");
}