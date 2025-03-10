#pragma once
#include "PMat.hpp"

class Link
{
public:
    enum Type
    {
        HOOK,
        EXTERNAL_FORCE
    };

    Type _type;

private:
    float _k, _z, _l, _l0, _maxLength;
    PMat &_pmat1;
    PMat &_pmat2;
    GFLvector *_g;

    Link();

    Link(PMat &pmat1, GFLvector *g) : _pmat1(pmat1), _pmat2(pmat1), _type(EXTERNAL_FORCE)
    {
        _g = g;
        _update = [this]()
        {
            this->_pmat1.addForce(*_g);
        };
    }

    Link(PMat &pmat1, PMat &pmat2, float k) : _pmat1(pmat1),
                                              _pmat2(pmat2),
                                              _k(k),
                                              _z(k),
                                              _type(HOOK)
    {
        _l0 = GFLdist(_pmat1.position(), _pmat2.position());
        _maxLength = _l0 * 1.5f;
    }

    Link(PMat &pmat1, PMat &pmat2, float k, float z) : _pmat1(pmat1),
                                                       _pmat2(pmat2),
                                                       _k(k),
                                                       _z(z),
                                                       _type(HOOK)
    {
        _l0 = GFLdist(_pmat1.position(), _pmat2.position());
        _maxLength = _l0 * 1.5f;
    }

public:
    std::function<void(void)> _update;

    static Link Hook_Spring(PMat &pmat1, PMat &pmat2, float k)
    {
        return Link(pmat1, pmat2, k);
    }

    static Link Hook_Spring(PMat &pmat1, PMat &pmat2, float k, float z)
    {
        return Link(pmat1, pmat2, k, z);
    }

    static Link Extern_Force(PMat &pmat1, GFLvector *g)
    {
        return Link(pmat1, g);
    }

    void updateHook()
    {
        GFLpoint p1 = _pmat1.position(), p2 = _pmat2.position();
        auto d = GFLdist(p1, p2);

        // Avoid float precision issue
        if (d <= 0.0001f)
        {
            return;
        }

        GFLvector u = gfl_Vector2p(p1, p2);
        u.x /= d;
        u.y /= d;

        GFLvector f = (GFLvector)gfl_mapscal2(u, _k * (d - _l0));

        std::cout << "u.x = " << u.x << std::endl;
        std::cout << "Distance d: " << d << " | Longueur au repos l0: " << _l0 << std::endl;
        std::cout << "Force: " << f.x << ", " << f.y << std::endl;
        std::cout << "P1: " << p1.x << ", " << p1.y << " | P2: " << p2.x << ", " << p2.y << std::endl;

        _pmat1.addForce(f);
        // _pmat2.subForce(f);
        _pmat2.addForce((GFLvector){-f.x, -f.y});
    }

    void update_Damper()
    {
        GFLvector p1 = _pmat1.vitesse(), p2 = _pmat2.vitesse();
        auto deltaV = gfl_SubVect(p1, p2);

        GFLvector f = (GFLvector)gfl_mapscal2(deltaV, -_z);
        _pmat1.addForce(f);
        _pmat2.subForce(f);
    }

    void update_Damper_Hook()
    {
        GFLpoint p1 = _pmat1.position(), p2 = _pmat2.position();
        GFLvector v1 = _pmat1.vitesse(), v2 = _pmat2.vitesse();

        auto d = GFLdist(p1, p2);
        auto deltaV = gfl_SubVect(v2, v1);

        GFLvector u = gfl_Vector2p(p1, p2); // direction
        u.x /= d;
        u.y /= d;

        GFLvector hook = (GFLvector)gfl_mapscal2(u, _k * (d - _l0));
        GFLvector damper = (GFLvector)gfl_mapscal2(deltaV, -_z);

        GFLvector f = GFLvector{hook.x - damper.x, hook.y - damper.y};

        _pmat1.addForce(f);
        _pmat2.addForce((GFLvector){-f.x, -f.y});
    }

    void update_Cond_Damper_Hook()
    {
        GFLpoint p1 = _pmat1.position(), p2 = _pmat2.position();
        GFLvector v1 = _pmat1.vitesse(), v2 = _pmat2.vitesse();

        auto d = GFLdist(p1, p2);
        auto deltaV = gfl_SubVect(v2, v1);

        // if (d <= 1e-7f || d >= _maxLength)
        // {
        //     return;
        // }

        GFLvector u = gfl_NormalVector(gfl_Vector2p(p1, p2)); // direction

        GFLvector hook = (GFLvector)gfl_mapscal2(u, _k * (d - _l0));
        GFLvector damper = (GFLvector)gfl_mapscal2(deltaV, _z);

        GFLvector f = gfl_SubVect(hook, damper);

        _pmat1.addForce(f);
        _pmat2.addForce((GFLvector){-f.x, -f.y});
    }

    void update()
    {
        switch (_type)
        {
        case HOOK:
            update_Cond_Damper_Hook();
            break;

        default:
            _pmat1.addForce(GFLvector{_g->x, _g->y * -1});
            break;
        }
    }

    const PMat &getPmat1() const
    {
        return _pmat1;
    }

    const PMat &getPmat2() const
    {
        return _pmat2;
    }
};
