#include "Factory.h"

// project includes

#include "Rocket.h"

// from OpenEngine
#include <Logging/Logger.h>
#include <Display/Frustum.h>
#include <Display/Viewport.h>
#include <Display/ViewingVolume.h>
#include <Scene/GeometryNode.h>
#include <Scene/TransformationNode.h>
#include <Resources/ResourceManager.h>
#include <Utils/Statistics.h>

#include <Display/SDLFrame.h>
#include <Devices/SDLInput.h>

#include <Renderers/OpenGL/Renderer.h>
#include <Renderers/OpenGL/RenderingView.h>
#include <Renderers/OpenGL/TextureLoader.h>

#include <Sound/OpenALSoundSystem.h>
#include <Sound/ISoundSystem.h>
#include <Sound/ISound.h>
#include <Scene/SceneNode.h>
#include <Scene/SoundNode.h>
#include <Sound/SoundRenderer.h>

#include <Resources/VorbisResource.h>
#include <Resources/ISoundResource.h>
#include <Resources/TGAResource.h>
#include <Resources/ITextureResource.h>
#include <Resources/IModelResource.h>
#include <Resources/ColladaResource.h>


#include <Scene/SphereNode.h>

#include <Scene/DirectionalLightNode.h>
#include <Scene/PointLightNode.h>

#include <Utils/MoveHandler.h>
#include <Utils/QuitHandler.h>
//#include <Utils/PropertyList.h>
#include <Sky/SkyDome.h>

#include <Core/IModule.h>


#include <string>

using namespace OpenEngine::Logging;
using namespace OpenEngine::Core;
using namespace OpenEngine::Display;
using namespace OpenEngine::Renderers::OpenGL;
using namespace OpenEngine::Scene;
using namespace OpenEngine::Sound;
using namespace OpenEngine::Resources;
using namespace OpenEngine::Utils;

using OpenEngine::Sky::SkyDome;

class Handler : public IListener<KeyboardEventArg> {
private:
    Rocket* rocket;
public:

    Handler(Rocket* rocket): rocket(rocket){}
    ~Handler() {}
    
    void Handle(KeyboardEventArg arg) {
        if (arg.type == EVENT_PRESS) {
            switch (arg.sym) {
            case KEY_f: rocket->Fire(); break;
            default: 
                break;
            }
        }
    }
};


Factory::Factory() {
    frame    = new SDLFrame(800, 600, 32);
    viewport = new Viewport(*frame);
        
    camera = new Camera(*(new ViewingVolume()));
    camera->SetPosition(Vector<3,float>(0,0,0));
    //viewport->SetViewingVolume(camera);

    // frustum hack
    Frustum* frustum = new Frustum(*camera);
    frustum->SetFar(15000);
    frustum->SetNear(1);
    viewport->SetViewingVolume(frustum);

    renderer = new Renderer(viewport);

    // Add rendering initialization tasks
    TextureLoader* tl = new TextureLoader();
    renderer->InitializeEvent().Attach(*tl);
}

Factory::~Factory() {
    delete frame;
    delete viewport;
    delete camera;
    delete renderer;
}

bool Factory::SetupEngine(IEngine& engine) {
    engine.InitializeEvent().Attach(*frame);
    engine.ProcessEvent().Attach(*frame);
    engine.DeinitializeEvent().Attach(*frame);

    engine.InitializeEvent().Attach(*renderer);
    engine.ProcessEvent().Attach(*renderer);
    engine.DeinitializeEvent().Attach(*renderer);

    // Setup input handling
    SDLInput* input = new SDLInput();
    engine.InitializeEvent().Attach(*input);
    engine.ProcessEvent().Attach(*input);
    engine.DeinitializeEvent().Attach(*input);

    // Register the handler as a listener on up and down keyboard events.
    MoveHandler* move_h = new MoveHandler(*camera,*input);
    input->KeyEvent().Attach(*move_h);

    QuitHandler* quit_h = new QuitHandler(engine);
    input->KeyEvent().Attach(*quit_h);

    // Create scene root
    ISceneNode* root = new SceneNode();
    this->renderer->SetSceneRoot(root);

    //First we set the resources directory
    //         DirectoryManager::AppendPath("/");
    DirectoryManager::AppendPath(""); //current directory
    // load the resource plug-ins
    ResourceManager<ISoundResource>::AddPlugin(new VorbisResourcePlugin());
    ResourceManager<ITextureResource>::AddPlugin(new TGAPlugin());
    ResourceManager<IModelResource>::AddPlugin(new ColladaPlugin());

    ISoundSystem* openalsmgr = new OpenALSoundSystem(root, camera);
    engine.ProcessEvent().Attach(*openalsmgr);

//     SoundRenderer* sr = new SoundRenderer();
//     renderer->preProcess.Attach(*sr);

    // load skydome
//     string skyfile = "skydome.txt";
//     PropertyList* skylist = new PropertyList(skyfile);
//     SkyDome* sky = new SkyDome(skylist);
//     root->AddNode(sky->GetSceneNode());
    
    // lighting setup
    TransformationNode* tn = new TransformationNode();
    tn->Rotate(0,0,-45);
    DirectionalLightNode* dln = new DirectionalLightNode();
    tn->AddNode(dln);
    root->AddNode(tn);

    //engine.AddModule(*(new Statistics(1000)));


    // setup rocket
    Rocket* r = new Rocket();
    root->AddNode(r->GetSceneNode());
    engine.ProcessEvent().Attach(*r);

    Handler* h = new Handler(r);
    input->KeyEvent().Attach(*h);
    
    // Return true to signal success.
    return true;
}

// Get methods for the factory.
IFrame*      Factory::GetFrame()      { return frame; }
IRenderer*   Factory::GetRenderer()   { return renderer; }
