#include <list>
#include "TurnTo.h"
#include "../EDGameCore/GameObject.h"
#include "../EDGameCore/Transform.h"
#include "../EDGameCore/RigidBody.h"
#include "../EDGameCore/Collider.h"
#include "../EDGameCore/Game.h"
#include "../EDGameCore/GameObject.h"

using namespace std;
using namespace EDGameCore;

TurnTo::TurnTo() {
	target = nullptr;
	targetId = 0;
	turn_speed = 0.0f;
}

void TurnTo::LateUpdate() {
	Transform* looker_transform = GetGameObject()->GetTransform();
	Transform* target_transform = nullptr;

	// Finds the target based on its instance id each update 
	// to avoid having a dangling pointer to an object that might have been destroyed.
	target = GameObject::GetGameObjectInstance(targetId);

	// TODO - comment this out and write your own solution

	target_transform = target->GetTransform();

	Float4x4 lookerMatrix = looker_transform->GetLocalMatrix();
	Float4x4 targetMatrix = target_transform->GetLocalMatrix();

	Float3 lookerPos = Float3(lookerMatrix.WAxis);
	Float3 targetPos = Float3(targetMatrix.WAxis);
	Float3 toTarget = (targetPos - lookerPos).normalize();

	float rotationX = DotProduct(toTarget, lookerMatrix.XAxis);
	float rotationY = -DotProduct(toTarget, lookerMatrix.YAxis);

	lookerMatrix.rotateLocalY(rotationX * EDGameCore::Game::GetDeltaTime());
	lookerMatrix.rotateLocalX(rotationY * EDGameCore::Game::GetDeltaTime());

	Float3 xAxis = ZERO_VECTOR;
	Float3 yAxis = ZERO_VECTOR;
	Float3 zAxis = lookerMatrix.ZAxis.normalize();
	CrossProduct(xAxis, Float3(0.0f, 1.0f, 0.0f), zAxis);
	xAxis.normalize();

	CrossProduct(yAxis, zAxis, xAxis);
	yAxis.normalize();

	Float4x4 localMatrix = Float4x4(
		xAxis.x, xAxis.y, xAxis.z, lookerMatrix.padX,
		yAxis.x, yAxis.y, yAxis.z, lookerMatrix.padY,
		zAxis.x, zAxis.y, zAxis.z, lookerMatrix.padZ,
		lookerMatrix.WAxis.x, lookerMatrix.WAxis.y, lookerMatrix.WAxis.z, lookerMatrix.padW
	);

	looker_transform->SetLocalMatrix(localMatrix);

	//TurnToSolution();
}

void TurnTo::LoadState(TiXmlElement* xmlElement) {
	// Get the name of the target to look-at
	TiXmlElement* targetEle = xmlElement->FirstChildElement("TargetName");
	if ( targetEle != 0 &&
		targetEle->GetText() != 0 &&
		strlen(targetEle->GetText()) != 0 )
		targetName = targetEle->GetText();

	targetEle = xmlElement->FirstChildElement("TurnSpeed");
	if ( targetEle != 0 &&
		targetEle->GetText() != 0 &&
		strlen(targetEle->GetText()) != 0 )
		turn_speed = static_cast<float>(std::atof(targetEle->GetText()));
}

void TurnTo::Awake(void) {
	// Find the first GameObject named targetName.
	GameObject* obj = GameObject::Find(targetName.c_str());

	if ( obj != 0 )
		targetId = obj->GetInstanceId();
}
