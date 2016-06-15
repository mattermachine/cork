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

// \NOTE: for some reason the return bool value is always marshalled true across to c#, thus the extra return param 'success'
bool CreateTriMesh(float vertices[], uint n_vertices, uint faces[], uint n_faces, int target, bool* success)
{
	/*
	std::cout << "Cork.main.cpp.CreateTriMesh vertices (#" << n_vertices << "):" << std::endl;
	for (int v = 0; v < n_vertices; v++)
	{
		std::cout << "\t(" << vertices[v * 3 + 0] << "," << vertices[v * 3 + 1] << "," << vertices[v * 3 + 2] << ")" << std::endl;
	}
	std::cout << "Cork.main.cpp.CreateTriMesh triangles (#" << n_faces << "):" << std::endl;
	uint max_ref_idx = 0;
	for (int i = 0; i < n_faces; i++)
	{
		std::cout << "\t(" << faces[i * 3 + 0] << "," << faces[i * 3 + 1] << "," << faces[i * 3 + 2] << ")" << std::endl;
		max_ref_idx = std::max(
			std::max(max_ref_idx,
					 faces[3 * i + 0]),
			std::max(faces[3 * i + 1],
					 faces[3 * i + 2])
		);
	}
	std::cout << "Cork.main.cpp.CreateTriMesh max vertex index: " << max_ref_idx << std::endl;
	*/
	if (target == 1)
	{
		std::cout << "Cork.main.cpp.CreateTriMesh mesh1 triangles ptr: " << mesh1.triangles << std::endl;
		if ((mesh1.triangles != nullptr) || (mesh1.vertices != nullptr))
		{
			errorMessage = "mesh1 creation failed: existing mesh not cleared";
			*success = false;
			return false;
		}
		mesh1 = {
			n_faces,
			n_vertices,
			faces,
			vertices
		};
		*success = true;
		return true;
	}
	if (target == 2)
	{
		std::cout << "Cork.main.cpp.CreateTriMesh mesh2 triangles ptr: " << mesh2.triangles << std::endl;
		if ((mesh2.triangles != nullptr) || (mesh2.vertices != nullptr))
		{
			errorMessage = "mesh2 creation failed: existing mesh not cleared";
			*success = false;
			return false;
		}
		mesh2 = {
			n_faces,
			n_vertices,
			faces,
			vertices
		};
		*success = true;
		return true;
	}
	*success = false;
	return false;
}

