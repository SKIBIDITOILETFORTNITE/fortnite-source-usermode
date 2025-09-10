#pragma once
#include <d3d9.h>
#include <vector>
#include <algorithm>
#define M_PI 3.14159265358979323846264338327950288419716939937510

struct FVector2D {
	FVector2D() : X(), Y() {}
	FVector2D(double x, double y) : X(x), Y(y) {}

	double X, Y;

	operator bool() { return bool(this->X && this->Y); }
	__forceinline FVector2D operator-(const FVector2D& V) {
		return FVector2D(X - V.X, Y - V.Y);
	}

	__forceinline FVector2D operator+(const FVector2D& V) {
		return FVector2D(X + V.X, Y + V.Y);
	}

	__forceinline FVector2D operator*(float Scale) const {
		return FVector2D(X * Scale, Y * Scale);
	}

	__forceinline FVector2D operator/(float Scale) const {
		const float RScale = 1.f / Scale;
		return FVector2D(X * RScale, Y * RScale);
	}

	__forceinline FVector2D operator+(float A) const {
		return FVector2D(X + A, Y + A);
	}

	__forceinline FVector2D operator-(float A) const {
		return FVector2D(X - A, Y - A);
	}

	__forceinline FVector2D operator*(const FVector2D& V) const {
		return FVector2D(X * V.X, Y * V.Y);
	}

	__forceinline FVector2D operator/(const FVector2D& V) const {
		return FVector2D(X / V.X, Y / V.Y);
	}

	__forceinline float operator|(const FVector2D& V) const {
		return X * V.X + Y * V.Y;
	}

	__forceinline float operator^(const FVector2D& V) const {
		return X * V.Y - Y * V.X;
	}

	__forceinline FVector2D& operator+=(const FVector2D& v) {
		(*this);
		(v);
		X += v.X;
		Y += v.Y;
		return *this;
	}

	__forceinline FVector2D& operator-=(const FVector2D& v) {
		(*this);
		(v);
		X -= v.X;
		Y -= v.Y;
		return *this;
	}

	__forceinline FVector2D& operator*=(const FVector2D& v) {
		(*this);
		(v);
		X *= v.X;
		Y *= v.Y;
		return *this;
	}

	__forceinline FVector2D& operator/=(const FVector2D& v) {
		(*this);
		(v);
		X /= v.X;
		Y /= v.Y;
		return *this;
	}

	__forceinline bool operator==(const FVector2D& src) const {
		(src);
		(*this);
		return (src.X == X) && (src.Y == Y);
	}

	__forceinline bool operator!=(const FVector2D& src) const {
		(src);
		(*this);
		return (src.X != X) || (src.Y != Y);
	}
};

struct FVector
{
	using UnderlayingType = double; // 0x0(0x8)

	double X; // 0x0(0x8)
	double Y; // 0x8(0x8)
	double Z; // 0x10(0x8)

	FVector(double InX = 0.0, double InY = 0.0, double InZ = 0.0)
		: X(InX), Y(InY), Z(InZ)
	{
	}

	FVector& Normalize()
	{
		*this /= Magnitude();
		return *this;
	}

	FVector& operator*=(const FVector& Other)
	{
		*this = *this * Other;
		return *this;
	}

	FVector& operator*=(float Scalar)
	{
		*this = *this * Scalar;
		return *this;
	}

	FVector& operator+=(const FVector& Other)
	{
		*this = *this + Other;
		return *this;
	}

	FVector& operator-=(const FVector& Other)
	{
		*this = *this - Other;
		return *this;
	}

	FVector& operator/=(const FVector& Other)
	{
		*this = *this / Other;
		return *this;
	}

	FVector& operator/=(double Scalar)
	{
		*this = *this / Scalar;
		return *this;
	}

	UnderlayingType Dot(const FVector& Other) const
	{
		return (X * Other.X) + (Y * Other.Y) + (Z * Other.Z);
	}

