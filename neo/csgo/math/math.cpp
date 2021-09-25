#include "math.h"
#include <d3d9.h>
#include "../globals.h"
#include <DirectXMath.h>

namespace math {
    void vector_transform(const vec3_t& in1, const matrix3x4_t& in2, vec3_t& out)
    {
        out[0] = in1.dot(in2[0]) + in2[0][3];
        out[1] = in1.dot(in2[1]) + in2[1][3];
        out[2] = in1.dot(in2[2]) + in2[2][3];
    }
	bool screen_transform(const vec3_t& in, vec3_t& out)
	{
		auto exception_filter = [](int code, PEXCEPTION_POINTERS ex)
		{
			return EXCEPTION_EXECUTE_HANDLER;
		};

		__try
		{
			auto result = *(PDWORD)(g::v_matrix::offset) + 988;
			if (!result)
			{
				out.x *= 100000;
				out.y *= 100000;
				return false;
			}

			const auto& world_matrix = *(_D3DMATRIX*)result;

			const auto w = world_matrix.m[3][0] * in.x + world_matrix.m[3][1] * in.y + world_matrix.m[3][2] * in.z + world_matrix.m[3][3];
			if (w < 0.001f)
			{
				out.x *= 100000;
				out.y *= 100000;
				return false;
			}

			out.x = world_matrix.m[0][0] * in.x + world_matrix.m[0][1] * in.y + world_matrix.m[0][2] * in.z + world_matrix.m[0][3];
			out.y = world_matrix.m[1][0] * in.x + world_matrix.m[1][1] * in.y + world_matrix.m[1][2] * in.z + world_matrix.m[1][3];
			out.z = 0.0f;

			out.x /= w;
			out.y /= w;

			return true;
		}
		__except (exception_filter(GetExceptionCode(), GetExceptionInformation()))
		{
			out.x *= 100000;
			out.y *= 100000;
			return false;
		}
	}

	bool w2s(const vec3_t& in, vec3_t& out)
	{
		if (!g::v_matrix::has_offset)
			return false;

		if (!screen_transform(in, out))
			return false;

		int w, h;
		csgo::engine->screen_size(w, h);

		out.x = (w / 2.0f) + (out.x * w) / 2.0f;
		out.y = (h / 2.0f) - (out.y * h) / 2.0f;

		return true;
	}
	void vector_angles(const vec3_t& forward, vec3_t& angles)
	{
		float	tmp, yaw, pitch;

		if (forward[1] == 0 && forward[0] == 0) {
			yaw = 0;
			if (forward[2] > 0)
				pitch = 270;
			else
				pitch = 90;
		}
		else {
			yaw = (atan2(forward[1], forward[0]) * 180 / DirectX::XM_PI);
			if (yaw < 0)
				yaw += 360;

			tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
			pitch = (atan2(-forward[2], tmp) * 180 / DirectX::XM_PI);
			if (pitch < 0)
				pitch += 360;
		}

		angles[0] = pitch;
		angles[1] = yaw;
		angles[2] = 0;
	}
	void fix_angles(vec3_t* angles) {
		angles->clamp();
		//angles->normalized();
	}

	//float Pi = 3.141592653589793;
	float deg2rad(float deg)
	{
		return (deg * pi) / 180;
	}

	float rad2deg(float rad)
	{
		return (pi / 180) * rad;
	}

	void SinCos(float x, float* s, float* c)
	{
		__asm
		{
			fld dword ptr[x]
			fsincos
			mov edx, dword ptr[c]
			mov eax, dword ptr[s]
			fstp dword ptr[edx]
			fstp dword ptr[eax]
		}
	}

	void SinCos(float x, float& s, float& c)
	{
		__asm
		{
			fld dword ptr[x]
			fsincos
			mov edx, dword ptr[c]
			mov eax, dword ptr[s]
			fstp dword ptr[edx]
			fstp dword ptr[eax]
		}
	}

	void AngleVectors(const vec3_t& angles, vec3_t* forward)
	{
		float sp, sy, cp, cy;

		SinCos(deg2rad(angles.x), &sp, &cp);
		SinCos(deg2rad(angles.y), &sy, &cy);

		if (forward)
		{
			forward->x = cp * cy;
			forward->y = cp * sy;
			forward->z = -sp;
		}
	}

