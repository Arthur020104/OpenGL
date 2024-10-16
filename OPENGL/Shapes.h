#pragma once
const float PI = 3.14159265358979323846f;
void retangle(unsigned int& VAO, unsigned int& VBO, unsigned int& EBO, unsigned int& texture1, unsigned int& texture2);
void CreateCircle(float r, int tri, int render, unsigned int& VAO, unsigned int& VBO, unsigned int& EBO);
void triangles(unsigned int& VAO, unsigned int& VBO, const int& vertices);
void house(unsigned int& VAO, unsigned int& VBO, unsigned int& EBO);
void box(unsigned int& VAO, unsigned int& VBO, unsigned int& EBO, unsigned int& texture1, unsigned int& texture2);