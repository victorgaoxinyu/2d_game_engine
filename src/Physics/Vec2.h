#ifndef VEC2_H
#define VEC2_H  // protection guard

struct Vec2 {
    float x;
    float y;

    Vec2();                                  // empty constructor
    
    Vec2(float x, float y);                  // constructor with parameters
    ~Vec2() = default;                       // destructor, for memory management

    void Add(const Vec2& v);                 // v1.Add(v2), 
                                             // `const` means we are not gonna modify value of v, 
                                             // just need the value for calculation
                                             // Vec2& here means we dont want to pass a copy of obj
                                             // we just want to pass a reference to the obj, point to memory address
    void Sub(const Vec2& v);                 // v2.Sub(v2)
    void Scale(const float n);               // v1.Scale(n)
    Vec2 Rotate(const float angle) const;    // v1.Rotate(angle)
                                             // the last const means we will not change any attribute of this object

    float Magnitude() const;                 // v1.Magnitude()
    float MagnitudeSquared() const;          // v1.MagnitudeSquared()

    Vec2& Normalize();                       // v1.Normalize()
                                             // this func will change v1 inplace
    Vec2 UnitVector() const;                 // v1.UnitVector()
    Vec2 Normal() const;                     // n = v1.Normal()

    float Dot(const Vec2& v) const;          // v1.Dot(v2)
    float Cross(const Vec2& v) const;        // v1.Cross(v2)

    // operator overload
    Vec2& operator = (const Vec2& v);  // v1 = v2;  paramter of operator overload is the RHS of operator
    bool operator == (const Vec2& v) const;  // v1 == v2;
    bool operator != (const Vec2& v) const;  // v1 != v2;

    Vec2 operator + (const Vec2& v) const;   // v1 + v2;
    Vec2 operator - (const Vec2& v) const;   // v1 - v2;
    Vec2 operator * (const float n) const;   // v1 * n;
    Vec2 operator / (const float n) const;   // v1 / n;
    Vec2 operator - ();                      // -v1;
    
    Vec2& operator += (const Vec2& v);       // v1 += v2
    Vec2& operator -= (const Vec2& v);       // v1 -= v2;
    Vec2& operator *= (const float n);       // v1 *= n;
    Vec2& operator /= (const float n);       // v1 /= n;
};

#endif