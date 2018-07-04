#include "Collision.h"

Collision::Collision()
{
}

Collision::~Collision()
{
}



RECT Collision::GetBroadphaseRect(GameObject * GameObject, float DeltaTime)
{
	// quãng đường đi được sau Deltatime
	POINT distance = POINT(GameObject->getVelocity().x * DeltaTime, GameObject->getVelocity().y * DeltaTime);

	RECT rect;
	/*rect.top = distance.y > 0 ? (GameObject->getPosition().y + GameObject->getSize().y / 2) + distance.y : GameObject->getPosition().y + GameObject->getSize().y / 2;
	rect.bottom = distance.y > 0 ? GameObject->getPosition().y - GameObject->getSize().y / 2 : GameObject->getPosition().y - GameObject->getSize().y / 2 + distance.y;
	rect.left = distance.x > 0 ? GameObject->getPosition().x - GameObject->getSize().x / 2 : GameObject->getPosition().x - GameObject->getSize().x / 2 + distance.x;
	rect.right = distance.x > 0 ? GameObject->getPosition().x + GameObject->getSize().x / 2 + distance.x : GameObject->getPosition().x + GameObject->getSize().x / 2;*/

	rect.top = distance.y > 0 ? (GameObject->getPosition().y) + distance.y : GameObject->getPosition().y;
	rect.bottom = distance.y > 0 ? GameObject->getPosition().y - GameObject->getSize().y : GameObject->getPosition().y - GameObject->getSize().y + distance.y;
	rect.left = distance.x > 0 ? GameObject->getPosition().x : GameObject->getPosition().x + distance.x;
	rect.right = distance.x > 0 ? GameObject->getPosition().x + GameObject->getSize().x + distance.x : GameObject->getPosition().x + GameObject->getSize().x;

	return rect;
}

RECT Collision::GetRECT(GameObject *GameObject)
{
	RECT rect;
	/*rect.top = GameObject->getPosition().y + GameObject->getSize().y / 2;
	rect.bottom = GameObject->getPosition().y - GameObject->getSize().y / 2;
	rect.left = GameObject->getPosition().x - GameObject->getSize().x / 2;
	rect.right = GameObject->getPosition().x + GameObject->getSize().x / 2;*/

	rect.top = GameObject->getPosition().y;
	rect.bottom = GameObject->getPosition().y - GameObject->getSize().y;
	rect.left = GameObject->getPosition().x;
	rect.right = GameObject->getPosition().x + GameObject->getSize().x;

	return rect;
}

