/*
** EPITECH PROJECT, 2019
** RogueLike
** File description:
** HitList
*/

#pragma once

#include <vector>
#include "Hit.hpp"

class HitList : public hitable {
    public:
        HitList() {}

        virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;

        inline void emplace_back(hitable *elem)
        { m_list.emplace_back(elem); }
    private:
        std::vector<hitable *> m_list;
};

bool HitList::hit(const ray& r, float tmin, float tmax, hit_record& rec) const
{
    hit_record temp_rec;
    bool hit_anything = false;
    double closest_so_far = tmax;

    for (int i = 0; i < m_list.size(); i++) {
        if (m_list[i]->hit(r, tmin, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}