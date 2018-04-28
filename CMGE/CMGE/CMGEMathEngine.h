#pragma once
#include<math.h>
#include <assert.h>
#include<algorithm>
#include<time.h>
#include<iostream>
using namespace std;

namespace CMGEMathEngine 
{
	typedef float(*Function)(float);

	class MathFunc
	{
	public:
		/*
		角度转弧度
		*/
		inline static float radians(const float deg)
		{
			return ((float)3.1415926 / 180.f) * deg;
		}
		/*
		弧度转角度
		*/
		inline static float degrees(const float rad)
		{
			return (180.f / (float)3.1415926) * rad;
		}
		/*
		生成 x1 x2 之间的随机数
		*/
		inline static float random(float x1, float x2)
		{
			srand((unsigned int)time(0));
			float f1 = random(15000000) / (float)(15000000);
			return Lerp(x1, x2, f1);
		}

		static long random(long n)//产生0~n-1之间的等概率随机数
		{
			srand((unsigned int)time(0));
			long t = 0;
			if (n <= RAND_MAX)
			{
				long r = RAND_MAX - (RAND_MAX + 1) % n;//尾数
				t = rand();
				while (t > r)
				{
					t = rand();
				}
				return t % n;
			}
			else
			{
				long r = n % (RAND_MAX + 1);//余数
				if (happened((double)r / n))//取到余数的概率
				{
					return n - r + random(r);
				}
				else
				{
					return rand() + random(n / (RAND_MAX + 1))*(RAND_MAX + 1);
				}
			}
		}
		/*
		在 a 和 b之间插值
		*/
		inline static float Lerp(float a, float b, float f)
		{
			return a*(1 - f) + b*f;
		}

		static float MCIntegral(float a, float b, Function f, float max, int count)
		{
			float count1 = 0;
			for (int i = 0; i < count; i++)
			{
				float r1 = random(a, b);
				float r2 = random(0, max);
				float f1 = f(r1);
				if (r2 < f1)
					count1++;
			}
			return (b - a)*max*((float)count1 / (float)count);
		}


	private:
		static bool happened(double probability)//probability 0~1
		{
			double MinProb = 1.0 / (RAND_MAX + 1);
			if (probability <= 0)
			{
				return false;
			}
			if (probability<MinProb)
			{
				return rand() == 0 && happened(probability*(RAND_MAX + 1));
			}
			if (rand() <= probability*(RAND_MAX + 1))
			{
				return true;
			}
			return false;
		}
	};

	/*
	四维向量
	*/
	class Vector4 
	{
	public:
		float x, y, z, w;
		Vector4() {}
		Vector4(float r, float g, float b, float a) { x = r; y = g; z = b; w = a; }
		inline Vector4 operator+(const Vector4& v2)const
		{
			Vector4 v3(x + v2.x, y + v2.y, z + v2.z, w + v2.w);
			return v3;
		}
		inline Vector4 operator*(const float& v2)const
		{
			Vector4 v3(x*v2, y*v2, z*v2, w*v2);
			return v3;
		}

		inline void output() 
		{
			cout << x << " " << y << " " << z << " " << w << endl;
		}
	};
	class Vector3
	{
	public:
		float x, y, z;
		Vector3() { x = 0.0f; y = 0.0f; z = 0.0f; };
		Vector3(float x, float y, float z) { this->x = x; this->y = y; this->z = z; };
		~Vector3() {};

		inline static float Dot(Vector3 v1, Vector3 v2)
		{
			return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
		}

		inline static float Distance(Vector3 v1, Vector3 v2)
		{
			Vector3 v = v1 - v2;
			return powf(v.x*v.x + v.y*v.y + v.z*v.z, 0.5f);
		}

		inline static Vector3 Lerp(Vector3 v1, Vector3 v2, float f1)
		{
			return v1*(1 - f1) + v2*f1;
		}

