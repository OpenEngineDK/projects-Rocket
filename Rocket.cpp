// Rocket class
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include "Rocket.h"

#include <Scene/ISceneNode.h>
#include <Resources/IModelResource.h>
#include <Resources/ResourceManager.h>
#include <Scene/TransformationNode.h>
#include <Resources/DirectoryManager.h>

using OpenEngine::Resources::IModelResource;
using OpenEngine::Resources::IModelResourcePtr;
using OpenEngine::Resources::ResourceManager;
using OpenEngine::Resources::DirectoryManager;

Rocket::Rocket(): 
    active(false), 
    root(NULL), 
    time(0.0),
    vel(Vector<3,float>(5.0,5.0,5.0)), 
    grav(Vector<3,float>(0,-.1,0))
{
    
    DirectoryManager::AppendPath("projects/Rocket/data/");
    
    IModelResourcePtr resource = ResourceManager<IModelResource>::Create("rock.dae");
    resource->Load();
    ISceneNode* model = resource->GetSceneNode();
    resource->Unload();
    root = new TransformationNode();
    root->AddNode(model);
}

Rocket::~Rocket() {

}

void Rocket::Handle(ProcessEventArg arg) {
    float dt = arg.approx * 1000.0f;

    if (!active) return;
    time += dt;
    
    root->Move(vel[0],vel[1],vel[2]);
    vel += grav;
    
}

void Rocket::Fire() {
    logger.info << "FIRE!!!" << logger.end;
    active = true;
}


ISceneNode* Rocket::GetSceneNode() {
    return root;
}
