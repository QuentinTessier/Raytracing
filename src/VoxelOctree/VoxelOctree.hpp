#pragma once

#include <iostream>
#include <cstring>
#include "BoundingBox.hpp"

static inline std::ostream& operator<<(std::ostream& o, BoundingBox const& b)
{
    o << "BoundingBox : { " << "( " << b.center.x << ", " << b.center.y << ", " << b.center.z << " ), " << b.size << " }";
    return o;
}

static inline BoundingBox BuildFromChildID(BoundingBox const& p, size_t id)
{
    float nSize = p.size / 2.f;

    switch (id) {
        case 0: { // -x, -y, -z
            return BoundingBox(
                    glm::vec3(p.center.x - nSize, p.center.y - nSize, p.center.z - nSize),
                    nSize
                );
        }
        case 1: { // +x, -y, -z
            return BoundingBox(
                    glm::vec3(p.center.x + nSize, p.center.y - nSize, p.center.z - nSize),
                    nSize
                );
        }
        case 2: { // -x, +y, -z
            return BoundingBox(
                    glm::vec3(p.center.x - nSize, p.center.y + nSize, p.center.z - nSize),
                    nSize
                );
        }
        case 3: { // +x, +y, -z
            return BoundingBox(
                    glm::vec3(p.center.x + nSize, p.center.y + nSize, p.center.z - nSize),
                    nSize
                );
        }
        case 4: { // -x, -y, +z
            return BoundingBox(
                    glm::vec3(p.center.x - nSize, p.center.y - nSize, p.center.z + nSize),
                    nSize
                );
        }
        case 5: { // +x, -y, +z
            return BoundingBox(
                    glm::vec3(p.center.x + nSize, p.center.y - nSize, p.center.z + nSize),
                    nSize
                );
        }
        case 6: { // -x, +y, +z
            return BoundingBox(
                    glm::vec3(p.center.x - nSize, p.center.y + nSize, p.center.z + nSize),
                    nSize
                );
        }
        case 7: { // +x, +y, +z
            return BoundingBox(
                    glm::vec3(p.center.x + nSize, p.center.y + nSize, p.center.z + nSize),
                    nSize
                );
        }
        default:
            return p;
    }
};

enum VoxelOctreeNodeType {
    VONT_UNDEFINED,
    VONT_Branching,
    VONT_Leaf
};

struct VoxelInfo {
    size_t x, y, z;
    glm::vec3 color;
};

struct VoxelOctree {
    VoxelOctreeNodeType type;
    BoundingBox volume;
    float voxel_volume; // define the size of each voxel

    union VoxelOctreeData {
        struct VoxelOctreeBranch {
            VoxelOctree *children[8];
        } branching;

        struct VoxelOctreeLeaf {
            glm::vec3 vColor;
        } leaf;
    } data;

    VoxelOctree(VoxelOctreeNodeType type, BoundingBox const& b, float voxel_volume = 1.f)
        : type(type), volume(b), voxel_volume(voxel_volume)
    {
        memset(&data, 0, sizeof(data));
    }

};

static inline VoxelOctree *VoxelOctree_create(glm::vec3 center, float voxel_size, size_t n_voxels)
{
    BoundingBox b(center, (float)n_voxels * voxel_size);
    return new VoxelOctree(VONT_UNDEFINED, b, voxel_size);
}

static inline glm::vec3 CoordToPoint(VoxelInfo const& vi, BoundingBox const& b, float voxel_size)
{
    glm::vec3 vmin = b.min();
    return glm::vec3(
        vmin.x + vi.x * voxel_size,
        vmin.y + vi.y * voxel_size,
        vmin.z + vi.z * voxel_size
    );
}

static inline void VoxelOctree_insert(VoxelOctree *vo, VoxelInfo const& vi)
{
    if (vo) {
        glm::vec3 point = CoordToPoint(vi, vo->volume, vo->voxel_volume);
        bool contains = vo->volume.Contains(point);
        if (contains && vo->volume.size > vo->voxel_volume) {
            vo->type = VONT_Branching;
            for (size_t i = 0; i < 8; i += 1) {
                BoundingBox cb = BuildFromChildID(vo->volume, i);
                if (cb.Contains(point)) {
                    vo->data.branching.children[i] = new VoxelOctree(VONT_UNDEFINED, cb, vo->voxel_volume);
                    VoxelOctree_insert(vo->data.branching.children[i], vi);
                    return;
                }
            }
        } else if (contains) {
            vo->type = VONT_Leaf;
            vo->data.leaf.vColor = vi.color;
        }
    }
}

void VoxelOctree_destroy(VoxelOctree *vo)
{
    if (vo && vo->type == VONT_Branching) {
        for (size_t i = 0; i < 8; i += 1) {
            VoxelOctree_destroy(vo->data.branching.children[i]);
        }
        delete vo;
    } else if (vo) {
        delete vo;
    }
}

static inline std::ostream& operator<<(std::ostream& o, const VoxelOctree *vo)
{
    o << "VoxelOctree : ";
    if (vo) {
        o << "{\n\t" << vo->volume << "\n\tVoxelVolume : " << vo->voxel_volume << "\n";
        if (vo->type == VONT_Branching) {
            o << "\tType : Branch\n";
            for (size_t i = 0; i < 8; i += 1) {
                o << "\t\tChild[" << i << "] : ";
                if (vo->data.branching.children[i] == 0)
                    o << "null\n";
                else
                    o << &vo->data.branching.children[i] << "\n";
            }
        } else if (vo->type == VONT_Leaf) {
            o << "\tType : Voxel\n\t\tColor : ( " << vo->data.leaf.vColor.x << ", " << vo->data.leaf.vColor.y << ", " << vo->data.leaf.vColor.z << " )\n";
        } else {
            o << "\tType : UNDEFINED\n";
        }
        o << "}\n";
    } else {
        o << "null";
    }
    return o;
}

bool VoxelOctree_Intersect(const VoxelOctree *vo, ray const& r, float tmin, float tmax, Record& rec)
{
    if (vo && vo->type == VONT_Branching) {
        if (vo->volume.Intersect(r, tmin, tmax)) {
            bool hit_anything = false;
            float closest_so_far = tmax;
            Record temp_rec;
            for (size_t i = 0; i < 8; i += 1) {
                if (VoxelOctree_Intersect(vo->data.branching.children[i], r, tmin, closest_so_far, temp_rec)) {
                    hit_anything = true;
                    closest_so_far = temp_rec.t;
                    rec = temp_rec;
                }
            }
            return hit_anything;
        }
    } else if (vo && vo->type && VONT_Leaf) {
        Record temp_rec;
        if (vo->volume.Intersect(r, tmin, tmax, temp_rec)) {
            rec = temp_rec;
            rec.color = vo->data.leaf.vColor;
            return true;
        }
    }
    return false;
}