bool ComputeUnion(bool doSolidCheck)
{
	errorMessage = "";
	// free result as the data is owned/created in Cork library c++ land
	FreeResult();

	try
	{
		if (doSolidCheck)
		{
			bool success = SolidCheck();
			if (!success)
			{
				return false;
			}
		}
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
	// free result as the data is owned/created in Cork library c++ land
	FreeResult();

	try
	{
		if (doSolidCheck)
		{
			bool success = SolidCheck();
			if (!success)
			{
				return false;
			}
		}
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
	// free result as the data is owned/created in Cork library c++ land
	FreeResult();

	try
	{
		if (doSolidCheck)
		{
			bool success = SolidCheck();
			if (!success)
			{
				return false;
			}
		}
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
	// free result as the data is owned/created in Cork library c++ land
	FreeResult();

	try
	{
		if (doSolidCheck)
		{
			bool success = SolidCheck();
			if (!success)
			{
				return false;
			}
		}
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

bool SolidCheck()
{
	bool isSolidSuccess = false;
	int meshProblem = 0;
	// -- test mesh1 --
	// try to perform solid check
	// and catch exceptions accordingly
	try
	{
		isSolidSuccess = isSolid(mesh1, meshProblem);
	}
	catch (const std::exception& ex)
	{
		errorMessage = "mesh1 solidcheck failed: " + std::string(ex.what());
		return false;
	}
	catch (const char* ex)
	{
		errorMessage = "mesh1 solidcheck failed: " + std::string(ex);
		return false;
	}
	catch (...)
	{
		errorMessage = "mesh1 solidcheck failed: unknown error";
		return false;
	}

	// handle result of solid check
	if (!isSolidSuccess)
	{
		if (meshProblem == 1)
		{
			CORK_ERROR("isSolid() was given a self-intersecting mesh1");
			errorMessage = "first mesh is a self-intersecting mesh.";
		}
		else if (meshProblem == 2)
		{
			CORK_ERROR("isSolid() was given a non-closed mesh1");
			errorMessage = "first mesh is a non-closed mesh";
		}
		else
		{
			CORK_ERROR("isSolid() was given a mesh1 with unknown problem");
			errorMessage = "first mesh has an unknown problem";
		}

		// return before checking mesh2, if there was already failures
		return false;
	}

	// -- test mesh2 --
	// try to perform solid check
	// and catch exceptions accordingly
	try
	{
		isSolidSuccess = isSolid(mesh2, meshProblem);
	}
	catch (const std::exception& ex)
	{
		errorMessage = "mesh2 solidcheck failed: " + std::string(ex.what());
		return false;
	}
	catch (const char* ex)
	{
		errorMessage = "mesh2 solidcheck failed: " + std::string(ex);
		return false;
	}
	catch (...)
	{
		errorMessage = "mesh2 solidcheck failed: unknown error";
		return false;
	}

	// handle result of solid check
	if (!isSolidSuccess)
	{
		if (meshProblem == 1)
		{
			CORK_ERROR("isSolid() was given a self-intersecting mesh2");
			errorMessage = "second mesh is a self-intersecting mesh.";
		}
		else if (meshProblem == 2)
		{
			CORK_ERROR("isSolid() was given a non-closed mesh2");
			errorMessage = "second mesh is a non-closed mesh";
		}
		else
		{
			CORK_ERROR("isSolid() was given a mesh2 with unknown problem");
			errorMessage = "second mesh has an unknown problem";
		}
	}

	return isSolidSuccess;
}

void RecycleResult()
{
	// reset mesh1 before assigning result
	ResetMesh1();

	// put result into mesh1
	mesh1.n_triangles = result.n_triangles;
	mesh1.n_vertices = result.n_vertices;
	mesh1.triangles = result.triangles;
	mesh1.vertices = result.vertices;

	// DO NOT free result, just reset it as we assigned it to mesh1
	ResetResult();
}

void ResetMeshes()
{
	// reset mesh1 and mesh2 as the data is externally owned
	ResetMesh1();
	ResetMesh2();
	// free the result as the data is internally created
	FreeResult();
}

void FreeMesh1()
{
	// free the memory used by mesh1
	freeCorkTriMesh(&mesh1);
	mesh1.n_triangles = 0;
	mesh1.n_vertices = 0;
	mesh1.triangles = nullptr;
	mesh1.vertices = nullptr;
}

void ResetMesh1()
{
	mesh1.n_triangles = 0;
	mesh1.n_vertices = 0;
	mesh1.triangles = nullptr;
	mesh1.vertices = nullptr;
}

void FreeMesh2()
{
	// free the memory used by mesh1
	freeCorkTriMesh(&mesh2);
	mesh2.n_triangles = 0;
	mesh2.n_vertices = 0;
	mesh2.triangles = nullptr;
	mesh2.vertices = nullptr;
}

void ResetMesh2()
{
	mesh2.n_triangles = 0;
	mesh2.n_vertices = 0;
	mesh2.triangles = nullptr;
	mesh2.vertices = nullptr;
}

void FreeResult()
{
	// free the memory used by result
	freeCorkTriMesh(&result);
	result.n_triangles = 0;
	result.n_vertices = 0;
	result.triangles = nullptr;
	result.vertices = nullptr;
}

void ResetResult()
{
	result.n_triangles = 0;
	result.n_vertices = 0;
	result.triangles = nullptr;
	result.vertices = nullptr;
}

const char* GetErrorMessage()
{
	return errorMessage.c_str();
}
