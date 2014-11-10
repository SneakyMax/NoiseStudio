#ifndef DIMENSIONS_H
#define DIMENSIONS_H

namespace noises
{
    template<unsigned int NumDimensions>
    class Dimensions<T>
    {
    public:
        Dimensions();

        enum { Number = T };

        static const Dimensions<1> Dim_1D() { return Dimensions<1>; }
        static const Dimensions<2> Dim_2D() { return Dimensions<2>; }
        static const Dimensions<3> Dim_3D() { return Dimensions<3>; }
        static const Dimensions<4> Dim_4D() { return Dimensions<4>; }

        template<unsigned int NumN> Dim_N<NumN>() { return Dimensions<NumN>; }
    }
}

#endif // DIMENSIONS_H
