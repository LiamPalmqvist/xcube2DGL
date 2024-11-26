#pragma once
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
class FileReader
{
private:
	// declaration of the file to be parsed
	ifstream file;
	string line;
	string out;

	// declaration of the basic shader program
	const char* textureFragment = R"glsl(
uniform vec3    iResolution     = vec3(800, 600, 1.0);
uniform float   iTime;
uniform float   iDeltaTime;
uniform float   progress;
uniform float   PI              = 3.1415926;

out vec4 outColor;

float linear_easeIn( float f ) {
    return f;
}

float linear_easeOut( float f ) {
    return f;
}

float linear_easeInOut( float f ) {
    return f;
}

// ========================================================
// ====================== QUADRATIC =======================
// ========================================================

float quadratic_easeIn ( float f ) {
    return f * f;
}

float quadratic_easeOut ( float f ) {
    return -f * (f - 2);
}

float quadratic_easeInOut ( float f ) {
    if (f < 0.5) {
        return 2 * f * f;
    } else {
        return -2 * f * (f - 2) - 1;
    }
}

// ========================================================
// ======================== CUBIC =========================
// ========================================================

float cubic_easeIn ( float f ) {
    return f * f * f;
}

float cubic_easeOut ( float f ) {
    float r = f - 1;
    return r*r*r+1;
}

float cubic_easeInOut ( float f ) {
    float r = f - 1;

    if (r < 0.5) {
        return (4 * f * f * f);
    } else {
        return (4 * r * r * r + 1);
    }
}

// ========================================================
// ======================= QUARTIC ========================
// ========================================================

float quartic_easeIn ( float f ) {
    return f * f * f * f;
}

float quartic_easeOut ( float f ) {
    float r = f - 1;
    return (1 - r * r * r * r);
}

float quartic_easeInOut ( float f ) {
    float r = f - 1;

    if (f < 0.5) {
        return 8 * f * f * f * f;
    } else {
        return -8 * r * r * r * r + 1;
    }
}

// ========================================================
// ======================= QUINTIC ========================
// ========================================================

float quintic_easeIn ( float f ) {
    return f * f * f * f * f;
}

float quintic_easeOut ( float f ) {
    float r = f - 1;

    return 1 + r*r*r*r*r;
}

float quintic_easeInOut ( float f ) {
    float r = f - 1;

    if (f < 0.5) {
        return 16*f*f*f*f*f;
    } else {
        return 16 * r*r*r*r*r + 1;
    }
}

// ========================================================
// ===================== EXPONENTIAL ======================
// ========================================================

float exponential_easeIn ( float f ) {
    if (f == 0.0) {
        return 0.0; 
    } else {
        return pow(2.0, 10 * (f - 1));
    }
}

float exponential_easeOut( float f ) {
    if (f == 1.0) {
        return 1.0;
    } else  {
        return (1 - pow(2.0, -10 * f));
    }
}

float exponential_easeInOut( float f ) {
    if (f == 0.0 || f == 1.0) {
        return f;
    }

    float r = f * 2.0 - 1.0;

    if (r < 0.0) {
        return 0.5 * pow(2.0, 10.0 * r);
    }

    return (1 - 0.5 * pow(2.0, -10.0 * r));
}

// ========================================================
// ======================== SINE ==========================
// ========================================================

float sine_easeIn( float f ) {
    if (f == 1.0) {
        return 1.0;
    }

    return (1 - cos(f * (PI / 2)));
}

float sine_easeOut ( float f ) {
    return sin(f * (PI / 2));
}

float sine_easeInOut ( float f ) {
    return -0.5 * (cos(f * PI) - 1);
}

// ========================================================
// ======================= CIRCULAR =======================
// ========================================================

float circular_easeIn ( float f ) {
    return -(sqrt(1 - f*f) - 1);
}

float circular_easeOut ( float f ) {
    return sqrt(1 - (f - 1) * (f - 1));
}

float circular_easeInOut ( float f ) {
    float r = f * 2;
    float r2 = r - 2;

    if (r < 1) {
        return -0.5 * (sqrt(1 - r * r) - 1);
    } else {
        return 0.5 * (sqrt(1 - r2 * r2) + 1);
    }
}

// ========================================================
// ======================= BOUNCE =========================
// ========================================================

float bounce_easeOut ( float f ) {
    if (f < 1/2.75) {
        return 7.5625*f*f;
    } else if (f < 2/2.75) {
        float r = f - 1.5/2.75;
        return 7.5625*r*r+0.75;
    } else if (f < 2.5/2.75) {
        float r = f-2.25/2.75;
        return 7.5625*r*r+0.9375;
    } else {
        float r = f - 2.625/2.75;
        return 7.5625*r*r+0.984375;
    }
}

float bounce_easeIn ( float f ) {
    return 1 - bounce_easeOut(1 - f);
}

float bounce_easeInOut ( float f ) {
    float r = f * 2;

    if (r < 1) {
        return 0.5 * bounce_easeIn(r);
    } else {
        return 0.5 * bounce_easeOut(r - 1) + 0.5;
    }
}

// ========================================================
// ======================== ELASTIC =======================
// ========================================================

float elastic_easeIn ( float f ) {
    float a = 1;
    float p = 0.3;
    float s = p / 4;

    if (f == 0.0 || f == 1.0) {
        return f;
    }

    float r = f - 1;

    return -(a * pow(2.0, 10 * r) * sin((r - s) * (2 * PI) / p));
}

float elastic_easeOut ( float f ) {
    float a = 1;
    float p = 0.3;
    float s = p / 4;

    if (f == 0.0 || f == 1.0) {
        return f;
    }

    return a * pow(2.0, -10 * f) *  sin((f - s) * (2 * PI) / p) + 1;
}

float elastic_easeInOut ( float f ) {
    float a = 1;
    float p = 0.3;
    float s = p / 4;

    if (f == 0.0 || f == 1.0) {
        return f;
    }

    float r = f*2 - 1;

    if (r < 0) {
        return -0.5 * (a * pow(2.0, 10 * r) * sin((r - s*1.5) * (2 * PI) /(p*1.5)));
    }

    return 0.5 * a * pow(2.0, -10 * r) * sin((r - s*1.5) * (2 * PI) / (p*1.5)) + 1;
}

// ========================================================
// ========================= BACK =========================
// ========================================================

float back_easeIn ( float f ) {
    float s = 1.70158;

    if (f == 1.0) {
        return 1.0;
    }

    return f * f * ((s+1) * f - s);
}

float back_easeOut ( float f ) {
    float s = 1.70158;
    
    if (f == 0.0) {
        return 0.0;
    }

    float r = f - 1;

    return r * r * ((s+1) * r + s) + 1;
}

float back_easeInOut ( float f ) {
    float s = 1.70158;
    
    float r = f * 2;
    float r2 = r - 2;

    if (r < 1) {
        return 0.5*(r*r*((s*1.525+1)*r-s*1.525));
    } else {
        return 0.5*(r2 * r2 * ((s*1.525 + 1) * r2 + s*1.525)+2);
    }
}

// ========================================================
// ======================== RANDOM ========================
// ========================================================

// ========================================================
// ======================== PERLIN ========================
// ========================================================

// MAIN GOES BELOW
)glsl";

public:
	FileReader::FileReader();

	// returns a pointer to the file's contents
	void FileReader::ReadFile(string& fileName, string* output);
	void FileReader::WriteFile();

};

