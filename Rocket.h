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

#include <Core/IListener.h>
#include <Core/EngineEvents.h>
#include <Math/Vector.h>

//forward declarations
namespace OpenEngine {
    namespace Scene {
        class ISceneNode;
        class TransformationNode;
    }
}

using OpenEngine::Core::IListener;
using OpenEngine::Core::ProcessEventArg;
using OpenEngine::Math::Vector;
using OpenEngine::Scene::ISceneNode;
using OpenEngine::Scene::TransformationNode;

class Rocket : public IListener<ProcessEventArg> {
private:
    bool active;
    TransformationNode* root;
    float time;
 
    bool initial; 
    Vector<3,float> vel, grav, thrust; // velocity and gravitation

public:
    Rocket();
    virtual ~Rocket();

    void Handle(ProcessEventArg arg);

    void Fire();
    ISceneNode* GetSceneNode();
    
};

#endif