bool Collision::Collide(GameObject * targetGameObject, GameObject * otherGameObject, float DeltaTime)
{
	//if (!IsOverlayingBroadphaseRect(targetGameObject, otherGameObject, DeltaTime))
	//{
	//	_CollisionDirection.x = 0.0f;
	//	_CollisionDirection.y = 0.0f;
	//	_CollisionRatio = 1.0f;
	//	_RemainingTime = std::numeric_limits<float>::infinity();
	//	_CollisionPosition.x = 0.0f;
	//	_CollisionPosition.y = 0.0f;
	//	return false;
	//}

	POINT targetVelocity = targetGameObject->getVelocity();
	POINT otherVelocity = otherGameObject->getVelocity();

	//tính toán dx entry và dx exit, có 2 trường hợp là vật a di chuyển ngược và xuôi với trục toạ độ
	target_tempvx = targetVelocity.x - otherVelocity.x;
	target_tempvy = targetVelocity.y - otherVelocity.y;

	if (target_tempvx > 0.0f)
	{
		dxentry = (otherGameObject->getPosition().x - otherGameObject->getSize().x / 2) - (targetGameObject->getPosition().x + targetGameObject->getSize().x / 2);
		dxexit = (otherGameObject->getPosition().x + otherGameObject->getSize().x / 2) - (targetGameObject->getPosition().x - targetGameObject->getSize().x / 2);
	}
	else
	{
		dxentry = (otherGameObject->getPosition().x + otherGameObject->getSize().x / 2) - (targetGameObject->getPosition().x - targetGameObject->getSize().x / 2);
		dxexit = (otherGameObject->getPosition().x - otherGameObject->getSize().x / 2) - (targetGameObject->getPosition().x + targetGameObject->getSize().x / 2);
	}

	//tính toán dy entry và exit, tương tự với dx entry/ exit
	if (target_tempvy > 0.0f)
	{
		dyentry = (otherGameObject->getPosition().y - otherGameObject->getSize().y / 2) - (targetGameObject->getPosition().y + targetGameObject->getSize().y / 2);
		dyexit = (otherGameObject->getPosition().y + otherGameObject->getSize().y / 2) - (targetGameObject->getPosition().y - targetGameObject->getSize().y / 2);
	}
	else
	{
		dyentry = (otherGameObject->getPosition().y + otherGameObject->getSize().y / 2) - (targetGameObject->getPosition().y - targetGameObject->getSize().y / 2);
		dyexit = (otherGameObject->getPosition().y - otherGameObject->getSize().y / 2) - (targetGameObject->getPosition().y + targetGameObject->getSize().y / 2);
	}

	//tính toán t x entry/ exit
	if (targetVelocity.x == 0.0f) //tránh trường hợp a.velocity = 0 dẫn tới việc chia cho 0, nên ta gán x entry/ exit = +/-vô cùng
	{
		rxentry = -std::numeric_limits<float>::infinity();
		rxexit = std::numeric_limits<float>::infinity();
	}
	else
	{
		rxentry = dxentry / (target_tempvx*DeltaTime);
		rxexit = dxexit / (target_tempvx*DeltaTime);
	}

	//tính toán t y entry/ exit, tương tự x entry/ exit
	if (targetVelocity.y == 0.0f)
	{
		ryentry = -std::numeric_limits<float>::infinity();
		ryexit = std::numeric_limits<float>::infinity();
	}
	else
	{
		ryentry = dyentry / (target_tempvy*DeltaTime);
		ryexit = dyexit / (target_tempvy*DeltaTime);
	}

	// tính toán thời gian va chạm và thoát khỏi thực sự của vật a chuyển động đối với vật b đứng yên
	rentry = max(rxentry, ryentry);
	rexit = min(rxexit, ryexit);

	//sau khi tính toán được thời gian thực sự va chạm và thoát khỏi, ta kiểm tra xem việc va chạm có xảy ra hay không
	if ((rentry > rexit) || //trường hợp không xảy ra va chạm 1: thời gian thực sự xảy ra va chạm > thời gian thực sự a thoát khỏi b

		(rxentry < 0.0f && ryentry < 0.0f) || //trường hợp không xảy ra va chạm thứ 2: vật a có vận tốc = 0 dẫn đến x entry/ y entry = -vô cùng, hoặc vật a di chuyển hướng ra khỏi vật b

		(rxentry > 1.0f) || (ryentry > 1.0f)) //trường hợp không xảy ra va chạm thứ 3: trong khoảng thời gian delta_time đang xét (thời gian của 1 frame) thì vật a di chuyển chưa tới vật b
	{
		_CollisionDirection.x = NOT_COLLIDED;
		_CollisionDirection.y = NOT_COLLIDED;

		//int touching = IsTouching(targetGameObject, otherGameObject);
		//if (touching == 1 && targetVelocity.y != 0)
		//{
		//	_Listener->OnCollisionExit(targetGameObject, otherGameObject, _CollisionDirection);
		//}
		//else
		//{

		//	if (touching == 2 && targetVelocity.x != 0)
		//	{
		//		_Listener->OnCollisionExit(targetGameObject, otherGameObject, _CollisionDirection);
		//	}
		//}

		//if (IsPreviousCollding(targetGameObject, otherGameObject,DeltaTime))
		//{
		//	_Listener->OnCollisionExit(targetGameObject, otherGameObject, _CollisionDirection);
		//}

		_CollisionRatio = 1.0f;
		_RemainingTime = std::numeric_limits<float>::infinity();
		_CollisionPosition.x = 0.0f;
		_CollisionPosition.y = 0.0f;
		return false;
	}
	else //xảy ra va chạm, chia làm 4 trường hợp, tương ứng với 4 hướng va chạm của vật a so với vật b
	{
		if (rxentry > ryentry)//nếu việc xảy ra va chạm nằm trên trục x 
		{
			// nếu vật a đang nằm ngay sát vật b thì trả về là đang va chạm
			// và ngăn không cho vật a thay đổi theo x
			if (dxentry == 0.0f)//nếu vật a ngay sát bên phải/trái vật b thì không cho xảy ra va chạm
			{
				_CollisionDirection.x = -targetVelocity.x;
				_CollisionDirection.y = NOT_COLLIDED; //hardcode 100.0f để báo là ko va chạm theo chiều này
			}
			else
			{
				if (dxentry < 0.0f)//nếu vật a nằm bên phải vật b => vật a va chạm cạnh bên phải của hình bao vật b
				{
					_CollisionDirection.x = abs(targetVelocity.x);
					_CollisionDirection.y = NOT_COLLIDED; //hardcode 100.0f để báo là ko va chạm theo chiều này
				}
				else //nếu vật a nằm bên trái vật b => vật a va chạm cạnh bên trái của hình bao vật b
				{
					_CollisionDirection.x = -abs(targetVelocity.x);
					_CollisionDirection.y = NOT_COLLIDED; //hardcode 100.0f để báo là ko va chạm theo chiều này
				}
			}
		}
		else//nếu việc xảy ra va chạm nằm trên trục y
		{
			// nếu vật a đang nằm ngay sát vật b thì trả về là đang va chạm
			// và ngăn không cho vật a thay đổi theo y
			if (dyentry == 0.0f)
			{
				_CollisionDirection.x = NOT_COLLIDED; //hardcode 100.0f để báo là ko va chạm theo chiều này
				_CollisionDirection.y = -targetVelocity.y;
			}
			else
			{
				if (dyentry < 0.0f)//nếu vật a nằm bên dưới vật b => vật a sẽ va chạm cạnh dưới hình bao vât b
				{
					_CollisionDirection.x = NOT_COLLIDED; //hardcode 100.0f để báo là ko va chạm theo chiều này
					_CollisionDirection.y = abs(targetVelocity.y);
				}
				else//nếu vật a nằm bên trên vật b => vật a sẽ va chạm cạnh trên hình bao vât b
				{
					_CollisionDirection.x = NOT_COLLIDED; //hardcode 100.0f để báo là ko va chạm theo chiều này
					_CollisionDirection.y = -abs(targetVelocity.y);
				}
			}
		}

		//if (!targetGameObject->_IsSensor)
		//{
		//	if (rxentry != 0 && ryentry != 0)
		//	{
		//		_Listener->OnCollisionEnter(targetGameObject, otherGameObject, _CollisionDirection);
		//	}
		//	else
		//	{
		//		_Listener->OnColliding(targetGameObject, otherGameObject, _CollisionDirection);
		//	}
		//}
		//else
		//{
		//	_Listener->OnSersorEnter(targetGameObject, otherGameObject);
		//	IsSensorEntered = true;
		//}



		_CollisionRatio = rentry;
		_RemainingTime = DeltaTime - rentry * DeltaTime;

		_CollisionPosition.x = targetGameObject->getPosition().x + rentry * DeltaTime*targetVelocity.x;
		_CollisionPosition.y = targetGameObject->getPosition().y + rentry * DeltaTime*targetVelocity.y;


		return true;
	}
}

