/*
 * color_classifier.c
 *
 *  Created on: Jun 7, 2025
 *      Author: phvs2
 */

#include "color_classifier.h"
//#include UART library header


//    name, hCenter, sCenter, lCenter, hTol, sTol, lTol;
ColorBucket Buckets[MAX_BUCKETS] = {
    { "red",     30.8f,   0.35f,   0.32f,   20.0f,  0.20f, 0.20f },
    { "blue",   128.4f,   0.13f,   0.33f,   20.0f,  0.20f, 0.20f },
    { "green",   96.3f,   0.29f,   0.32f,   20.0f,  0.20f, 0.20f }
};


size_t bucketCount = 3;

RGB normalizeRGB(uint16_t r, uint16_t g, uint16_t b, uint16_t c) {
    if (c == 0) c = 1;
    float inv = 1.0f / (float)c;
    RGB out = { r * inv, g * inv, b * inv };
    return out;
}

HSL rgbToHsl(RGB in) {
    float mx = fmaxf(in.r, fmaxf(in.g, in.b));
    float mn = fminf(in.r, fminf(in.g, in.b));
    float d = mx - mn;
    float l = (mx + mn) * 0.5f;

    float s = 0.0f;
    if (d > 1e-6f) {
        s = d / (1.0f - fabsf(2.0f * l - 1.0f));
    }

    float h = 0.0f;
    if (d > 1e-6f) {
        if (mx == in.r) h = fmodf((in.g - in.b) / d, 6.0f);
        else if (mx == in.g) h = ((in.b - in.r) / d) + 2.0f;
        else h = ((in.r - in.g) / d) + 4.0f;

        h *= 60.0f;
        if (h < 0.0f) h += 360.0f;
    }

    HSL out = { h, s, l };
    return out;
}

HSL readHslSample(void) {
    color_data_t color;
    tcs34725_get_color_data(&color);
    RGB norm = normalizeRGB(color.red, color.green, color.blue, color.clear);
    return rgbToHsl(norm);
}

// Stub implementations
void calibrateNewBucket(void) { /* Implement via UART */ }
void printBuckets(void) { /* Implement via UART */ }
void editBucketTolerances(void) { /* Implement via UART */ }

int classifyColor(const HSL *hsl) {
    for (int i = 0; i < (int)bucketCount; ++i) {
        ColorBucket *b = &Buckets[i];
        float dh = fabsf(hsl->h - b->hCenter);
        if (dh > 180.0f) dh = 360.0f - dh;

        if (dh <= b->hTol &&
            fabsf(hsl->s - b->sCenter) <= b->sTol &&
            fabsf(hsl->l - b->lCenter) <= b->lTol)
        {
            return i;
        }
    }
    return -1;
}


int getColor(void)
{
    static HSL sample = {0};
    sample = readHslSample();
    int colorID = classifyColor(&sample);

    if (colorID >= 0 && colorID < bucketCount) {
        return colorID; // Return valid color ID
    }
    return -1; // Return -1 for no match
}



