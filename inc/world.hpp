#pragma once

#ifndef WORLD_HEADER
#define WORLD_HEADER

#include "cell.hpp"
#include "game_constants.hpp"
#include "shaders.hpp"
#include <SDL2/SDL.h>
#include <unordered_map>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
// extern float VP_RATIO;

class World {
    public:
    World(int width, int height);
    int width, height;
    std::unordered_map<CellType, uint32_t> VAOs;
    std::vector<std::vector<Cell>> cells;
    std::unordered_map<CellType, std::pair<uint32_t, uint32_t>> vbos;
    void Render(const Shader &s);
};

#endif

World::World(int width, int height)
    : width(width), height(height),
      cells(width, std::vector<Cell>(height, {CellType::NONE})) {
    // glGenVertexArrays(1, &VAO);
    for (int i = 0; i < width; i++) {
        cells[i][0].type = CellType::SAND;
    }
    for (int i = 0; i < width; i++) {
        cells[i][30].type = CellType::SAND;
    }

    for (int i = 0; i < width; i++) {
        cells[i][25].type = CellType::WATER;
    }
}

void World::Render(const Shader &s) {
    std::unordered_map<CellType, std::vector<glm::vec2>> instance_points;

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            if (cells[i][j].type == CellType::NONE) continue;
            if (vbos.find(cells[i][j].type) == vbos.end()) {
                glGenVertexArrays(1, &VAOs[cells[i][j].type]);
                glGenBuffers(1, &vbos[cells[i][j].type].first);
                glGenBuffers(1, &vbos[cells[i][j].type].second);
            }
            instance_points[cells[i][j].type].push_back(
                {((float)i - (float)width / 2)*4,
                 ((float)j - (float)height / 2)});
        }
    }

    // clang-format off
    float vertices[] = {
        -1.f,  1.f,
         1.f, -1.f,
        -1.f, -1.f,

        -1.f,  1.f,
         1.f, -1.f,
         1.f,  1.f,
    };
    // clang-format on

    // experimental
    GLuint FramebufferName = 0;
    glGenFramebuffers(1, &FramebufferName);
    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

    GLuint renderedTexture;
    glGenTextures(1, &renderedTexture);

    // "Bind" the newly created texture : all future texture functions will
    // modify this texture
    glBindTexture(GL_TEXTURE_2D, renderedTexture);

    // Give an empty image to OpenGL ( the last "0" )
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, 0);

    // Poor filtering. Needed !
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    GLuint depthrenderbuffer;
    glGenRenderbuffers(1, &depthrenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 768);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER, depthrenderbuffer);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture,
                         0);

    // Set the list of draw buffers.
    // GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    // glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
    // end experimental

    s.Use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(0.6f / 200, 1.0f / 200, 1.0f));
    int modelLoc = glGetUniformLocation(s.ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    for (auto const &it : vbos) {
        glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
        glViewport(0, 0, 200, 200);
        glBindVertexArray(VAOs[it.first]);
        glBindBuffer(GL_ARRAY_BUFFER, it.second.first);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                     GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                              (void *)0);
        glBindBuffer(GL_ARRAY_BUFFER, it.second.second);
        //     glBindBuffer(GL_ARRAY_BUFFER, it.second.second);
        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(glm::vec2) * instance_points[it.first].size(),
                     instance_points[it.first].data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                              (void *)0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glVertexAttribDivisor(
            1, 1); // tell OpenGL this is an instanced vertex attribute.
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // glBindVertexArray(VAO);
        float c = 1;
        if (it.first == CellType::WATER) c = 0;
        // std::cout << it.first << std::endl;
        glUniform3f(glGetUniformLocation(s.ID, "color"), 1.f, 1.f, c);
        // glViewport(0, 0, width, height);

        glDrawArraysInstanced(GL_TRIANGLES, 0, 6,
                              instance_points[it.first].size());
        glBindVertexArray(0);
        // glViewport(0,0,1000,600);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, FramebufferName);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        // glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBlitFramebuffer(0, 0, width, height, 0, 0, width, height,
                          GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT,
                          GL_NEAREST);
        // glViewport(0,0,1000,600);
        // glViewport(0,0,1024,768); /
    }
}