bool Collision::CanMaskCollide(GameObject * targetGameObject, GameObject * otherGameObject)
{
	auto e1 = targetGameObject->getBitMask() & otherGameObject->getCategoryMask();
	auto e2 = targetGameObject->getCategoryMask() & otherGameObject->getBitMask();
	return (e1 != 0 && e2 != 0);
}

bool Collision::IsOverlayingRect(const RECT & rect1, const RECT & rect2)
{
#if _DEBUG
	bool temp = !(rect1.right < rect2.left || rect1.left > rect2.right ||
		rect1.bottom > rect2.top || rect1.top < rect2.bottom);
	return temp;
#endif
	//if it's not overlaying rect
	return !(rect1.right < rect2.left || rect1.left > rect2.right ||
		rect1.bottom > rect2.top || rect1.top < rect2.bottom);
}

void Collision::UpdateTargetPosition(GameObject * GameObject, const POINT & move)
{
	if (move.x == 0 && move.y == 0)
		GameObject->setPosition(_CollisionPosition.x, _CollisionPosition.y);
	else {
		GameObject->setPosition(GameObject->getPosition().x + move.x, GameObject->getPosition().y + move.y);
	}
}

void Collision::PerformCollision(GameObject * targetGameObject, GameObject * otherGameObject, float DeltaTime, int collisionAction, bool & needMoveX, bool & needMoveY)
{
	//if (targetGameObject->IsSensor()) return; // this is trigger object

	POINT targetVelocity = targetGameObject->getVelocity();

	if (_CollisionDirection != POINT(NOT_COLLIDED, NOT_COLLIDED))
	{
		// nếu va chạm theo trục x
		if (_CollisionDirection.x == targetVelocity.x * -1)
		{
			// cập nhật tọa độ
			UpdateTargetPosition(targetGameObject, POINT(0, 0));

			// ngăn không cho targetObject di chuyển theo x trong hàm Body->Next trong vòng update World
			needMoveX = false;
		}
		else
		{
			//nếu va chạm theo trục y
			if (_CollisionDirection.y == targetVelocity.y * -1)
			{
				// cập nhật tọa độ
				UpdateTargetPosition(targetGameObject, POINT(0, 0));

				// ngăn không cho targetObject di chuyển theo y trong hàm Body->Next trong vòng update World
				needMoveY = false;
			}
		}
	}
}