		inline static Vector3 Cross(Vector3 v1,Vector3 v2) 
		{
			Vector3 v3(v1.y*v2.z-v1.z*v2.y,v1.z*v2.x-v2.z*v1.x,v1.x*v2.y-v2.x*v1.y);
			return v3;
		}

		inline static Vector3 randomTangent(Vector3 v) 
		{
			float randomX = MathFunc::random(-1,1);
			float randomY = MathFunc::random(-1,1);
			float randomZ = -(v.x*randomX + v.y*randomY) / v.z;
			Vector3 res = Vector3(randomX, randomY, randomZ);
			res.normalized();
			return res;
		}

		inline static Vector3 Slerp(Vector3 v1, Vector3 v2, float f1)
		{
			Vector3 v3(v1.x, v1.y,v1.z);
			Vector3 v4(v2.x, v2.y,v2.z);
			v1.normalized();
			v2.normalized();
			double dot = Dot(v2, v1);
			if (dot < 0.0f)
			{
				v1 = Vector3(0, 0, 0) - v1;
				dot = -dot;
			}
			const double DOT_THRESHOLD = 0.9995;
			if (dot > DOT_THRESHOLD)
			{

				Vector3 result = v1 + (v2 - v1)*f1;
				return result;
			}
			float theta_0 = acos(dot);
			float theta = theta_0*f1;
			float s0 = cos(theta) - dot * sin(theta) / sin(theta_0);
			float s1 = sin(theta) / sin(theta_0);
			return (v3*s0) + (v4*s1);
		}

		inline float magnitude()
		{
			return Distance(Vector3(x, y, z), Vector3());
		}

		inline void normalized()
		{
			float m = magnitude();
			x /= m;
			y /= m;
			z /= m;
		}

		inline void operator = (const Vector3& v)
		{
			x = v.x;
			y = v.y;
			z = v.z;
		}

		inline void operator+=(const Vector3& v1)
		{
			x += v1.x;
			y += v1.y;
			z += v1.z;
		}

		inline Vector3 operator-(const Vector3& v1)const
		{
			Vector3 v2(x - v1.x, y - v1.y, z - v1.z);
			return v2;
		}

		inline Vector3 operator*(const float& f1)const
		{
			Vector3  v2(x*f1, y*f1, z*f1);
			return v2;
		}

		inline Vector3 operator*(const Vector3& f1)const
		{
			Vector3  v2(x*f1.x,y*f1.y,z*f1.z);
			return v2;
		}

		inline Vector3 operator+(const Vector3& v2)const
		{
			Vector3 v3(x + v2.x, y + v2.y, z + v2.z);
			return v3;
		}

		inline Vector3 operator-()const
		{
			Vector3 v3(-x,-y,-z);
			return v3;
		}

		inline void output()
		{
			cout << x << " " << y << " " << z<< endl;
		}
	};
	class  Vector2
	{
	public:
		float x, y;
		Vector2() { x = 0.0f; y = 0.0f; };
		Vector2(float x, float y) { this->x = x; this->y = y; };
		~Vector2() {};
		inline static float Dot(Vector2 v1, Vector2 v2)
		{
			return v1.x*v2.x + v1.y*v2.y;
		}
		inline static float Distance(Vector2 v1, Vector2 v2)
		{
			return powf((v1 - v2).x*(v1 - v2).x + (v1 - v2).y*(v1 - v2).y, 0.5f);
		}
		inline static Vector2 Lerp(Vector2 v1, Vector2 v2, float f1)
		{
			return v1*(1 - f1) + v2*f1;
		}

