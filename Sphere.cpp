#include <iostream>
#include <fstream>
#include <map>
#include <tuple>
#include <vector>
#include <numeric>

#include "Sphere.h"

/* Code sampled from https://gist.github.com/zwzmzd/0195733fa1210346b00d, adjusted to use GL_QUADS instead of GL_QUADS_STRIP */
Sphere::Sphere(glm::vec3 eyePos) : eyePos(eyePos)
{
    model = glm::rotate(glm::mat4(3), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    int i, j;
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
    std::vector<GLfloat> normals;
    int indicator = 0;
    for (i = 0; i <= stackCount; i++) {
        double lat0 = glm::pi<double>() * (-0.5 + (double)(i - 1) / stackCount);
        double z0 = sin(lat0);
        double zr0 = cos(lat0);

        double lat1 = glm::pi<double>() * (-0.5 + (double)i / stackCount);
        double z1 = sin(lat1);
        double zr1 = cos(lat1);
        for (j = 0; j <= sectorCount; j++) {
            double lng = 2 * glm::pi<double>() * (double)(j - 1) / sectorCount;
            double lng1 = 2 * glm::pi<double>() * (double)(j) / sectorCount;

            double x1 = cos(lng1);
            double y1 = sin(lng1);
            double x = cos(lng);
            double y = sin(lng);

            vertices.push_back(x * zr1);
            vertices.push_back(y * zr1);
            vertices.push_back(z1);
            indices.push_back(indicator);
            indicator++;

            vertices.push_back(x * zr0);
            vertices.push_back(y * zr0);
            vertices.push_back(z0);
            indices.push_back(indicator);
            indicator++;

            vertices.push_back(x1 * zr0);
            vertices.push_back(y1 * zr0);
            vertices.push_back(z0);
            indices.push_back(indicator);
            indicator++;

            vertices.push_back(x1 * zr1);
            vertices.push_back(y1 * zr1);
            vertices.push_back(z1);
            indices.push_back(indicator);
            indicator++;

            float x_avg = ((x * zr1) + (x * zr0) + (x1 * zr0) + (x1 * zr1)) / 4;
            float y_avg = ((y * zr1) + (y * zr0) + (y1 * zr0) + (y1 * zr1)) / 4;
            float z_avg = (2 * z0 + 2 * z1) / 4;

            glm::vec3 avg = glm::normalize(glm::vec3(x_avg, y_avg, z_avg));

            normals.push_back(avg.x);
            normals.push_back(avg.y);
            normals.push_back(avg.z);

            normals.push_back(avg.x);
            normals.push_back(avg.y);
            normals.push_back(avg.z);

            normals.push_back(avg.x);
            normals.push_back(avg.y);
            normals.push_back(avg.z);

            normals.push_back(avg.x);
            normals.push_back(avg.y);
            normals.push_back(avg.z);
        }
        indices.push_back(GL_PRIMITIVE_RESTART_FIXED_INDEX);
    }

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glGenBuffers(1, &vbo_n);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_n);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat), &normals[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    numsToDraw = indices.size();
}

Sphere::~Sphere()
{
    // Delete the VBO/EBO and the VAO.
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &vbo_n);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);
}

void Sphere::draw(GLuint shaderProgram, glm::mat4 projection, glm::mat4 view, glm::mat4 C)
{
    glUseProgram(shaderProgram);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "cameraPos"), 1, GL_FALSE, glm::value_ptr(eyePos));
    glUniform1f(glGetUniformLocation(shaderProgram, "drawDiscoball"), 1.0);

    glBindVertexArray(vao);
    
    glPatchParameteri(GL_PATCH_VERTICES, 4);
    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(GL_PRIMITIVE_RESTART_FIXED_INDEX);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glDrawElements(GL_QUADS, numsToDraw, GL_UNSIGNED_INT, NULL);

    glBindVertexArray(0);
    glUseProgram(0);
}

void Sphere::update()
{
    // Spin the cube by 1 degree.
    spin(0.1f);
}

void Sphere::spin(float deg)
{
    // Update the model matrix by multiplying a rotation matrix
    model = model * glm::rotate(glm::radians(deg), glm::vec3(0.0f, 0.0f, 1.0f));
}