	void AngleVectors(const vec3_t& angles, vec3_t* forward, vec3_t* right, vec3_t* up)
	{
		float sr, sp, sy, cr, cp, cy;

		SinCos(deg2rad(angles.x), &sp, &cp);
		SinCos(deg2rad(angles.y), &sy, &cy);
		SinCos(deg2rad(angles.z), &sr, &cr);

		if (forward)
		{
			forward->x = cp * cy;
			forward->y = cp * sy;
			forward->z = -sp;
		}

		if (right)
		{
			right->x = (-1 * sr * sp * cy + -1 * cr * -sy);
			right->y = (-1 * sr * sp * sy + -1 * cr * cy);
			right->z = -1 * sr * cp;
		}

		if (up)
		{
			up->x = (cr * sp * cy + -sr * -sy);
			up->y = (cr * sp * sy + -sr * cy);
			up->z = cr * cp;
		}
	}

	void VectorAngles(const vec3_t& forward, vec3_t& angles)
	{
		float tmp, yaw, pitch;

		if (forward.y == 0 && forward.x == 0)
		{
			yaw = 0;

			if (forward.z > 0)
				pitch = 270;
			else
				pitch = 90;
		}
		else
		{
			yaw = rad2deg(atan2f(forward.y, forward.x));

			if (yaw < 0)
				yaw += 360;

			tmp = forward.length_2d();
			pitch = rad2deg(atan2f(-forward.z, tmp));

			if (pitch < 0)
				pitch += 360;
		}

		angles[0] = pitch;
		angles[1] = yaw;
		angles[2] = 0;
	}

	float VectorNormalize(vec3_t& v)
	{
		float flLength = v.length();

		if (!flLength)
		{
			v.x = 0.0f;
			v.y = 0.0f;
			v.z = 0.0f;
		}

		else
		{
			v /= flLength;
		}

		return flLength;
	}

	void AngleNormalize(vec3_t& v)
	{
		for (auto i = 0; i < 3; i++)
		{
			if (v[i] < -180.0f) v[i] += 360.0f;
			if (v[i] > 180.0f) v[i] -= 360.0f;
		}
	}

	void ClampAngles(vec3_t& v)
	{
		if (v.x > 89.0f && v.x <= 180.0f)
			v.x = 89.0f;

		if (v.x > 180.0f)
			v.x = v.x - 360.0f;

		if (v.x < -89.0f)
			v.x = -89.0f;

		v.y = fmodf(v.y + 180, 360) - 180;

		v.z = 0;
	}

	void VectorTransform(const vec3_t& vSome, const matrix3x4_t& vMatrix, vec3_t& vOut)
	{
		for (auto i = 0; i < 3; i++)
			vOut[i] = vSome.dot((vec3_t)vMatrix[i]) + vMatrix[i][3];
	}

	bool IsVisible(vec3_t& vecAbsStart, vec3_t& vecAbsEnd, i_client_entity* pLocal, i_client_entity* pBaseEnt)
	{
		return csgo::engine->IsOccluded(vecAbsStart, vecAbsEnd);
	}

	vec3_t DirectionToAngle(vec3_t direction)
	{
		float tmp, yaw, pitch;

		if (direction.y == 0 && direction.x == 0)
		{
			yaw = 0;
			if (direction.z > 0)
				pitch = 270;
			else
				pitch = 90;
		}
		else
		{
			yaw = (atan2(direction.y, direction.x) * 180 / pi);
			if (yaw < 0)
				yaw += 360;

			tmp = sqrt(direction.x * direction.x + direction.y * direction.y);
			pitch = (atan2(-direction.z, tmp) * 180 / Pi);
			if (pitch < 0)
				pitch += 360;
		}

		if (pitch > 180.f)
			pitch -= 360.f;

		return vec3_t(pitch, yaw, 0.f);
	}

	float Distance3d(const vec3_t& a, const vec3_t& b)
	{
		float distance = sqrt(powf(a.x - b.x, 2) + powf(a.y - b.y, 2) + powf(a.z - b.z, 2));

		return distance;
	}

	float Distance2d(const vec3_t& a, const vec3_t& b)
	{
		float distance = sqrt(powf(a.x - b.x, 2) + powf(a.y - b.y, 2));

		return distance;
	}

	float point_to_line(vec3_t from, vec3_t to, vec3_t dir)
	{
		auto PointDir = to - from;
		auto TempOffset = PointDir.dot(dir) / dir.length_sqr();
		if (TempOffset < 0.000001f)
			return FLT_MAX;

		auto PerpendicularPoint = from + (dir * TempOffset);

		return (to - PerpendicularPoint).length();
	}

	float GetFOV(const vec3_t& _from, const vec3_t& _to, vec3_t _dir)
	{
		return point_to_line(_from, _to, _dir);
	}

	float GetFOV(const vec3_t& _from, const vec3_t& _to)
	{
		vec3_t from = _from;
		from -= _to;
		from.clamp();
		return (abs(from.y) + abs(from.x));
	}

	vec3_t CalculateAimAngle(vec3_t vLocal, vec3_t vTarget)
	{
		vec3_t out{};

		float dist2d = 
	}
}