		inline static Vector2 Slerp(Vector2 v1, Vector2 v2, float f1)
		{
			Vector2 v3(v1.x, v1.y);
			Vector2 v4(v2.x,v2.y);
			v1.normalized();
			v2.normalized();
			double dot = Dot(v2, v1);
			if (dot < 0.0f)
			{
				v1 = Vector2(0, 0) - v1;
				dot = -dot;
			}
			const double DOT_THRESHOLD = 0.9995;
			if (dot > DOT_THRESHOLD)
			{

				Vector2 result = v1 + (v2 - v1)*f1;
				result.normalized();
				return result;
			}
			float theta_0 = acos(dot);
			float theta = theta_0*f1;
			float s0 = cos(theta) - dot * sin(theta) / sin(theta_0);
			float s1 = sin(theta) / sin(theta_0);
			return (v3*s0) + (v4*s1);
		}
		inline static Vector2 GenerateSpherePoint(float r, Vector2 center)
		{
			float x = 0.0f, y = 0.0f;
			int MAX_COUNT = 4;
			int TEMP_COUNT = 0;
			while (TEMP_COUNT<MAX_COUNT && !IsPointInSpere(center, Vector2(x, y), r))
			{
				x = MathFunc::random(0, r);
				y = MathFunc::random(0, r);
				TEMP_COUNT++;
			}
			if (TEMP_COUNT == MAX_COUNT)
			{
				return Vector2(0, r);
			}
			else
			{
				return Vector2(x, y);
			}
		}

		inline static bool IsPointInSpere(Vector2 center, Vector2 Point, float r)
		{
			if (Vector2::Distance(center, Point) <= r)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		inline float magnitude()
		{
			return Distance(Vector2(x, y), Vector2());
		}

		inline void normalized()
		{
			float m = magnitude();
			x /= m;
			y /= m;
		}


		inline Vector2 operator-(const Vector2& v2)const
		{
			Vector2 v3;
			v3.x =x- v2.x;
			v3.y =y- v2.y;
			return v3;
		}
		inline Vector2 operator*(const float& f1)const
		{
			Vector2  v2(x*f1,y*f1);
			return v2;
		}
		inline Vector2 operator*(const Vector2& f1)const 
		{
			Vector2  v3(x*f1.x, y*f1.y);
			return v3;
		}
		inline Vector2 operator+(const Vector2& v2)const
		{
			Vector2 v3(x + v2.x, y + v2.y);
			return v3;
		}
		inline void output()
		{
			cout << x << " " << y<<endl;
		}
	};


