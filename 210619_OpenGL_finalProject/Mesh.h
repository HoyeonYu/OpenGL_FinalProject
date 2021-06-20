#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <gl/glut.h>
using namespace std;

class VECTOR3D {
public:
	float x, y, z;

	VECTOR3D() : x(0.0f), y(0.0f), z(0.0f) {}
	VECTOR3D(float x, float y, float z) : x(x), y(y), z(z) {}

	float Magnitude() {
		return sqrt(x * x + y * y + z * z);
	}

	float InnerProduct(VECTOR3D v) {
		return (x * v.x + y * v.y + z * v.z);
	}

	VECTOR3D CrossProduct(VECTOR3D v) {
		VECTOR3D result = (*this);
		result.x = y * v.z - z * v.y;
		result.y = z * v.x - x * v.z;
		result.z = x * v.y - y * v.x;

		return result;
	}

	void Normalize() {
		float size = sqrt(x * x + y * y + z * z);
		*this = *this * (1 / size);
	}

	VECTOR3D operator+(VECTOR3D v) {
		VECTOR3D result = (*this);
		result.x += v.x;
		result.y += v.y;
		result.z += v.z;

		return result;
	}

	VECTOR3D operator-(VECTOR3D v) {
		VECTOR3D result = (*this);
		result.x -= v.x;
		result.y -= v.y;
		result.z -= v.z;

		return result;
	}

	VECTOR3D operator*(float val) {
		VECTOR3D result = (*this);
		result.x *= val;
		result.y *= val;
		result.z *= val;

		return result;
	}

	void display() {
		cout << "(" << x << ", " << y << ", " << z << ")\n";
	}
};

class Vertex {
public:
	VECTOR3D position;
	VECTOR3D normal;
	vector<int> neighborFaces;
};

class Face {
public:
	int vertex[3];
	VECTOR3D normal;
};

class Mesh {
public:
	vector<Vertex> vertexArray;
	vector<Face> faceArray;

	void LoadMesh(string fileName) {
		ifstream file(fileName);

		if (!file) {
			cout << "File Not Found";
		}

		string inputString;

		while (!file.eof()) {
			getline(file, inputString);

			if (inputString[0] == '#') {
				continue;
			}

			string delimiter = " ";
			string tokens[4];
			size_t pos = 0;
			int index = 0;
			string token;

			while ((pos = inputString.find(delimiter)) != string::npos) {
				token = inputString.substr(0, pos);
				tokens[index] = token;
				inputString.erase(0, pos + delimiter.length());
				index++;

				if (index == 3) {
					break;
				}
			}

			tokens[3] = inputString;

			if (tokens[0] == "v") {
				Vertex v;
				v.position = VECTOR3D(stof(tokens[1]), stof(tokens[2]), stof(tokens[3]));
				vertexArray.push_back(v);
			}

			else if (tokens[0] == "f") {
				Face f;

				for (int i = 1; i <= 3; i++) {
					int val = 0;

					for (int j = 0; j < tokens[i].length(); j++) {
						if (tokens[i][j] == '/') {
							break;
						}

						val *= 10;
						val += tokens[i][j] - '0';
					}

					f.vertex[i - 1] = val;
					f.vertex[i - 1]--;
				}

				faceArray.push_back(f);
			}

			else {
				continue;
			}
		}

		file.close();
		cout << "Load Done\n\n\n";
	}

	void RenderMesh(VECTOR3D colorVec, float scale) {
		glColor3f(colorVec.x, colorVec.y, colorVec.z);
		glTranslatef(0, 0, 0);
		glBegin(GL_TRIANGLES);

		for (int i = 0; i < faceArray.size(); i++) {
			Vertex v0 = vertexArray[faceArray[i].vertex[0]];
			Vertex v1 = vertexArray[faceArray[i].vertex[1]];
			Vertex v2 = vertexArray[faceArray[i].vertex[2]];

			glVertex3f(v0.position.x * scale * 0.9, v0.position.y * scale, v0.position.z * scale * 0.8);
			glVertex3f(v1.position.x * scale * 0.9, v1.position.y * scale, v1.position.z * scale * 0.8);
			glVertex3f(v2.position.x * scale * 0.9, v2.position.y * scale, v2.position.z * scale * 0.8);
		}

		glEnd();

		glColor3f(0, 0, 0);
		glBegin(GL_LINES);

		for (int i = 0; i < faceArray.size(); i++) {
			Vertex v0 = vertexArray[faceArray[i].vertex[0]];
			Vertex v1 = vertexArray[faceArray[i].vertex[1]];
			Vertex v2 = vertexArray[faceArray[i].vertex[2]];

			glVertex3f(v0.position.x * scale * 0.9, v0.position.y * scale, v0.position.z * scale * 0.8);
			glVertex3f(v1.position.x * scale * 0.9, v1.position.y * scale, v1.position.z * scale * 0.8);
			glVertex3f(v2.position.x * scale * 0.9, v2.position.y * scale, v2.position.z * scale * 0.8);
		}

		glEnd();
	}
};