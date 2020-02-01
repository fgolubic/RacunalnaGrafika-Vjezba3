#pragma once
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp>
#include<vector>
#include<iostream>


	struct VertexS
	{
		float x, y, z;
		VertexS() {}
		VertexS(float initX, float initY, float initZ)
		{
			x = initX; y = initY; z = initZ;
		}

		glm::vec3 getGlmVector() {
			return glm::vec3(x, y, z);
		}
	};

	struct PolygonS
	{
		VertexS v1, v2, v3;
		PolygonS() {}
		PolygonS(VertexS first, VertexS second, VertexS third)
		{
			v1 = first; v2 = second; v3 = third;
		}
	};

	struct TrajectoryS
	{
		std::vector<VertexS> R;

		const glm::mat4x4 B = glm::mat4x4(-1, 3, -3, 1,
										   3, -6, 0, 4,
										  -3, 3, 3, 1,
										   1, 0, 0, 0);
		const glm::mat4x3 dB = glm::mat4x3(-1, 2, -1,
											3, -4, 0,
											-3, 2, 1,
											1, 0, 0);

		std::vector<glm::vec3> trajectoryPoints;

		TrajectoryS(std::vector<VertexS> vertices)
		{
			R = vertices;
			trajectoryPoints = calculateTrajectoryPoints();
		}

		std::vector<glm::vec3> calculateTrajectoryPoints()
		{
			std::vector<glm::vec3> temp;

			for (unsigned int i = 1; i < R.size() - 2; i++)
			{
				for (float t = 0.00f; t < 1.00f; t += 0.05f)
				{
					glm::vec3 pom = calculatePosition(t, i);
					temp.push_back(pom);
				}
			}
			return temp;
		}


		glm::vec3 calculatePosition(float t, int i)
		{

			glm::vec4 T = glm::vec4(t*t*t, t*t, t, 1);
			glm::mat3x4 pointsR =
				glm::mat3x4(
					R[i - 1].x, R[i].x, R[i + 1].x, R[i + 2].x,
					R[i - 1].y, R[i].y, R[i + 1].y, R[i + 2].y,
					R[i - 1].z, R[i].z, R[i + 1].z, R[i + 2].z
				);

			return T * 0.166666666667f * B * pointsR;

		}

		glm::vec3 derivateP(float t, int i)
		{
			glm::vec3 T = glm::vec3(t*t, t, 1);

			glm::mat3x4 pointsR = glm::mat3x4(
				R[i - 1].x, R[i].x, R[i + 1].x, R[i + 2].x,
				R[i - 1].y, R[i].y, R[i + 1].y, R[i + 2].y,
				R[i - 1].z, R[i].z, R[i + 1].z, R[i + 2].z
			);

			return T * 0.5f * dB * pointsR;
		}

		glm::vec3 secondDerivativeP(float t, int i)
		{
			glm::vec3 dVector = glm::vec3(2 * t, 1, 0);
			glm::mat3x4 pointR = glm::mat3x4(
				R[i - 1].x, R[i].x, R[i + 1].x, R[i + 2].x,
				R[i - 1].y, R[i].y, R[i + 1].y, R[i + 2].y,
				R[i - 1].z, R[i].z, R[i + 1].z, R[i + 2].z
			);

			return dVector * 0.5f * dB * pointR;
		}

	};


	struct ObjectS {

		std::vector<PolygonS> polygons;

		ObjectS( std::vector<PolygonS> polygonList) 
		{
			polygons = polygonList;
		}

	};