	UnderlayingType GetDistanceTo(const FVector& Other) const
	{
		FVector DiffVector = Other - *this;
		return DiffVector.Magnitude();
	}

	inline double Length() { return math::custom_sqrtf(X * X + Y * Y + Z * Z); }

	UnderlayingType GetDistanceToInMeters(const FVector& Other) const
	{
		return GetDistanceTo(Other) * static_cast<UnderlayingType>(0.01);
	}

	FVector GetNormalized() const
	{
		return *this / Magnitude();
	}

	bool IsZero() const
	{
		return X == 0.0 && Y == 0.0 && Z == 0.0;
	}

	UnderlayingType Magnitude() const
	{
		return math::custom_sqrtf((X * X) + (Y * Y) + (Z * Z));
	}

	bool operator!=(const FVector& Other) const
	{
		return X != Other.X || Y != Other.Y || Z != Other.Z;
	}

	FVector operator*(const FVector& Other) const
	{
		return { X * Other.X, Y * Other.Y, Z * Other.Z };
	}

	FVector operator*(double Scalar) const
	{
		return { X * Scalar, Y * Scalar, Z * Scalar };
	}

	FVector operator+(const FVector& Other) const
	{
		return { X + Other.X, Y + Other.Y, Z + Other.Z };
	}

	FVector operator-(const FVector& Other) const
	{
		return { X - Other.X, Y - Other.Y, Z - Other.Z };
	}

	FVector operator/(const FVector& Other) const
	{
		if (Other.X == 0.0 || Other.Y == 0.0 || Other.Z == 0.0)
			return *this;

		return { X / Other.X, Y / Other.Y, Z / Other.Z };
	}

	FVector operator/(double Scalar) const
	{
		if (Scalar == 0.0)
			return *this;

		return { X / Scalar, Y / Scalar, Z / Scalar };
	}

	bool operator==(const FVector& Other) const
	{
		return X == Other.X && Y == Other.Y && Z == Other.Z;
	}

	inline double distance(FVector v)
	{
		return double(math::custom_sqrtf(crt::c_powf(v.X - X, 2.0) + crt::c_powf(v.Y - Y, 2.0) + crt::c_powf(v.Z - Z, 2.0)));
	}
	inline double distance2D()
	{
		return  math::custom_sqrtf(this->X * this->X + this->Y * this->Y + this->Z * this->Z);
	}
	__forceinline float Size() const {
		return  math::custom_sqrtf(X * X + Y * Y + Z * Z);
	}
};

struct FBounds
{
	float Top, Bottom, Left, Right;
};

struct FQuat { double X, Y, Z, W; };

struct FPlane : FVector
{
	double W;

	FPlane() : W(0) {}
	FPlane(double W) : W(W) {}
};

class FMatrix
{
public:
	double m[4][4];
	FPlane XPlane, YPlane, ZPlane, WPlane;

	FMatrix() : XPlane(), YPlane(), ZPlane(), WPlane() {}
	FMatrix(FPlane XPlane, FPlane YPlane, FPlane ZPlane, FPlane WPlane)
		: XPlane(XPlane), YPlane(YPlane), ZPlane(ZPlane), WPlane(WPlane) {
	}

