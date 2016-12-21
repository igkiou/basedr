#include "../include/smoothdielectric.h"


void textdump(const tvec::Vec3f &v) {
    printf("%.4f %.4f %.4f (%.4f)\n", v[0], v[1], v[2], v.length());
}


int main() {
    Float ior1 = 1.0f, ior2 = 1.5f;
    bsdf::SmoothDielectric bsdf(ior1, ior2);
    smp::Sampler sampler;

    tvec::Vec3f in, out;
    tvec::Vec3f n;

    in = tvec::normalize(tvec::Vec3f(1.0f, -0.25f, 0.0f));
    n = tvec::Vec3f(0.0f, 1.0f, 0.0f);

    printf("In = ");
    textdump(in);

    for ( int i = 0; i < 20; ++i ) {
        bsdf.sample(in, n, sampler, out);
        textdump(out);

        if ( tvec::dot(in, n)*tvec::dot(out, n) > 0.0f ) {
            // Test snell's law
            printf("Refract: %.4f %.4f\n", std::abs(in[0])/std::abs(out[0]), ior2/ior1);
        } else
            printf("Reflect\n");
    }

    return 0;
}