	class Matrix4x4
	{
	public:
		Matrix4x4 operator*(const Matrix4x4& m2)const
		{
			Matrix4x4 m3;
			float * d = new float[16];
			float * d1 = data;
			float * d2 = m2.data;
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					d[i * 4 + j] = d1[i * 4] * d2[j] + d1[i * 4 + 1] * d2[4 + j] + d1[i * 4 + 2] * d2[8 + j] + d1[i * 4 + 3] * d2[12 + j];
			m3.setData(d);
			return m3;
		}
		inline static Matrix4x4 TranslateMatrix(Vector3 v)
		{
			float * d = new float[16];
			d[0] = 1.0f; d[1] = 0.0f; d[2] = 0.0f; d[3] = v.x;
			d[4] = 0.0f; d[5] = 1.0f; d[6] = 0.0f; d[7] = v.y;
			d[8] = 0.0f; d[9] = 0.0f; d[10] = 1.0f; d[11] = v.z;
			d[12] = 0.0f; d[13] = 0.0f; d[14] = 0.0f; d[15] = 1.0f;
			Matrix4x4 m;
			m.setData(d);
			return m;
		}
		inline static Matrix4x4 ScaleMatrix(Vector3 v)
		{
			float * d = new float[16];
			d[0] = v.x; d[1] = 0.0f; d[2] = 0.0f; d[3] = 0.0f;
			d[4] = 0.0f; d[5] = v.y; d[6] = 0.0f; d[7] = 0.0f;
			d[8] = 0.0f; d[9] = 0.0f; d[10] = v.z; d[11] = 0.0f;
			d[12] = 0.0f; d[13] = 0.0f; d[14] = 0.0f; d[15] = 1.0f;
			Matrix4x4 m;
			m.setData(d);
			return m;
		}
		inline static Matrix4x4 RotateXMatrix(float x) 
		{
			float sin_t = sinf(MathFunc::radians(x));
			float cos_t = cosf(MathFunc::radians(x));
			float * d = new float[16];
			d[0] = 1.0f; d[1] = 0.0f; d[2] = 0.0f; d[3] = 0.0f;
			d[4] = 0.0f; d[5] = cos_t; d[6] = -sin_t; d[7] = 0.0f;
			d[8] = 0.0f; d[9] = sin_t; d[10] = cos_t; d[11] = 0.0f;
			d[12] = 0.0f; d[13] = 0.0f; d[14] = 0.0f; d[15] = 1.0f;
			Matrix4x4 m;
			m.setData(d);
			return m;
		}
		inline static Matrix4x4 RotateYMatrix(float x)
		{
			float sin_t = sinf(MathFunc::radians(x));
			float cos_t = cosf(MathFunc::radians(x));
			float * d = new float[16];
			d[0] = cos_t; d[1] = 0.0f; d[2] = sin_t; d[3] = 0.0f;
			d[4] = 0.0f; d[5] = 1.0f; d[6] = 0.0f; d[7] = 0.0f;
			d[8] = -sin_t; d[9] = 0.0f; d[10] = cos_t; d[11] = 0.0f;
			d[12] = 0.0f; d[13] = 0.0f; d[14] = 0.0f; d[15] = 1.0f;
			Matrix4x4 m;
			m.setData(d);
			return m;
		}
		inline static Matrix4x4 RotateZMatrix(float x)
		{
			float sin_t = sinf(MathFunc::radians(x));
			float cos_t = cosf(MathFunc::radians(x));
			float * d = new float[16];
			d[0] = cos_t; d[1] = -sin_t; d[2] = 0.0f; d[3] = 0.0f;
			d[4] = sin_t; d[5] = cos_t; d[6] = 0.0f; d[7] = 0.0f;
			d[8] = 0.0f; d[9] = 0.0f; d[10] = 1.0f; d[11] = 0.0f;
			d[12] = 0.0f; d[13] = 0.0f; d[14] = 0.0f; d[15] = 1.0f;
			Matrix4x4 m;
			m.setData(d);
			return m;
		}

		inline static Matrix4x4 RotateMatrix(Vector3 v) 
		{
			Matrix4x4 mX = RotateXMatrix(v.x);
			Matrix4x4 mY = RotateYMatrix(v.y);
			Matrix4x4 mZ = RotateZMatrix(v.z);
			Matrix4x4 m = mX*mZ*mY;
		//	m.transpose();
			return m;
		}

		inline static Matrix4x4 Rotate(float angle,float x,float y,float z) 
		{
			Matrix4x4 m;
			float x2 = x * x;
			float y2 = y * y;
			float z2 = z * z;
			float rads = float(angle) * 0.0174532925f;
			float c = cosf(rads);
			float s = sinf(rads);
			float omc = 1.0f - c;
			float * f = new float[16];
			f[0] = x2*omc + c; f[1] = y*x*omc + z*s; f[2] = x*z*omc - y*s; f[3] = 0;
			f[4] = x*y*omc - z*s; f[5] = y2*omc + c; f[6] = y*z*omc + x*s; f[7] = 0;
			f[8] = x*z*omc + y*s; f[9] = y*z*omc - x*s; f[10] = z2*omc + c; f[11] = 0;
			f[12] = 0.0f; f[13] = 0.0f; f[14] = 0.0f; f[15] = 1.0f;
			m.setData(f);
			return m;
		}