	D3DMATRIX ToD3DMATRIX() const
	{
		D3DMATRIX Result;
		Result._11 = XPlane.X; Result._12 = XPlane.Y; Result._13 = XPlane.Z; Result._14 = XPlane.W;
		Result._21 = YPlane.X; Result._22 = YPlane.Y; Result._23 = YPlane.Z; Result._24 = YPlane.W;
		Result._31 = ZPlane.X; Result._32 = ZPlane.Y; Result._33 = ZPlane.Z; Result._34 = ZPlane.W;
		Result._41 = WPlane.X; Result._42 = WPlane.Y; Result._43 = WPlane.Z; Result._44 = WPlane.W;
		return Result;
	}
};
struct FTransform final
{
public:
	FQuat   Rotation;
	FVector Translation;
	uint8_t Pad_1C[0x8];
	FVector Scale3D;
	uint8_t Pad_2C[0x8];

public:
	PFORCEINLINE FMatrix ToMatrixWithScale() const
	{
		const FVector Scale
		(
			(Scale3D.X == 0.0) ? 1.0 : Scale3D.X,
			(Scale3D.Y == 0.0) ? 1.0 : Scale3D.Y,
			(Scale3D.Z == 0.0) ? 1.0 : Scale3D.Z
		);

		const double x2 = Rotation.X + Rotation.X;
		const double y2 = Rotation.Y + Rotation.Y;
		const double z2 = Rotation.Z + Rotation.Z;
		const double xx2 = Rotation.X * x2;
		const double yy2 = Rotation.Y * y2;
		const double zz2 = Rotation.Z * z2;
		const double yz2 = Rotation.Y * z2;
		const double wx2 = Rotation.W * x2;
		const double xy2 = Rotation.X * y2;
		const double wz2 = Rotation.W * z2;
		const double xz2 = Rotation.X * z2;
		const double wy2 = Rotation.W * y2;

		FMatrix Matrix = FMatrix();

		Matrix.WPlane.X = Translation.X;
		Matrix.WPlane.Y = Translation.Y;
		Matrix.WPlane.Z = Translation.Z;

		Matrix.XPlane.X = (1.0 - (yy2 + zz2)) * Scale.X;
		Matrix.YPlane.Y = (1.0 - (xx2 + zz2)) * Scale.Y;
		Matrix.ZPlane.Z = (1.0 - (xx2 + yy2)) * Scale.Z;

		Matrix.ZPlane.Y = (yz2 - wx2) * Scale.Z;
		Matrix.YPlane.Z = (yz2 + wx2) * Scale.Y;

		Matrix.YPlane.X = (xy2 - wz2) * Scale.Y;
		Matrix.XPlane.Y = (xy2 + wz2) * Scale.X;

		Matrix.ZPlane.X = (xz2 + wy2) * Scale.Z;
		Matrix.XPlane.Z = (xz2 - wy2) * Scale.X;

		Matrix.XPlane.W = 0.0f;
		Matrix.YPlane.W = 0.0f;
		Matrix.ZPlane.W = 0.0f;
		Matrix.WPlane.W = 1.0f;

		return Matrix;
	}
};

D3DMATRIX MatrixMultiplication(D3DMATRIX pm1, D3DMATRIX pm2)
{
	D3DMATRIX pout{};
	pout._11 = pm1._11 * pm2._11 + pm1._12 * pm2._21 + pm1._13 * pm2._31 + pm1._14 * pm2._41;
	pout._12 = pm1._11 * pm2._12 + pm1._12 * pm2._22 + pm1._13 * pm2._32 + pm1._14 * pm2._42;
	pout._13 = pm1._11 * pm2._13 + pm1._12 * pm2._23 + pm1._13 * pm2._33 + pm1._14 * pm2._43;
	pout._14 = pm1._11 * pm2._14 + pm1._12 * pm2._24 + pm1._13 * pm2._34 + pm1._14 * pm2._44;
	pout._21 = pm1._21 * pm2._11 + pm1._22 * pm2._21 + pm1._23 * pm2._31 + pm1._24 * pm2._41;
	pout._22 = pm1._21 * pm2._12 + pm1._22 * pm2._22 + pm1._23 * pm2._32 + pm1._24 * pm2._42;
	pout._23 = pm1._21 * pm2._13 + pm1._22 * pm2._23 + pm1._23 * pm2._33 + pm1._24 * pm2._43;
	pout._24 = pm1._21 * pm2._14 + pm1._22 * pm2._24 + pm1._23 * pm2._34 + pm1._24 * pm2._44;
	pout._31 = pm1._31 * pm2._11 + pm1._32 * pm2._21 + pm1._33 * pm2._31 + pm1._34 * pm2._41;
	pout._32 = pm1._31 * pm2._12 + pm1._32 * pm2._22 + pm1._33 * pm2._32 + pm1._34 * pm2._42;
	pout._33 = pm1._31 * pm2._13 + pm1._32 * pm2._23 + pm1._33 * pm2._33 + pm1._34 * pm2._43;
	pout._34 = pm1._31 * pm2._14 + pm1._32 * pm2._24 + pm1._33 * pm2._34 + pm1._34 * pm2._44;
	pout._41 = pm1._41 * pm2._11 + pm1._42 * pm2._21 + pm1._43 * pm2._31 + pm1._44 * pm2._41;
	pout._42 = pm1._41 * pm2._12 + pm1._42 * pm2._22 + pm1._43 * pm2._32 + pm1._44 * pm2._42;
	pout._43 = pm1._41 * pm2._13 + pm1._42 * pm2._23 + pm1._43 * pm2._33 + pm1._44 * pm2._43;
	pout._44 = pm1._41 * pm2._14 + pm1._42 * pm2._24 + pm1._43 * pm2._34 + pm1._44 * pm2._44;
	return pout;
}

