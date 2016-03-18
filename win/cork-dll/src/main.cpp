// Win32Project1.cpp : Defines the exported functions for the DLL application.
//

#include "main.h"
#include "cork.h"
#include <stdexcept>
#include <string>

uint GetNbVertices()
{
	return result.n_vertices;
};

uint GetNbFaces()
{
	return result.n_triangles;
};

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
		if (doSolidCheck) {
			if (!isSolid(mesh1))
				throw("ComputeUnion failed: first mesh failed solid check.");
			if (!isSolid(mesh2))
				throw("ComputeUnion failed: second mesh failed solid check.");
		}

		computeUnion(mesh1, mesh2, &result);
		return true;
	}
	catch (const std::exception& ex)
	{
		errorMessage = "ComputeUnion failed: " + std::string(ex.what());
		return false;
	}
	catch (const std::string& ex)
	{
		errorMessage = "ComputeUnion failed: " + ex;
		return false;
	}
	catch (...)
	{
		errorMessage = "ComputeUnion failed: unknown error";
		return false;
	}
}

bool ComputeIntersection(bool doSolidCheck)
{
	errorMessage = "";
	result = {};

	try
	{
		if (doSolidCheck) {
			if (!isSolid(mesh1))
				throw("ComputeIntersection failed: first mesh failed solid check.");
			if (!isSolid(mesh2))
				throw("ComputeIntersection failed: second mesh failed solid check.");
		}

		computeIntersection(mesh1, mesh2, &result);
		return true;
	}
	catch (const std::exception& ex)
	{
		errorMessage = "ComputeIntersection failed: " + std::string(ex.what());
		return false;
	}
	catch (const std::string& ex)
	{
		errorMessage = "ComputeIntersection failed: " + ex;
		return false;
	}
	catch (...)
	{
		errorMessage = "ComputeSymmetricDifference failed: unknown error";
		return false;
	}
}


bool ComputeDifference(bool doSolidCheck)
{
	errorMessage = "";
	result = {};

	try
	{
		if (doSolidCheck) {
			if (!isSolid(mesh1))
				throw("ComputeDifference failed: first mesh failed solid check.");
			if (!isSolid(mesh2))
				throw("ComputeDifference failed: second mesh failed solid check.");
		}

		computeDifference(mesh1, mesh2, &result);
		return true;
	}
	catch (const std::exception& ex)
	{
		errorMessage = "ComputeDifference failed: " + std::string(ex.what());
		return false;
	}
	catch (const std::string& ex)
	{
		errorMessage = "ComputeDifference failed: " + ex;
		return false;
	}
	catch (...)
	{
		errorMessage = "ComputeDifference failed: unknown error";
		return false;
	}
}


bool ComputeSymmetricDifference(bool doSolidCheck)
{
	errorMessage = "";
	result = {};

	try
	{
		if (doSolidCheck) {
			if (!isSolid(mesh1))
				throw("ComputeSymmetricDifference failed: first mesh failed solid check.");
			if (!isSolid(mesh2))
				throw("ComputeSymmetricDifference failed: second mesh failed solid check.");
		}

		computeSymmetricDifference(mesh1, mesh2, &result);
		return true;
	}
	catch (const std::exception& ex)
	{
		errorMessage = "ComputeSymmetricDifference failed: " + std::string(ex.what());
		return false;
	}
	catch (const std::string& ex)
	{
		errorMessage = "ComputeSymmetricDifference failed: " + ex;
		return false;
	}
	catch (...)
	{
		errorMessage = "ComputeSymmetricDifference failed: unknown error";
		return false;
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