		inline static Matrix4x4 LookAt(Vector3 eye,Vector3 at,Vector3 up) 
		{
			Vector3 f = at - eye;
			f.normalized();
			Vector3 upN(up.x,up.y,up.z);
			upN.normalized();
			Vector3 s = Vector3::Cross(upN,f);
			Vector3 u = Vector3::Cross(f,s);
			Matrix4x4 m;
			float * d = new float[16];
			d[0] = s.x; d[1] = u.x; d[2] = -f.x; d[3] = 0.0f;
			d[4] = s.y; d[5] = u.y; d[6] = -f.y; d[7] = 0.0f;
			d[8] = s.z; d[9] = u.z; d[10] = -f.z; d[11] = 0.0f;
			d[12] = 0.0f; d[13] = 0.0f; d[14] = 0.0f; d[15] = 1.0f;
			m.setData(d);
			m.transpose();
		//	m.output();
		//	Matrix4x4::TranslateMatrix(Vector3(-eye.x, -eye.y, -eye.z)).output();
			Matrix4x4 m1 = m*Matrix4x4::TranslateMatrix(Vector3(-eye.x, -eye.y, -eye.z));
			//m1.output();
			return m1;
		}
		inline static Matrix4x4 Frustum(float left,float right,float bottom,float top,float n,float f) 
		{
			Matrix4x4 result = Matrix4x4::Identity();
			if ((right == left) ||
				(top == bottom) ||
				(n == f) ||
				(n < 0.0) ||
				(f < 0.0))
				return result;
			result.setData(0,0,(2.0f * n) / (right - left));
			result.setData(1,1,(2.0f * n) / (top - bottom));
			result.setData(2,0,(right + left) / (right - left));
			result.setData(2,1,(top + bottom) / (top - bottom));
			result.setData(2,2,-(f + n) / (f - n));
			result.setData(2,3,-1.0f);
			result.setData(3,2,-(2.0f * f * n) / (f - n));
			result.setData(3,3,0.0f);
			result.transpose();
			return result;
		}
		inline static Matrix4x4 Identity() 
		{
			Matrix4x4 m;
			float *f = new float[16];
			f[0] = 1.0f; f[1] = 0.0f; f[2] = 0.0f; f[3] = 0.0f;
			f[4] = 0.0f; f[5] = 1.0f; f[6] = 0.0f; f[7] = 0.0f;
			f[8] = 0.0f; f[9] = 0.0f; f[10] = 1.0f; f[11] = 0.0f;
			f[12] = 0.0f; f[13] = 0.0f; f[14] = 0.0f; f[15] = 1.0f;
			m.setData(f);
			return m;
		}
		inline static Matrix4x4 Ortho(float left, float right, float bottom, float top, float n, float f_)
		{
			Matrix4x4 m;
			float * f = new float[16];
			f[0] = 2.0f / (right - left); f[1] = 0.0f; f[2] = 0.0f; f[3] = 0.0f;
			f[4] = 0.0f; f[5] = 2.0f / (top - bottom); f[6] = 0.0f; f[7] = 0.0f;
			f[8] = 0.0f; f[9] = 0.0f; f[10] = 2.0f / (n - f_); f[11] = 0.0f;
			f[12] = (left + right) / (left - right); f[13] = (bottom + top) / (bottom - top); f[14] = (n + f_) / (n - f_); f[15] = 1.0f;
			m.setData(f);
			m.transpose();
			return m;
		}
		Vector4 operator*(const Vector4& v)
		{
			Vector4 v1;
			v1.x = v.x*data[0] + v.y*data[1] + v.z*data[2] + v.w*data[3];
			v1.y = v.x*data[4] + v.y*data[5] + v.z*data[6] + v.w*data[7];
			v1.z = v.x*data[8] + v.y*data[9] + v.z*data[10] + v.w*data[11];
			v1.w = v.x*data[12] + v.y*data[13] + v.z*data[14] + v.w*data[15];
			return v1;
		}
		void setData(int x, int y, float d) 
		{
			data[x * 4 + y] = d;
		}
		void setData(float *d) 
		{
			delete[] data;
			data = d;
		}
		void transpose() 
		{
			float *data1 = new float[16];
			data1[0] = data[0]; data1[4] = data[1]; data1[8] = data[2]; data1[12] = data[3];
			data1[1] = data[4]; data1[5] = data[5]; data1[9] = data[6]; data1[13] = data[7];
			data1[2] = data[8]; data1[6] = data[9]; data1[10] = data[10]; data1[14] = data[11];
			data1[3] = data[12]; data1[7] = data[13]; data1[11] = data[14]; data1[15] = data[15];
			setData(data1);
		}
		Matrix4x4 inverse() 
		{
			Matrix4x4 minv = Matrix4x4();
			float * f1 = new float[16];
			for (int i = 0; i < 16; i++)
				f1[i] = data[i];
			minv.setData(f1);

			int indxc[4], indxr[4];
			int ipiv[4] = { 0, 0, 0, 0 };
			for (int i = 0; i < 4; i++) 
			{
				int irow = -1, icol = -1;
				float big = 0.f;
				for (int j = 0; j < 4; j++) 
				{
					if (ipiv[j] != 1) 
					{
						for (int k = 0; k < 4; k++) 
						{
							if (ipiv[k] == 0) 
							{
								if (fabsf(minv.data[j*4+k]) >= big)
								{
									big = abs(minv.data[j*4+k]);
									irow = j;
									icol = k;
								}
							}
						}
					}
				}

				assert(irow >= 0 && irow < 4);
				assert(icol >= 0 && icol < 4);

				++ipiv[icol];
				// Swap rows _irow_ and _icol_ for pivot
				if (irow != icol)
				{
					for (int k = 0; k < 4; ++k)
						swap(minv.data[irow*4+k], minv.data[icol*4+k]);
				}

				indxr[i] = irow;
				indxc[i] = icol;

				float pivinv = 1.f / minv.data[icol*4+icol];
				minv.data[icol*4+icol] = 1.f;
				for (int j = 0; j < 4; j++)
					minv.data[icol*4+j] *= pivinv;

				for (int j = 0; j < 4; j++) 
				{
					if (j != icol) 
					{
						float save = minv.data[j*4+icol];
						minv.data[j*4+icol] = 0;
						for (int k = 0; k < 4; k++)
							minv.data[j*4+k] -= minv.data[icol*4+k] * save;
					}
				}
			}

			for (int j = 3; j >= 0; j--) 
			{
				if (indxr[j] != indxc[j]) 
				{
					for (int k = 0; k < 4; k++)
						swap(minv.data[k*4+indxr[j]], minv.data[k*4+indxc[j]]);
				}
			}

			return minv;
		}
		void output()
		{
			for(int i=0;i<4;i++)
			{
				for(int j=0;j<4;j++)
				{
					cout << data[i * 4 + j] << " ";
				}
				cout << endl;
			}
		}
		float *data = new float[16];
	};


