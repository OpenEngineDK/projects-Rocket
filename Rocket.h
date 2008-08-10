// Rocket class
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------
#ifndef _ROCKET_
#define _ROCKET_

#include <Core/IModule.h>
#include <Math/Vector.h>

//forward declarations
namespace OpenEngine {
    namespace Scene {
        class ISceneNode;
        class TransformationNode;
    }
}

using OpenEngine::Core::IModule;
using OpenEngine::Scene::ISceneNode;
using OpenEngine::Math::Vector;
using OpenEngine::Scene::TransformationNode;

class Rocket : public IModule {
private:
    bool active;
    TransformationNode* root;
    float time;
 
    bool initial; 
    Vector<3,float> vel, grav, thrust; // velocity and gravitation

public:
    Rocket();
    virtual ~Rocket();

    void Initialize();
    void Deinitialize();
    bool IsTypeOf(const std::type_info& inf);
    void Process(const float dt, const float percent);

    void Fire();
    ISceneNode* GetSceneNode();
    
};

#endif
