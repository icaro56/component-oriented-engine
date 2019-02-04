#ifndef KERNEL_API_H
#define KERNEL_API_H

#include "Core.h"
#include "Logic.h"
#include "MainWindow.h"
#include "engine/SoundEngine.h"

//Components
#include "components/audio/AudioListener.h"
#include "components/audio/AudioSource.h"

#include "components/effects/ParticleSystem.h"
#include "components/effects/TrailRenderer.h"

#include "components/mesh/ManualObject.h"
#include "components/mesh/MeshFilter.h"
#include "components/mesh/MeshRenderer.h"
#include "components/mesh/SkinnedMeshRenderer.h"

#include "components/miscellaneous/Animation.h"

#include "components/physics/BoxCollider.h"
#include "components/physics/CapsuleCollider.h"
#include "components/physics/SphereCollider.h"
#include "components/physics/MeshCollider.h"
#include "components/physics/TerrainCollider.h"
#include "components/physics/Vehicle.h"
#include "components/physics/CharacterController.h"
#include "components/physics/ConstantForce.h"
#include "components/physics/Rigidbody.h"

#include "components/rendering/Billboard.h"
#include "components/rendering/BoundingBox.h"
#include "components/rendering/Camera.h"
#include "components/rendering/FollowText.h"
#include "components/rendering/FollowTexture.h"
#include "components/rendering/Light.h"

#include "components/script/Script.h"

#include "components/transform/Transform.h"

#include "components/ComponentTemplate.h"

//gameobject
#include "gameobject/GameObject.h"
#include "gameobject/GameObjectManager.h"
#include "gameobject/GameObjectTemplate.h"
#include "gameobject/GameObjectTemplateManager.h"
#include "gameobject/LayerMask.h"
#include "gameobject/Tag.h"

//input
#include "input/InputManager.h"
#include "input/JoystickEvents.h"
#include "input/KeyEvents.h"
#include "input/MouseEvents.h"

//engine
#include "engine/callback/CharacterControllerCallback.h"
#include "engine/callback/SceneCallback.h"
#include "engine/callback/SimulationEventCallback.h"

#include "engine/Compositor.h"
#include "engine/CompositorListeners.h"
#include "engine/Engine.h"
#include "engine/RenderingEngine.h"
#include "engine/RenderView.h"
#include "engine/Scene.h"
#include "engine/SkyXManager.h"
#include "engine/StereoManager.h"

//math
#include "math/Bounds.h"
#include "math/KernelMath.h"
#include "math/Matrix3.h"
#include "math/Matrix4.h"
#include "math/Quaternion.h"
#include "math/Vector2.h"
#include "math/Vector3.h"
#include "math/Vector4.h"

//time
#include "time/Time.h"

//utils
#include "utils/Cooker.h"
#include "utils/KernelConverter.h"
#include "utils/Streams.h"
#include "utils/DebugClass.h"







#endif