	class Transform
	{
	public:
		Vector3 Position;
		Vector3 Rotation;
		Vector3 Scalition;
		Matrix4x4 PositionMatrix;
		Matrix4x4 RotationMatrix;
		Matrix4x4 ScaleMatrix;
		Vector3 forward() 
		{
			Vector4 v(0.0f,0.0f,1.0f,1.0f);
			Vector4 v1 = RotationMatrix*v;
			return Vector3(v1.x,v1.y,v1.z);
		}
		Vector3 up() 
		{
			Vector4 v(0.0f, 1.0f, 0.0f, 1.0f);
			Vector4 v1 = RotationMatrix*v;
			return Vector3(v1.x, v1.y, v1.z);
		}
		Vector3 right()
		{
			Vector4 v(1.0f, 0.0f, 0.0f, 1.0f);
			Vector4 v1 = RotationMatrix*v;
			return Vector3(v1.x, v1.y, v1.z);
		}
		void Translate(Vector3 v) 
		{
			Position += v;
			PositionMatrix = Matrix4x4::TranslateMatrix(Position);
		}
		void Rotate(Vector3 v) 
		{
			Rotation += v;
			RotationMatrix = Matrix4x4::RotateMatrix(Rotation);
		}
		void Scale(Vector3 v) 
		{
			Scalition = v;
			ScaleMatrix = Matrix4x4::ScaleMatrix(Scalition);
		}

		Matrix4x4 Model() 
		{
			return  PositionMatrix*RotationMatrix*ScaleMatrix;
		}
	};


}