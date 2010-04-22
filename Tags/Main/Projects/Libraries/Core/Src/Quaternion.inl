namespace BM
{
#pragma region Constructors

    inline Quaternion::Quaternion()
    {
        X = 0.0f;
        Y = 0.0f;
        Z = 0.0f;
        W = 0.0f;
    }

    inline Quaternion::Quaternion(Float32 x, Float32 y, Float32 z, Float32 w)
    {
        X = x;
        Y = y;
        Z = z;
        W = w;
    }

    inline Quaternion::Quaternion(const Quaternion& q)
    {
        X = q.X;
        Y = q.Y;
        Z = q.Z;
        W = q.W;
    }

    inline Quaternion::Quaternion(const D3DXQUATERNION& q)
    {
        X = q.x;
        Y = q.y;
        Z = q.z;
        W = q.w;
    }

#pragma endregion

#pragma region Operators

    inline Quaternion::operator D3DXQUATERNION& ()
    {
        return *(D3DXQUATERNION*)&X;
    }

    inline Quaternion::operator const D3DXQUATERNION& () const
    {
        return *(const D3DXQUATERNION*)&X;
    }

    inline Quaternion::operator D3DXQUATERNION* ()
    {
        return (D3DXQUATERNION*)&X;
    }

    inline Quaternion::operator const D3DXQUATERNION* () const
    {
        return (const D3DXQUATERNION*)&X;
    }

    inline Quaternion::operator Float32* ()
    {
        return (Float32*)&X;
    }

    inline Quaternion::operator const Float32* () const
    {
        return (const Float32*)&X;
    }

    inline Float32& Quaternion::operator [] (int i)
    {
        BM_Assert(i >= 0 && i < 4);
        return *(&X + i);
    }

    inline Float32 Quaternion::operator [] (int i) const
    {
        BM_Assert(i >= 0 && i < 4);
        return *(&X + i);
    }

    inline bool Quaternion::operator == (const Quaternion& q) const
    {
        return (X == q.X && Y == q.Y && Z == q.Z && W == q.W);
    }

    inline bool Quaternion::operator != (const Quaternion& q) const
    {
        return (X != q.X || Y != q.Y || Z != q.Z || W != q.W);
    }

    inline bool Quaternion::operator < (const Quaternion& q) const
    {
        return (X < q.X && Y < q.Y && Z < q.Z && W < q.W);
    }

    inline bool Quaternion::operator <= (const Quaternion& q) const
    {
        return (X <= q.X && Y <= q.Y && Z <= q.Z && W <= q.W);
    }

    inline bool Quaternion::operator > (const Quaternion& q) const
    {
        return (X > q.X && Y > q.Y && Z > q.Z && W > q.W);
    }

    inline bool Quaternion::operator >= (const Quaternion& q) const
    {
        return (X >= q.X && Y >= q.Y && Z >= q.Z && W >= q.W);
    }

    inline Quaternion& Quaternion::operator = (const Quaternion& q)
    {
        X = q.X;
        Y = q.Y;
        Z = q.Z;
        W = q.W;
        return *this;
    }

    inline Quaternion& Quaternion::operator += (const Quaternion& q)
    {
        X += q.X;
        Y += q.Y;
        Z += q.Z;
        W += q.W;
        return *this;
    }

    inline Quaternion& Quaternion::operator -= (const Quaternion& q)
    {
        X -= q.X;
        Y -= q.Y;
        Z -= q.Z;
        W -= q.W;
        return *this;
    }

    inline Quaternion& Quaternion::operator *= (const Quaternion& q)
    {
        Quaternion t(*this);
        X = t.W * q.X + t.X * q.W + t.Y * q.Z - t.Z * q.Y;
        Y = t.W * q.Y - t.X * q.Z + t.Y * q.W + t.Z * q.X;
        Z = t.W * q.Z + t.X * q.Y - t.Y * q.X + t.Z * q.W;
        W = t.W * q.W - t.X * q.X - t.Y * q.Y - t.Z * q.Z;
        return *this;
    }

    inline Quaternion& Quaternion::operator += (Float32 f)
    {
        X += f;
        Y += f;
        Z += f;
        W += f;
        return *this;
    }

    inline Quaternion& Quaternion::operator -= (Float32 f)
    {
        X -= f;
        Y -= f;
        Z -= f;
        W -= f;
        return *this;
    }

    inline Quaternion& Quaternion::operator *= (Float32 f)
    {
        X *= f;
        Y *= f;
        Z *= f;
        W *= f;
        return *this;
    }

    inline Quaternion& Quaternion::operator /= (Float32 f)
    {
        X /= f;
        Y /= f;
        Z /= f;
        W /= f;
        return *this;
    }

    inline Quaternion Quaternion::operator + () const
    {
        return *this;
    }

    inline Quaternion Quaternion::operator - () const
    {
        return Quaternion(-X, -Y, -Z, -W);
    }

    inline Quaternion Quaternion::operator + (const Quaternion& q) const
    {
        return Quaternion(X + q.X, Y + q.Y, Z + q.Z, W + q.W);
    }

    inline Quaternion Quaternion::operator - (const Quaternion& q) const
    {
        return Quaternion(X - q.X, Y - q.Y, Z - q.Z, W - q.W);
    }

    inline Quaternion Quaternion::operator * (const Quaternion& q) const
    {
        Quaternion t(*this);
        t *= q;
        return t;
    }

    inline Quaternion Quaternion::operator + (Float32 f) const
    {
        return Quaternion(X + f, Y + f, Z + f, W + f);
    }

    inline Quaternion Quaternion::operator - (Float32 f) const
    {
        return Quaternion(X - f, Y - f, Z - f, W - f);
    }

    inline Quaternion Quaternion::operator * (Float32 f) const
    {
        return Quaternion(X * f, Y * f, Z * f, W * f);
    }

    inline Quaternion Quaternion::operator / (Float32 f) const
    {
        return Quaternion(X / f, Y / f, Z / f, W / f);
    }

    inline /*friend*/ Quaternion operator + (Float32 f, const Quaternion& q)
    {
        return Quaternion(f + q.X, f + q.Y, f + q.Z, f + q.W);
    }

    inline /*friend*/ Quaternion operator - (Float32 f, const Quaternion& q)
    {
        return Quaternion(f - q.X, f - q.Y, f - q.Z, f - q.W);
    }

    inline /*friend*/ Quaternion operator * (Float32 f, const Quaternion& q)
    {
        return Quaternion(f * q.X, f * q.Y, f * q.Z, f * q.W);
    }

    inline /*friend*/ Quaternion operator / (Float32 f, const Quaternion& q)
    {
        return Quaternion(f / q.X, f / q.Y, f / q.Z, f / q.W);
    }

    inline Float32 Quaternion::operator | (const Quaternion& q) const
    {
        return Dot(q);
    }

#pragma endregion

#pragma region Utilities

    inline void Quaternion::Get(Float32& x, Float32& y, Float32& z, Float32& w) const
    {
        x = X;
        y = Y;
        z = Z;
        w = W;
    }

    inline void Quaternion::Set(Float32 x, Float32 y, Float32 z, Float32 w)
    {
        X = x;
        Y = y;
        Z = z;
        W = w;
    }

    inline Float32 Quaternion::Dot(const Quaternion& q) const
    {
        return X * q.X +  Y * q.Y + Z * q.Z + W * q.W;
    }

    inline Float32 Quaternion::GetLength() const
    {
        return Sqrt(GetSquareLength());
    }

    inline Float32 Quaternion::GetSquareLength() const
    {
        return X * X + Y * Y + Z * Z + W * W;
    }

    inline Quaternion& Quaternion::Normalize()
    {
        Float32 l = GetLength();
        if (!BM::IsNearlyZero(l))
        {
            X /= l;
            Y /= l;
            Z /= l;
            W /= l;
        }
        return *this;
    }

    inline Quaternion Quaternion::GetNormalized() const
    {
        return Quaternion(*this).Normalize();
    }

    inline Quaternion& Quaternion::Clamp(const Quaternion& min, const Quaternion& max)
    {
        X = BM::Clamp(X, min.X, max.X);
        Y = BM::Clamp(Y, min.Y, max.Y);
        Z = BM::Clamp(Z, min.Z, max.Z);
        W = BM::Clamp(W, min.W, max.W);
        return *this;
    }

    inline Quaternion Quaternion::GetClamped(const Quaternion& min, const Quaternion& max) const
    {
        return Quaternion(*this).Clamp(min, max);
    }

    inline Quaternion& Quaternion::Saturate()
    {
        X = BM::Clamp(X, 0.0f, 1.0f);
        Y = BM::Clamp(Y, 0.0f, 1.0f);
        Z = BM::Clamp(Z, 0.0f, 1.0f);
        W = BM::Clamp(W, 0.0f, 1.0f);
        return *this;
    }

    inline Quaternion Quaternion::GetSaturated() const
    {
        return Quaternion(*this).Saturate();
    }

    inline Quaternion& Quaternion::Min(const Quaternion& q)
    {
        X = BM::Min(X, q.X);
        Y = BM::Min(Y, q.Y);
        Z = BM::Min(Z, q.Z);
        W = BM::Min(W, q.W);
        return *this;
    }

    inline Quaternion Quaternion::GetMin(const Quaternion& q) const
    {
        return Quaternion(*this).Min(q);
    }

    inline Quaternion& Quaternion::Max(const Quaternion& q)
    {
        X = BM::Max(X, q.X);
        Y = BM::Max(Y, q.Y);
        Z = BM::Max(Z, q.Z);
        W = BM::Max(W, q.W);
        return *this;
    }

    inline Quaternion Quaternion::GetMax(const Quaternion& q) const
    {
        return Quaternion(*this).Max(q);
    }

    inline bool Quaternion::IsNearlyEqual(const Quaternion& q, Float32 epsilon) const
    {
        return (BM::IsNearlyEqual(X, q.X, epsilon) && BM::IsNearlyEqual(Y, q.Y, epsilon) && BM::IsNearlyEqual(Z, q.Z, epsilon));
    }

    inline bool Quaternion::IsZero() const
    {
        return (*this == Zero);
    }

    inline bool Quaternion::IsNearlyZero() const
    {
        return IsNearlyEqual(Zero);
    }

    inline bool Quaternion::IsLengthZero() const
    {
        return (GetSquareLength() == 0.0f);
    }

    inline bool Quaternion::IsLengthNearlyZero() const
    {
        return (GetSquareLength() < Sqr(Math32::Epsilon));
    }

    inline Quaternion& Quaternion::Inverse() 
    {
        Conjugate();
        *this /= GetSquareLength();
        return *this;
    }

    inline Quaternion Quaternion::GetInversed() const 
    {
        return Quaternion(*this).Inverse();
    }

    inline Quaternion& Quaternion::Conjugate() 
    {
        X = -X;
        Y = -Y;
        Z = -Z;
        return *this;
    }

    inline Quaternion Quaternion::GetConjugated() const 
    {
        return Quaternion(-X, -Y, -Z, W);
    }

    inline void Quaternion::FromAngleAxis(const Vector3& vAxis, Float32 fAngle) 
    {
        Float32 l = vAxis.GetLength();
        if (BM::IsNearlyZero(l))
        {
            X = 0.0f;
            Y = 0.0f;
            Z = 0.0f;
            W = 1.0f;
        }
        else 
        {
            Float32 fSin = Sin(fAngle * 0.5f);
            Float32 fCos = Cos(fAngle * 0.5f);
            l = fSin / l;
            X = vAxis.X * l;
            Y = vAxis.Y * l;
            Z = vAxis.Z * l;
            W = fCos;
        }
    }

    inline void Quaternion::ToAngleAxis(Vector3& vAxis, Float32& fAngle) const
    {
        fAngle = ACos(W) * 2.0f;
        if (IsNearlyEqual(Identity)) 
        {
            vAxis.X = 0.0f; 
            vAxis.Y = 0.0f; 
            vAxis.Z = 1.0f; 
        }
        else
        {
            vAxis.X = X;
            vAxis.Y = Y; 
            vAxis.Z = Z;
            vAxis.Normalize();
        }
    }

#pragma endregion
}