D3DMATRIX ToMatrix(FVector rot, FVector origin = FVector{ 0, 0, 0 })
{
	float radpitch = (rot.X * M_PI / 180);
	float radyaw = (rot.Y * M_PI / 180);
	float radroll = (rot.Z * M_PI / 180);
	float sp = math::custom_sinf(radpitch);
	float cp = math::custom_cosf(radpitch);
	float sy = math::custom_sinf(radyaw);
	float cy = math::custom_cosf(radyaw);
	float sr = math::custom_sinf(radroll);
	float cr = math::custom_cosf(radroll);
	D3DMATRIX matrix{};
	matrix.m[0][0] = cp * cy;
	matrix.m[0][1] = cp * sy;
	matrix.m[0][2] = sp;
	matrix.m[0][3] = 0.f;
	matrix.m[1][0] = sr * sp * cy - cr * sy;
	matrix.m[1][1] = sr * sp * sy + cr * cy;
	matrix.m[1][2] = -sr * cp;
	matrix.m[1][3] = 0.f;
	matrix.m[2][0] = -(cr * sp * cy + sr * sy);
	matrix.m[2][1] = cy * sr - cr * sp * sy;
	matrix.m[2][2] = cr * cp;
	matrix.m[2][3] = 0.f;
	matrix.m[3][0] = origin.X;
	matrix.m[3][1] = origin.Y;
	matrix.m[3][2] = origin.Z;
	matrix.m[3][3] = 1.f;
	return matrix;
}

inline double RadiansToDegrees(double dRadians)
{
	return dRadians * (180.0 / M_PI);
}

struct Camera
{
	FVector Location;
	FVector Rotation;
	float FOV;
};

Camera LocalCamera;

template< typename t >
class TArray
{
public:
	TArray() : tData(), iCount(), iMaxCount() {}
	TArray(t* data, int count, int max_count) :
		tData(tData), iCount(iCount), iMaxCount(iMaxCount) {
	}

public:
	auto Get(int idx) -> t
	{
		return memory.Read< t >(reinterpret_cast<__int64>(this->tData) + (idx * sizeof(t)));
	}

	auto Size() -> std::uint32_t
	{
		return this->iCount;
	}
	
	bool IsValid()
	{
		return this->iCount != 0;
	}

	t* tData;
	int iCount;
	int iMaxCount;
};

template <typename T>
struct TArray1 {
	std::uintptr_t Array;
	std::uint32_t Count;
	std::uint32_t MaxCount;

	T Get(std::uint32_t Index) const {
		if (Index >= Count) {
			return T();
		}
		return memory.Read<T>(Array + (Index * sizeof(T)));
	}

	T operator[](std::uint32_t Index) const {
		return Get(Index);
	}

	std::uint32_t size() const {
		return Count;
	}

	bool isValid() const {
		return Array && Count <= MaxCount && MaxCount <= 1000000;
	}

	std::uintptr_t getAddress() const {
		return Array;
	}
};