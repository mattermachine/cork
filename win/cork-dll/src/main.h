#pragma once
#ifndef MATHASSISTANT_ARITHMETICS_ARITHMETIC_H
#define MATHASSISTANT_ARITHMETICS_ARITHMETIC_H
#include <cork.h>
#include <string>

#if UNITY_METRO
#define EXPORT_API __declspec(dllexport) __stdcall
#elif UNITY_WIN
#define EXPORT_API __declspec(dllexport)
#else
#define EXPORT_API
#endif

extern "C"
{
	typedef void* HANDLE;

	static CorkTriMesh mesh1;
	static CorkTriMesh mesh2;
	static CorkTriMesh result;
	static std::string errorMessage;

	/*
	// http://stackoverflow.com/questions/134569/c-exception-throwing-stdstring
	struct SolidException : public std::exception
	{
		std::string s;
		SolidException(std::string ss) : s(ss) {}
		~SolidException() throw () {}
		const char* what() const throw() { return s.c_str(); }
	};
	*/

	__declspec(dllexport) void RecycleResult();
	__declspec(dllexport) void ResetMeshes();
	void SolidCheck();

	__declspec(dllexport) const char* GetErrorMessage();

	__declspec(dllexport) bool ComputeUnion(bool doSolidCheck);
	__declspec(dllexport) bool ComputeIntersection(bool doSolidCheck);
	__declspec(dllexport) bool ComputeDifference(bool doSolidCheck);
	__declspec(dllexport) bool ComputeSymmetricDifference(bool doSolidCheck);

	__declspec(dllexport) uint GetNbVertices();
	__declspec(dllexport) uint GetNbFaces();
	__declspec(dllexport) __int32* GetFaces(uint* pSize);
	__declspec(dllexport) float* GetVertices(uint* pSize);
	__declspec(dllexport) void FreeIntList(__int32* list);
	__declspec(dllexport) void FreeFloatList(float* list);

	__declspec(dllexport) bool CreateTriMesh(float vertices[], uint n_vertices, uint faces[], uint n_faces, int target);
}

#endif
