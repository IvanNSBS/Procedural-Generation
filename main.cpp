#include "image.h"
#include "perlin_noise.h"
#include "FastNoise.h"

/*const vec3 BEACH = vec3(0, 0.1, 0.95);
const vec3 WATER = vec3(0, 0.1, 0.75);
const vec3 FOREST = vec3(0.2, .7, 0.1);
const vec3 JUNGLE = vec3(0.2, .9, 0.1);
const vec3 SNOW = vec3(0.9, 0.9, 0.9);
*/


const vec3 SNOW = vec3(0.9, 0.9, 0.9);
const vec3 TUNDRA = vec3(0.866667, 0.866667, 0.733333);
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
  if (e < 0.35) return BEACH;
  if (e < 0.4) return OCEAN;
  
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

int main()
{
    Image im(480, 480);

    PerlinNoise pn(6, 0.6);

    FastNoise el(5646);
    el.SetNoiseType(FastNoise::SimplexFractal);
    el.SetFrequency(0.0025);
    el.SetFractalType(FastNoise::FBM);
    el.SetFractalOctaves(8);
    el.SetFractalGain(0.65);

    FastNoise ms(5646);
    ms.SetNoiseType(FastNoise::SimplexFractal);
    ms.SetFrequency(0.00428);
    ms.SetFractalType(FastNoise::FBM);
    ms.SetFractalOctaves(5);
    ms.SetFractalGain(0.6);
    
    const float mult = 1.8;
    for(int y = 0; y < 480; ++y)
    {
        for(int x = 0; x < 480; ++x)
        {
            float e = (el.GetNoise(x,y)+1.f) /2.f;
            float elevation = std::pow(e,2);
            float moisture = (ms.GetNoise(x,y)+1.f) /2.f;
            vec3 c = biome(e, moisture)*elevation*mult; 

            /*if(e > 0.65)
                c = SNOW*elevation*mult;
            else if(e > 0.55)
                c = JUNGLE*elevation*mult;
            else if (e > 0.45)
                c = FOREST*elevation*mult;
            else if (e > 0.4)
                c = BEACH*elevation*mult;
            else
                c = WATER*elevation*mult; 
            */

            //c = vec3(e, e, e);
            //c = vec3(moisture, moisture, moisture);
            //printf("noise: %f\n", n);
            im.set_pixel(x,y, c);
        }
    }
    im.save_as_pbm("Images/", "test");
}