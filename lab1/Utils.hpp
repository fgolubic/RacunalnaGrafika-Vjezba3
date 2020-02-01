#pragma once
#define _USE_MATH_DEFINES
#include<math.h>
#include"Structs.hpp"
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>

std::vector<std::string> refactor(std::string s) {
	std::vector<std::string> things;

	for (int i = 1; i < s.size(); i++) {
		if (s[i] != ' ') {
			things.push_back(s.substr(i, s.substr(i).find(" ")));
			std::string temp = s.substr(i);
			if (s[0] == 'v') {
				i += 3;
			}
		}

	}

	return things;
}

ObjectS * load() {
	std::ifstream objectFile;
	objectFile.open("./tetra.obj", std::ios::in);
	std::string c;
	std::vector<std::string> input;
	std::vector<VertexS> vertices;
	std::vector<PolygonS> polygons;

	while (getline(objectFile, c)) {
		
		for (int i = 0; i < c.size(); i++) {
			if (c[i] == '#' || c[i] == 'g') break;
			if (c[i] == 'v' || c[i] == 'f') {
				 input = refactor(c.substr(i, c.size()));
				if (c[i] == 'v') {
					float x = std::stof(input[0]);
					float y = std::stof(input[1]);
					float z = std::stof(input[2]);

					vertices.push_back(VertexS(x,y,z));
				}
				else if (c[i] == 'f') {
					polygons.push_back(PolygonS(vertices[std::stoi(input[0]) - 1],
												vertices[std::stoi(input[1]) - 1],
												vertices[std::stoi(input[2]) - 1]));
				}

			}
		}
	}

	return new ObjectS(polygons);

}

std::vector<VertexS> readTrajectory() {

	std::ifstream trajectoryFile;
	trajectoryFile.open("./Bspline.txt", std::ios::in);
	std::string line;
	std::vector<VertexS> trajectoryVertices;
	float x, y, z;

	while (std::getline(trajectoryFile,line)) {

		if (!line.length()) break;

		std::istringstream stream(line);
		
		stream >> x >> y >> z;

		trajectoryVertices.push_back(VertexS(x, y, z));
	}
	
	return trajectoryVertices;
}

float getAngle(glm::vec3 v1, glm::vec3 v2) {

	return acos(glm::dot(v1, v2) / (glm::length(v1)*glm::length(v2))) * 180.0f / M_PI;

}