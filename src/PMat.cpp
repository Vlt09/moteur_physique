#include "PMat.hpp"

PMat::PMat(float m, GFLpoint P0, GFLvector V0, PMat::Model model, bool flag3d) : _m(m),
                                                                                 _pos(P0),
                                                                                 _vit(V0),
                                                                                 _model(model),
                                                                                 _flag3d(flag3d)
{
    switch (model)
    {
    case LEAP_FROG:
        _update = [this](float h)
        { update_leapFrog(h); };
        break;

    default:
        _update = [this](float h)
        { update_euler(h); };
        break;
    }

    _frc = (GFLvector){0., 0.};
}