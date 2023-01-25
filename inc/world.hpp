#pragma once

#ifndef WORLD_HEADER
#define WORLD_HEADER

#include "cell.hpp"
#include "game_constants.hpp"
#include "shaders.hpp"
#include <SDL2/SDL.h>
#include <unordered_map>
#include <random>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <vector>
#include <random>

std::unordered_map<CellType, glm::vec3> COLORS;

extern uint32_t frame_count;
extern RNG *rng_gen;
// extern std::uniform_int_distribution<int> *dist;

#define COLOR_MAP(CellType)                                                    \
    COLORS[CellType][0], COLORS[CellType][1], COLORS[CellType][2]
// extern float VP_RATIO;

class World {
    public:
    World(int width, int height);
    int width, height;
    std::unordered_map<CellType, uint32_t> VAOs;
    std::vector<std::vector<Cell>> cells;
    std::unordered_map<CellType, std::pair<uint32_t, uint32_t>> vbos;
    void Render(const Shader &s);
    void updateDim(int w, int h);
    void update();
};

#endif

World::World(int width, int height)
    : width(width), height(height),
      cells(width, std::vector<Cell>(height, {CellType::NONE})) {
    // glGenVertexArrays(1, &VAO);
    std::cout << "world [" << width << ", " << height << "]" << std::endl;
    for (int i = 0; i < width; i++) {
        cells[i][0].type = CellType::SAND;
    }
    for (int i = 0; i < width; i++) {
        cells[i][26].type = CellType::SAND;
    }
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 80; j++) {
            cells[i + 100][j + 100].type = CellType::SAND;
        }
    }

    for (int i = 0; i < width; i++) {
        cells[i][25].type = CellType::WATER;
    }
    COLORS[CellType::WATER] = {37.f / 255, 150.f / 255, 190.f / 255};
    COLORS[CellType::SAND] = {234.f / 255, 182.f / 255, 118.f / 255};
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
                {((float)i - (float)width / 2) + 0.5,
                 ((float)j - (float)height / 2) + 0.5});
            // std::cout << instance_points[cells[i][j].type].back()[0] <<
            // std::endl;
        }
    }

    // clang-format off
    float vertices[] = {
        -0.5f,  0.5f,
         0.5f, -0.5f,
        -0.5f, -0.5f,

        -0.5f,  0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
    };
    // clang-format on

    // experimental
    // GLuint FramebufferName = 0;
    // glGenFramebuffers(1, &FramebufferName);
    // glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

    // GLuint renderedTexture;
    // glGenTextures(1, &renderedTexture);

    // // "Bind" the newly created texture : all future texture functions will
    // // modify this texture
    // glBindTexture(GL_TEXTURE_2D, renderedTexture);

    // // Give an empty image to OpenGL ( the last "0" )
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
    //              GL_UNSIGNED_BYTE, 0);

    // // Poor filtering. Needed !
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // GLuint depthrenderbuffer;
    // glGenRenderbuffers(1, &depthrenderbuffer);
    // glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
    // glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 768);
    // glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
    //                           GL_RENDERBUFFER, depthrenderbuffer);
    // glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
    // renderedTexture,
    //                      0);

    // Set the list of draw buffers.
    // GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    // glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
    // end experimental

    s.Use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3((float)height / (width * height / 2),
                                        1.0f / (height / 2), 1.0f));
    // std::cout << "scale mat: " << std::endl;
    // std::cout << glm::to_string(model) << std::endl;
    int modelLoc = glGetUniformLocation(s.ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    for (auto const &it : vbos) {
        // glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
        // glViewport(0, 0, 200, 200);
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
        // float c = 1;
        // if (it.first == CellType::WATER) c = 0;
        // std::cout << it.first << std::endl;
        glUniform3f(glGetUniformLocation(s.ID, "color"), COLOR_MAP(it.first));
        // glViewport(0, 0, width, height);

        glDrawArraysInstanced(GL_TRIANGLES, 0, 6,
                              instance_points[it.first].size());
        glBindVertexArray(0);
        // glViewport(0,0,1000,600);
        // glBindFramebuffer(GL_READ_FRAMEBUFFER, FramebufferName);
        // glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        // glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // glBlitFramebuffer(0, 0, width, height, 0, 0, width, height,
        //                   GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT,
        //                   GL_NEAREST);
        // glViewport(0,0,1000,600);
        // glViewport(0,0,1024,768); /
    }
}

void World::updateDim(int w, int h) {
    width = w;
    height = h;
}

void World::update() {
    // for (auto &row : cells) {
    //     for (auto &cell : row) {
    //         cell.updatedThisFrame = false;
    //     }
    // }
    // std::string x;
    // std:: cin >> x;
    uint32_t f_even = frame_count & 1;
    uint32_t ran = f_even ? 0 : 1;
    for (int j = 0; j < height; j++) {
        for (uint32_t i = ran ? 0 : width - 1; ran ? i < width : i > 0;
             ran ? i++ : i--) {
            cells[i][j].updatedThisFrame = false;
        }
    }

    for (int j = 0; j < height; j++) {
        for (uint32_t i = ran ? 0 : width - 1; ran ? i < width : i > 0;
             ran ? i++ : i--) {

            if (cells[i][j].type == CellType::NONE) continue;
            if (cells[i][j].updatedThisFrame) continue;

            // if(j == 0) continue;
            if (i == 0 || i == width - 1) continue;

            switch (cells[i][j].type) {
            case CellType::SAND: {
                if (j == 0) break;
                if (cells[i][j - 1].type != CellType::SAND) {
                    cells[i][j].updatedThisFrame = true;
                    std::swap(cells[i][j], cells[i][j - 1]);
                } else if (cells[i - 1][j - 1].type != CellType::SAND) {
                    cells[i][j].updatedThisFrame = true;
                    std::swap(cells[i][j], cells[i - 1][j - 1]);
                } else if (cells[i + 1][j - 1].type != CellType::SAND) {
                    cells[i][j].updatedThisFrame = true;
                    std::swap(cells[i][j], cells[i + 1][j - 1]);
                }
                break;
            }
            case CellType::WATER: {
                int rng = rng_gen->get_num();
                if (rng <= 3 && cells[i - 1][j].type == CellType::NONE) {
                    std::swap(cells[i][j], cells[i - 1][j]);
                } else if (rng <= 6 && cells[i + 1][j].type == CellType::NONE) {
                    std::swap(cells[i][j], cells[i + 1][j]);
                }
                if (j == 0) break;
                if (cells[i][j - 1].type == CellType::NONE) {
                    cells[i][j].updatedThisFrame = true;
                    std::swap(cells[i][j], cells[i][j - 1]);
                } else if (cells[i - 1][j - 1].type == CellType::NONE) {
                    cells[i][j].updatedThisFrame = true;
                    std::swap(cells[i][j], cells[i - 1][j - 1]);
                } else if (cells[i + 1][j - 1].type == CellType::NONE) {
                    cells[i][j].updatedThisFrame = true;
                    std::swap(cells[i][j], cells[i + 1][j - 1]);
                }
                break;
            }
            }
        }
    }
}