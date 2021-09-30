#pragma once
#include "vector_3d.h"
#include "v_matrix.h"
#include "../sdk.h"

#define TICK_INTERVAL (csgo::globals->interval_per_tick)
#define TICKS_TO_TIME(t) (csgo::globals->interval_per_tick * (t))
#define TIME_TO_TICKS(dt)((int)( 0.5f + (float)(dt) / TICK_INTERVAL))


namespace math {

    inline float pi = 3.141592653589793f;

    void vector_transform(const vec3_t& in1, const matrix3x4_t& in2, vec3_t& out);
    static bool screen_transform(const vec3_t& in, vec3_t& out);
    bool w2s(const vec3_t& in, vec3_t& out);
    void vector_angles(const vec3_t& forward, vec3_t& angles);
	void fix_angles(vec3_t* angles);

    float deg2rad(float deg);
    float rad2deg(float rad);
    void SinCos(float x, float* s, float* c);
    void SinCos(float x, float& s, float& c);
    void AngleVectors(const vec3_t& angles, vec3_t* forward);
    void AngleVectors(const vec3_t& angles, vec3_t* forward, vec3_t* right, vec3_t* up);
    void VectorAngles(const vec3_t& forward, vec3_t& angles);
    float VectorNormalize(vec3_t& v);
    void AngleNormalize(vec3_t& v);
    void ClampAngles(vec3_t& v);
    void VectorTransform(const vec3_t& vSome, const matrix3x4_t& vMatrix, vec3_t& vOut);
    bool IsVisible(vec3_t& vecAbsStart, vec3_t& vecAbsEnd, i_client_entity* pLocal, i_client_entity* pBaseEnt);
    vec3_t DirectionToAngle(vec3_t direction);
    float Distance(const vec3_t& a, const vec3_t& b);
    float point_to_line(vec3_t from, vec3_t to, vec3_t dir);
    float GetFOV(const vec3_t& _from, const vec3_t& _to);
    vec3_t CalculateAimAngle(vec3_t vLocal, vec3_t vTarget);
    void CalcAngle(const vec3_t& src, const vec3_t& dst, vec3_t& angle);
    vec3_t CalcAngle(const vec3_t& src, const vec3_t& dst);
    float AngleDelta(const vec3_t& viewAngle, const vec3_t& tgtAngle);
    float Distance2d(const vec3_t& a, const vec3_t& b);
    float Distance3d(const vec3_t& a, const vec3_t& b);
}