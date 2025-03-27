#pragma once
#include <iostream>
#include <gfl.h>
#include <functional>

class PMat
{

public:
    enum Model
    {
        LEAP_FROG,
        EULER_EXPLICIT,
        FIXE
    };

private:
    bool _flag3d = false;

    float _m; // masse
    GFLpoint _pos;
    GFLvector _vit;
    GFLvector _frc; // Vecteur d'accumulation des forces
    std::function<void(float h)> _update;
    Model _model;

    void update_leapFrog(float h)
    {
        _vit.x += h * _frc.x / _m;
        _vit.y += h * _frc.y / _m;
        if (_flag3d)
            _vit.z += h * _frc.z / _m;

        _pos.x += h * _vit.x;
        _pos.y += h * _vit.y;
        if (_flag3d)
            _pos.z += h * _vit.z;

        _frc = _flag3d ? (GFLvector){0., 0., 0.} : (GFLvector){0., 0.};
    }

    void update_euler(float h)
    {
        _pos.x += h * _vit.x;
        _pos.y += h * _vit.y;

        _vit.x += h * (_frc.x / _m);
        _vit.y += h * (_frc.y / _m);

        _frc = (GFLvector){0., 0.};
    }

    void update_fixe()
    {
        _frc = _flag3d ? (GFLvector){0., 0., 0.} : (GFLvector){0., 0.}; // vide le buffer de force
    }

public:
    PMat()
    {
        _m = 1;
        _model = PMat::Model::FIXE;
        _pos = GFLvector{0.f, 0.f};
        _vit = GFLvector{0.f, 0.f};
        _frc = GFLvector{0.f, 0.f};
    }

    PMat(bool flag3d) : _flag3d(flag3d)
    {
        _m = 1;
        _model = PMat::Model::FIXE;
        _pos = flag3d ? GFLvector{0.f, 0.f, 0.f} : GFLvector{0.f, 0.f};
        _vit = flag3d ? GFLvector{0.f, 0.f, 0.f} : GFLvector{0.f, 0.f};
        _frc = flag3d ? GFLvector{0.f, 0.f, 0.f} : GFLvector{0.f, 0.f};
    }

    PMat(GFLvector position)
    {
        _m = 1;
        _model = PMat::Model::FIXE;
        _pos = position;
        _vit = GFLvector{0.f, 0.f};
        _frc = GFLvector{0.f, 0.f};
    }

    PMat(GFLvector position, bool flag3d) : _flag3d(flag3d)
    {
        _m = 1;
        _model = PMat::Model::FIXE;
        _pos = position;
        _vit = GFLvector{0.f, 0.f, 0.f};
        _frc = GFLvector{0.f, 0.f, 0.f};
    }

    PMat(float m, GFLpoint P0, GFLvector V0, Model model, bool flag3d);

    PMat(const PMat &pmat) : _m(pmat._m), _update(pmat._update), _model(pmat._model)
    {
        _pos = (GFLvector)(pmat._pos);
        _vit = (GFLvector)(pmat._vit);
        _frc = (GFLvector)(pmat._frc);
    }

    GFLvector force() const
    {
        return _frc;
    }

    void addForce(GFLvector vector)
    {
        // std::cout << "BF add _frc.x " << _frc.x << std::endl;

        _frc = gfl_AddVect(_frc, vector);

        // std::cout << "AF add _frc.x " << _frc.x << std::endl;
    }

    void subForce(GFLvector &vector)
    {
        _frc = gfl_SubVect(vector, _frc);
    }

    GFLpoint position() const
    {
        return _pos;
    }

    void updateX(float step)
    {
        _pos.x += step;
    }

    void updateY(float step)
    {
        _pos.y += step;
    }

    GFLvector vitesse() const
    {
        return _vit;
    }

    void update(float h)
    {
        if (_model == LEAP_FROG)
        {
            update_leapFrog(h);
            return;
        }
        update_fixe();
    }
};
