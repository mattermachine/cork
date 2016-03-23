// Win32Project1.cpp : Defines the exported functions for the DLL application.
//

#include "main.h"
#include "cork.h"
#include "prelude.h"
#include <stdexcept>
#include <string>

uint GetNbVertices()
{
	return result.n_vertices;
}

uint GetNbFaces()
{
	return result.n_triangles;
}

float* GetVertices(uint* pSize)
{
	unsigned int n_float = result.n_vertices * 3;
	float* list = (float*)malloc(n_float * sizeof(float));
	for (unsigned int i = 0; i < n_float; i++)
	{
		list[i] = result.vertices[i];
	}
	*pSize = n_float;
	return list;
}

__int32* GetFaces(uint* pSize)
{
	unsigned int n_int = result.n_triangles * 3;
	__int32* list = (__int32*)malloc(n_int * sizeof(__int32));
	for (unsigned int i = 0; i < n_int; i++)
	{
		list[i] = result.triangles[i];
	}
	*pSize = n_int;

	return list;
}

void FreeIntList(__int32* list)
{
	free(list);
}

void FreeFloatList(float* list)
{
	free(list);
}

bool CreateTriMesh(float vertices[], uint n_vertices, uint faces[], uint n_faces, int target)
{
	if (target == 1) {
		mesh1 = {
			n_faces,
			n_vertices,
			faces,
			vertices
		};
		return true;
	}
	if (target == 2) {
		mesh2 = {
			n_faces,
			n_vertices,
			faces,
			vertices
		};
		return true;
	}
	return false;
}

bool ComputeUnion(bool doSolidCheck)
{
	errorMessage = "";
	result = {};

	try
	{
		if (doSolidCheck)
			SolidCheck();
		computeUnion(mesh1, mesh2, &result);
		return true;
	}
	catch (const std::exception& ex)
	{
		errorMessage = "union failed: " + std::string(ex.what());
	}
	catch (const char* ex)
	{
		errorMessage = "union failed: " + std::string(ex);
	}
	catch (...)
	{
		errorMessage = "union failed: unknown error";
	}
	return false;
}

bool ComputeIntersection(bool doSolidCheck)
{
	errorMessage = "";
	result = {};

	try
	{
		if (doSolidCheck)
			SolidCheck();
		computeIntersection(mesh1, mesh2, &result);
		return true;
	}
	catch (const std::exception& ex)
	{
		errorMessage = "intersect failed: " + std::string(ex.what());
	}
	catch (const char* ex)
	{
		errorMessage = "intersect failed: " + std::string(ex);
	}
	catch (...)
	{
		errorMessage = "intersect failed: unknown error";
	}
	return false;
}


bool ComputeDifference(bool doSolidCheck)
{
	errorMessage = "";
	result = {};

	try
	{
		if (doSolidCheck)
			SolidCheck();
		computeDifference(mesh1, mesh2, &result);
		return true;
	}
	catch (const std::exception& ex)
	{
		errorMessage = "subtract failed: " + std::string(ex.what());
	}
	catch (const char* ex)
	{
		errorMessage = "subtract failed: " + std::string(ex);
	}
	catch (...)
	{
		errorMessage = "subtract failed: unknown error";
	}
	return false;
}


bool ComputeSymmetricDifference(bool doSolidCheck)
{
	errorMessage = "";
	result = {};

	try
	{
		if (doSolidCheck)
			SolidCheck();
		computeSymmetricDifference(mesh1, mesh2, &result);
		return true;
	}
	catch (const std::exception& ex)
	{
		errorMessage = "symmetric difference failed: " + std::string(ex.what());
	}
	catch (const char* ex)
	{
		errorMessage = "symmetric difference failed: " + std::string(ex);
	}
	catch (...)
	{
		errorMessage = "symmetric difference failed: unknown error";
	}
	return false;
}

void SolidCheck() {
	try {
		isSolid(mesh1);
	}
	catch (int e) {
		if (e == 1) {
			CORK_ERROR("isSolid() was given a self-intersecting mesh");
			throw "first mesh is a self-intersecting mesh.";
		}
		else {
			CORK_ERROR("isSolid() was given a non-closed mesh");
			throw "first mesh is a non-closed mesh";
		}
	}
	try {
		isSolid(mesh2);
	}
	catch (int e) {
		if (e == 1) {
			CORK_ERROR("isSolid() was given a self-intersecting mesh");
			throw "second mesh is a self-intersecting mesh.";
		}
		else {
			CORK_ERROR("isSolid() was given a non-closed mesh");
			throw "second mesh is a non-closed mesh";
		}
	}
}

void RecycleResult()
{
	mesh1.n_triangles = result.n_triangles;
	mesh1.n_vertices = result.n_vertices;
	mesh1.triangles = result.triangles;
	mesh1.vertices = result.vertices;

	result.n_triangles = 0;
	result.n_vertices = 0;
	result.triangles = nullptr;
	result.vertices = nullptr;
}

void ResetMeshes()
{
	mesh1 = {};
	mesh2 = {};
	result = {};
}

const char* GetErrorMessage()
{
	return errorMessage.c_str();
}
