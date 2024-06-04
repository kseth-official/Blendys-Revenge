#pragma once

#include "common.hpp"
#include "tiny_ecs.hpp"
#include "components.hpp"
#include "tiny_ecs_registry.hpp"

// A simple physics system that moves rigid bodies and checks for collision
class PhysicsSystem
{
public:
    void step(float elapsed_ms);
    static std::map<Direction, Mesh> loaded_blendy_meshes;
    static std::map<Direction, Mesh> loaded_minion_meshes;
    static std::map<Direction, Mesh> loaded_boss_meshes;

    PhysicsSystem()
    {
        for (const auto& entry : blendy_direction_mesh) {
            Direction dir = entry.first;
            std::string path = entry.second;
            Mesh mesh;
            if (Mesh::loadFromOBJFile(path, mesh.vertices, mesh.vertex_indices, mesh.original_size)) {
                loaded_blendy_meshes[dir] = mesh;
            }
            else {
                // Handle loading failure
            }
        }
        for (const auto& entry : minion_direction_mesh) {
            Direction dir = entry.first;
            std::string path = entry.second;
            Mesh mesh;
            if (Mesh::loadFromOBJFile(path, mesh.vertices, mesh.vertex_indices, mesh.original_size)) {
                loaded_minion_meshes[dir] = mesh;
            }
            else {
                // Handle loading failure
            }
        }
        for (const auto& entry : boss_direction_mesh) {
            Direction dir = entry.first;
            std::string path = entry.second;
            Mesh mesh;
            if (Mesh::loadFromOBJFile(path, mesh.vertices, mesh.vertex_indices, mesh.original_size)) {
                loaded_boss_meshes[dir] = mesh;
            }
            else {
                // Handle loading failure
            }
        }
    }
};